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

void Engine::Initialize(const std::shared_ptr<Win32App> window) 
{
	BuildDeviceAndSwapChain(window);
	BuildCommandObjects();
	BuildDescriptorHeaps();
	BuildRenderTargetViews();

	CD3DX12_ROOT_SIGNATURE_DESC rootSignatureDesc;
	ZeroMemory(&rootSignatureDesc, sizeof(CD3DX12_ROOT_SIGNATURE_DESC));

	rootSignatureDesc.Init(0, 0, 0, 0, D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

	ID3DBlob* signatureBlob;
	ThrowIfFailed(D3D12SerializeRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1, &signatureBlob, nullptr));

	ThrowIfFailed(m_device->CreateRootSignature(
		0,
		signatureBlob->GetBufferPointer(),
		signatureBlob->GetBufferSize(),
		IID_PPV_ARGS(m_rootSignature.GetAddressOf()	
	)));
	ID3DBlob* vs, *ps;

	ThrowIfFailed(D3DCompileFromFile(L"VS.hlsl", 0, 0, "VSMain", "vs_4_0", 0, 0, &vs, 0));
	ThrowIfFailed(D3DCompileFromFile(L"PS.hlsl", 0, 0, "PSMain", "ps_4_0", 0, 0, &ps, 0));

	D3D12_INPUT_ELEMENT_DESC inputLayout[1] = 
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0}
	};

	D3D12_GRAPHICS_PIPELINE_STATE_DESC psoDesc;
	ZeroMemory(&psoDesc, sizeof(D3D12_GRAPHICS_PIPELINE_STATE_DESC));
	
	psoDesc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
	psoDesc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
	psoDesc.VS = CD3DX12_SHADER_BYTECODE(vs);
	psoDesc.PS = CD3DX12_SHADER_BYTECODE(ps);
	psoDesc.SampleDesc.Count = 1;
	psoDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
	psoDesc.SampleMask = UINT_MAX;
	psoDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	psoDesc.DepthStencilState.DepthEnable = false;
	psoDesc.DepthStencilState.StencilEnable = false;
	psoDesc.InputLayout = { inputLayout, _countof(inputLayout) };
	psoDesc.NumRenderTargets = 1;
	psoDesc.pRootSignature = m_rootSignature.Get();

	ThrowIfFailed(m_device->CreateGraphicsPipelineState(&psoDesc, IID_PPV_ARGS(m_pipelineState.GetAddressOf())));

	Vertex vertices[3] = 
	{
		Vertex(DirectX::XMFLOAT3(-0.5f, -0.5f, +0.0f)),
		Vertex(DirectX::XMFLOAT3(+0.0f, +0.5f, +0.0f)),
		Vertex(DirectX::XMFLOAT3(+0.5f, -0.5f, +0.0f)),
	};
	const UINT vBufferSize = sizeof(vertices);

	ThrowIfFailed(m_device->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer(vBufferSize),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&m_vertexBuffer)
	));

	void* data;
	CD3DX12_RANGE readRange(0, 0);
	m_vertexBuffer->Map(0, &readRange, reinterpret_cast<LPVOID*>(&data));
	CopyMemory(data, vertices, vBufferSize);
	m_vertexBuffer->Unmap(0, 0);

	m_vertexBufferView.BufferLocation = m_vertexBuffer->GetGPUVirtualAddress();
	m_vertexBufferView.SizeInBytes = vBufferSize;
	m_vertexBufferView.StrideInBytes = sizeof(Vertex);

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

	m_commandList->OMSetRenderTargets(1, &currentRTVHandle, false, nullptr);
	m_commandList->ClearRenderTargetView(currentRTVHandle, DirectX::Colors::DarkCyan, 0, nullptr);

	m_commandList->SetGraphicsRootSignature(m_rootSignature.Get());
	m_commandList->RSSetViewports(1, &m_viewPort);
	m_commandList->RSSetScissorRects(1, &m_scissorsRect);

	m_commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	m_commandList->IASetVertexBuffers(0, 1, &m_vertexBufferView);
	
	m_commandList->DrawInstanced(3, 1, 0, 0);

	m_commandList->Close();

	ID3D12CommandList* cmdLists[] = { m_commandList.Get() };
	m_commandQueue->ExecuteCommandLists(_countof(cmdLists), cmdLists);
}

void Engine::SwapBuffers() const
{
	ThrowIfFailed(m_dxgiSwapChain1->Present(1, 0));
}