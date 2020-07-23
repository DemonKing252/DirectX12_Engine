#include "Engine.h"
#include "Win32App.h"
Engine* Engine::s_pInstance = nullptr;

Engine::Engine() : D3DApp()
{
}

Engine::~Engine()
{
}

Engine * Engine::GetApp()
{
	if (s_pInstance == nullptr)
	{
		s_pInstance = new Engine();
		return s_pInstance;
	}
	return s_pInstance;
}

void Engine::Initialize(const std::shared_ptr<Win32App> window, const LPCWSTR vsPath, const LPCWSTR psPath)
{
	BuildDeviceAndSwapChain(window);
	BuildCommandObjects();
	BuildDescriptorHeaps();
	BuildRenderTargetViews();
	BuildDepthStencilViews();
	BuildRootSignature();
	AssemblePipeline(vsPath, psPath);

	GeometryGenerator geoGen;
	DirectX::XMFLOAT3 Positions[10] = 
	{
		{-1.0f, 0.5f, +0.0f },
		{+1.0f, 0.5f, +0.0f },
		{-1.0f, 0.5f, +1.0f },
		{+1.0f, 0.5f, +1.0f },
		{-1.0f, 0.5f, -1.0f },
		{+1.0f, 0.5f, -1.0f },
		{-1.0f, 0.5f, -2.0f },
		{+1.0f, 0.5f, -2.0f },
		{-1.0f, 0.5f, +2.0f },
		{+1.0f, 0.5f, +2.0f },
	};

	for (UINT i = 0; i < _countof(Positions); i++)
	{
		m_meshes.push_back(std::make_shared<MeshGeometry>());
		m_meshes.back() = geoGen.CreateCylinder(m_device.Get(), 0.1f, 0.15f, 1.0f, 30);

		m_meshes.back()->AddComponent<MaterialComponent>();
		m_meshes.back()->GetComponent<MaterialComponent>().DiffuseAlbedo = { 0.0f, 0.5f, 1.0f, 1.0f };

		DirectX::XMMATRIX model;
		model = DirectX::XMMatrixTranslation(Positions[i].x, Positions[i].y, Positions[i].z);

		m_meshes.back()->AddComponent<TransformComponent>();
		m_meshes.back()->GetComponent<TransformComponent>().SetModelMatrix(model);

		ID3D12Resource* temp;
		ZeroMemory(&temp, sizeof(ID3D12Resource));
		m_cbvResources.push_back(temp);

		// ----------------------

		m_meshes.push_back(std::make_shared<MeshGeometry>());
		m_meshes.back() = geoGen.CreateSphere(m_device.Get(), 0.1f, 10, 20);

		m_meshes.back()->AddComponent<MaterialComponent>(); 
		m_meshes.back()->GetComponent<MaterialComponent>().DiffuseAlbedo = { 1.0f, 0.0f, 0.0f, 1.0f };

		model = DirectX::XMMatrixTranslation(Positions[i].x, Positions[i].y+0.6f, Positions[i].z);

		m_meshes.back()->AddComponent<TransformComponent>();
		m_meshes.back()->GetComponent<TransformComponent>().SetModelMatrix(model);

		ID3D12Resource* temp2;
		ZeroMemory(&temp2, sizeof(ID3D12Resource));
		m_cbvResources.push_back(temp2);
	}

	{
		m_meshes.push_back(std::make_shared<MeshGeometry>());
		m_meshes.back() = geoGen.CreateBox(m_device.Get(), 0.5f, 0.25f, 0.5f);

		m_meshes.back()->AddComponent<MaterialComponent>();
		m_meshes.back()->GetComponent<MaterialComponent>().DiffuseAlbedo = { 0.0f, 0.5f, 0.0f, 1.0f };

		DirectX::XMMATRIX model;
		model = DirectX::XMMatrixTranslation(0.0f, 0.125f, 0.0f);
	
		m_meshes.back()->AddComponent<TransformComponent>();
		m_meshes.back()->GetComponent<TransformComponent>().SetModelMatrix(model);
	
		ID3D12Resource* temp;
		ZeroMemory(&temp, sizeof(ID3D12Resource));
	
		m_cbvResources.push_back(temp);
	}
	{
		m_meshes.push_back(std::make_shared<MeshGeometry>());
		m_meshes.back() = geoGen.CreateGrid(m_device.Get(), 3.0f, 5.0f);

		m_meshes.back()->AddComponent<MaterialComponent>(); 
		m_meshes.back()->GetComponent<MaterialComponent>().DiffuseAlbedo = { 0.0f, 0.7f, 0.0f, 1.0f };

		DirectX::XMMATRIX model;
		model = DirectX::XMMatrixTranslation(0.0f, -0.0001f, 0.0f);

		m_meshes.back()->AddComponent<TransformComponent>();
		m_meshes.back()->GetComponent<TransformComponent>().SetModelMatrix(model);

		ID3D12Resource* temp;
		ZeroMemory(&temp, sizeof(ID3D12Resource));
		m_cbvResources.push_back(temp);
	}
	
	BuildConstantBufferViews();

	// View port and scissors rect is in d3dApp since I need the window dimensions.
}

void Engine::Update()
{
	// { 0, 1, 2, 0, 1, 2, ... }
	// Next buffer
	m_iBufferIndex = (m_iBufferIndex + 1) % m_iNumBuffers;

}

void Engine::Draw()
{
	CD3DX12_CPU_DESCRIPTOR_HANDLE currentRTVHandle(m_rtvDescriptorHeap->GetCPUDescriptorHandleForHeapStart());
	currentRTVHandle.Offset(1, m_iBufferIndex * m_device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV));

	// Indicate that the back buffer will be used as a render target (according to Hooman's slides)
	m_commandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(
		m_renderTargets[m_iBufferIndex].Get(), D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET));

	m_commandList->OMSetRenderTargets(1, &currentRTVHandle, false, &m_dsvHeap->GetCPUDescriptorHandleForHeapStart());

	float ClearColor[] = { 0.0f, 0.0f, 0.3f, 1.0f };
	m_commandList->ClearRenderTargetView(currentRTVHandle, ClearColor, 0, nullptr);
	m_commandList->ClearDepthStencilView(m_dsvHeap->GetCPUDescriptorHandleForHeapStart(),
		D3D12_CLEAR_FLAG_DEPTH | D3D12_CLEAR_FLAG_STENCIL, 1.0f, 0, 0, nullptr);

	m_commandList->SetGraphicsRootSignature(m_rootSignature.Get());
	m_commandList->RSSetViewports(1, &m_viewPort);
	m_commandList->RSSetScissorRects(1, &m_scissorsRect);

	for (int i = 0; i < (std::uint16_t)m_meshes.size(); i++)
	{
		m_constantBuffer->Model = m_meshes[i]->GetComponent<TransformComponent>().GetModelMatrix();
		m_constantBuffer->DiffuseAlbedo = m_meshes[i]->GetComponent<MaterialComponent>().DiffuseAlbedo;
		this->UpdateConstants();

		void* data;
		m_cbvResources[i]->Map(0, nullptr, reinterpret_cast<void**>(&data));
		CopyMemory(data, m_constantBuffer.get(), sizeof(ConstantBuffer));
		m_cbvResources[i]->Unmap(0, nullptr);

		m_commandList->SetGraphicsRootConstantBufferView(0, m_cbvResources[i]->GetGPUVirtualAddress());
		m_commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		m_commandList->IASetVertexBuffers(0, 1, &m_meshes[i]->GetComponent<VertexBufferComponent<Vertex>>().Get());
		m_commandList->IASetIndexBuffer(&m_meshes[i]->GetComponent<IndexBufferComponent<std::uint16_t>>().Get());

		m_commandList->DrawIndexedInstanced(m_meshes[i]->IndexCount, 1, 0, 0, 0);
	}

	// Indicate that the back buffer will be used to present (according to Hooman's slides)
	m_commandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(
		m_renderTargets[m_iBufferIndex].Get(), D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT));

	m_commandList->Close();

	ID3D12CommandList* cmdLists[] = { m_commandList.Get() };
	m_commandQueue->ExecuteCommandLists(_countof(cmdLists), cmdLists);
}

void Engine::SwapBuffers() const
{
	ThrowIfFailed(m_dxgiSwapChain1->Present(1, 0));
}

void Engine::Clean()
{
	WaitForPreviousFrame();
}

void Engine::UpdateConstants()
{
	Camera::UpdateEyePosition();

	m_constantBuffer->View = DirectX::XMMatrixLookAtLH
	(
		DirectX::XMLoadFloat4(&Camera::Eye),
		DirectX::XMLoadFloat4(&Camera::Focus),
		DirectX::XMLoadFloat4(&Camera::Up)
	);
	m_constantBuffer->Projection = DirectX::XMMatrixPerspectiveFovLH(DirectX::XMConvertToRadians(45.0f), 4.0f / 3.0f, 0.1f, 300.0f);

	m_constantBuffer->World = DirectX::XMMatrixTranspose
	(
		m_constantBuffer->Model *
		m_constantBuffer->View *
		m_constantBuffer->Projection
	);
}

void Engine::BuildDescriptorHeaps()
{
	// Render Target View
	D3D12_DESCRIPTOR_HEAP_DESC rtvDescHeapDesc;
	ZeroMemory(&rtvDescHeapDesc, sizeof(D3D12_DESCRIPTOR_HEAP_DESC));

	rtvDescHeapDesc.NumDescriptors = m_iNumBuffers;
	rtvDescHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;

	ThrowIfFailed(m_device->CreateDescriptorHeap(&rtvDescHeapDesc, IID_PPV_ARGS(m_rtvDescriptorHeap.GetAddressOf())));

	D3D12_DESCRIPTOR_HEAP_DESC depthStencilHeap = {};
	depthStencilHeap.NumDescriptors = 1;
	depthStencilHeap.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;

	ThrowIfFailed(m_device->CreateDescriptorHeap(&depthStencilHeap, IID_PPV_ARGS(&m_dsvHeap)));
}

void Engine::BuildConstantBufferViews()
{
	m_constantBuffer = std::make_unique<ConstantBuffer>();

	const UINT cBufferSize = sizeof(ConstantBuffer);
	UINT AlignedBufferSize = 0U;
		
	// What's happening here? 
	// We have to determine an allligned size for our constant buffer where our constant buffer is a multiple of 256 bytes.
	// Remember that ALL constant buffers have to be a multiple of 256 bytes. No more AND No less!!!
	for (UINT i = 0; i < (UINT)std::ceil((float)cBufferSize / 256U); i++)
		if (cBufferSize > 256U * i)
			AlignedBufferSize += 256U;
	
	for (int i = 0; i < (std::uint16_t)m_cbvResources.size(); i++)
	{
		ThrowIfFailed(m_device->CreateCommittedResource(
			&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
			D3D12_HEAP_FLAG_NONE,
			&CD3DX12_RESOURCE_DESC::Buffer(AlignedBufferSize),
			D3D12_RESOURCE_STATE_GENERIC_READ,
			nullptr,
			IID_PPV_ARGS(m_cbvResources[i].GetAddressOf())
		));

		void* data;
		m_cbvResources[i]->Map(0, nullptr, reinterpret_cast<void**>(&data));
		CopyMemory(data, m_constantBuffer.get(), cBufferSize);
		m_cbvResources[i]->Unmap(0, nullptr);
	}
}

void Engine::BuildRootSignature()
{
	/*
	Slot Parameters in our Root Signature:
		1. We have a Constant Buffer declared at base register 0
	*/

	CD3DX12_ROOT_PARAMETER slotParameters[1];

	slotParameters[0].InitAsConstantBufferView(0);

	CD3DX12_ROOT_SIGNATURE_DESC rootSignatureDesc;
	ZeroMemory(&rootSignatureDesc, sizeof(CD3DX12_ROOT_SIGNATURE_DESC));

	rootSignatureDesc.Init(_countof(slotParameters), slotParameters, 0, 0, D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

	ID3DBlob* signatureBlob;
	ThrowIfFailed(D3D12SerializeRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1, &signatureBlob, nullptr));

	ThrowIfFailed(m_device->CreateRootSignature(
		0,
		signatureBlob->GetBufferPointer(),
		signatureBlob->GetBufferSize(),
		IID_PPV_ARGS(m_rootSignature.GetAddressOf()
		)));
}

void Engine::AssemblePipeline(const LPCWSTR vsPath, const LPCWSTR psPath)
{
	ID3DBlob* vs, *ps;

	ThrowIfFailed(D3DCompileFromFile(vsPath, 0, 0, "VSMain", "vs_4_0", 0, 0, &vs, 0));
	ThrowIfFailed(D3DCompileFromFile(psPath, 0, 0, "PSMain", "ps_4_0", 0, 0, &ps, 0));

	D3D12_INPUT_ELEMENT_DESC inputLayout[3] =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
		{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
		{"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 20, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0}
	};

	D3D12_RASTERIZER_DESC rasDesc;
	ZeroMemory(&rasDesc, sizeof(D3D12_RASTERIZER_DESC));
	
	rasDesc.CullMode = D3D12_CULL_MODE_BACK;
	rasDesc.FillMode = D3D12_FILL_MODE_SOLID;
	rasDesc.AntialiasedLineEnable = true;
	rasDesc.MultisampleEnable = true;

	D3D12_GRAPHICS_PIPELINE_STATE_DESC psoDesc;
	ZeroMemory(&psoDesc, sizeof(D3D12_GRAPHICS_PIPELINE_STATE_DESC));

	psoDesc.RasterizerState = rasDesc;
	psoDesc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
	psoDesc.VS = CD3DX12_SHADER_BYTECODE(vs);
	psoDesc.PS = CD3DX12_SHADER_BYTECODE(ps);
	psoDesc.SampleDesc.Count = 1;
	psoDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
	psoDesc.SampleMask = UINT_MAX;
	psoDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	psoDesc.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);
	psoDesc.DSVFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;
	psoDesc.InputLayout = { inputLayout, _countof(inputLayout) };
	psoDesc.NumRenderTargets = 1;
	psoDesc.pRootSignature = m_rootSignature.Get();

	ThrowIfFailed(m_device->CreateGraphicsPipelineState(&psoDesc, IID_PPV_ARGS(m_pipelineState.GetAddressOf())));
}
