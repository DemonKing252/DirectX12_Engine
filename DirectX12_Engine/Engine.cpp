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

void Engine::Initialize(GameTimer* gameTimer, const std::shared_ptr<Win32App> window, const LPCWSTR vsPath, const LPCWSTR dafaultpsPath, const LPCWSTR shadowpsPath)
{
	this->BuildDeviceAndSwapChain(window);
	this->BuildCommandObjects();
	this->BuildDescriptorHeaps();
	this->BuildRenderTargetViews();
	this->BuildDepthStencilViews();
	this->BuildShaderResourceViews();
	this->BuildRootSignature();
	this->AssemblePipeline(vsPath, dafaultpsPath, shadowpsPath);

	GeometryGenerator geoGen;
	

	XMFLOAT3 Positions[10] = 
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
		auto cylinder = std::make_shared<MeshGeometry>();
		cylinder = geoGen.CreateCylinder(m_device.Get(), 0.1f, 0.15f, 1.0f, 30);
		
		cylinder->AddComponent<MaterialComponent>();
		cylinder->GetComponent<MaterialComponent>().DiffuseAlbedo = { 1.0f, 1.0f, 1.0f, 1.0f };
		cylinder->GetComponent<MaterialComponent>().FresnelFactor = 2.0f;
		cylinder->GetComponent<MaterialComponent>().texture.hGPUHandle = m_textureMap["roughstone"]->hGPUHandle;
		cylinder->GetComponent<MaterialComponent>().texture.m_texHeap = m_textureMap["roughstone"]->m_texResource;

		XMMATRIX model;
		model = XMMatrixTranslation(Positions[i].x, Positions[i].y, Positions[i].z);

		cylinder->AddComponent<TransformComponent>();
		cylinder->GetComponent<TransformComponent>().SetModelMatrix(model);

		m_meshes[Pipeline::Opaque].push_back(cylinder);
		m_meshes[Pipeline::StencilReflection].push_back(cylinder);
		m_meshes[Pipeline::StencilShadow].push_back(cylinder);

		ID3D12Resource* temp;
		ZeroMemory(&temp, sizeof(ID3D12Resource));
		m_cbvResources[Pipeline::Opaque].push_back(temp);
		m_cbvResources[Pipeline::StencilReflection].push_back(temp);
		m_cbvResources[Pipeline::StencilShadow].push_back(temp);

		// ----------------------

		auto sphere = std::make_shared<MeshGeometry>();
		sphere = geoGen.CreateSphere(m_device.Get(), 0.1f, 30, 40);
		
		sphere->AddComponent<MaterialComponent>(); 
		sphere->GetComponent<MaterialComponent>().DiffuseAlbedo = { 1.0f, 1.0f, 1.0f, 1.0f };
		sphere->GetComponent<MaterialComponent>().FresnelFactor = 2.0f;
		sphere->GetComponent<MaterialComponent>().texture.hGPUHandle = m_textureMap["stone"]->hGPUHandle;
		sphere->GetComponent<MaterialComponent>().texture.m_texHeap = m_textureMap["stone"]->m_texResource;

		model = XMMatrixTranslation(Positions[i].x, Positions[i].y+0.6f, Positions[i].z);

		sphere->AddComponent<TransformComponent>();
		sphere->GetComponent<TransformComponent>().SetModelMatrix(model);
		
		// Create a physical object
		m_meshes[Pipeline::Opaque].push_back(sphere);

		// Reflectable by mirrors
		m_meshes[Pipeline::StencilReflection].push_back(sphere);

		// Create a shadow for this 
		m_meshes[Pipeline::StencilShadow].push_back(sphere);

		ID3D12Resource* temp2;
		ZeroMemory(&temp2, sizeof(ID3D12Resource));
		m_cbvResources[Pipeline::Opaque].push_back(temp2);
		m_cbvResources[Pipeline::StencilReflection].push_back(temp2);
		m_cbvResources[Pipeline::StencilShadow].push_back(temp2);
	}

	{
		auto box = std::make_shared<MeshGeometry>();

		box = geoGen.CreateBox(m_device.Get(), 0.5f, 0.25f, 0.5f);
		
		box->AddComponent<MaterialComponent>();
		box->GetComponent<MaterialComponent>().DiffuseAlbedo = { 1.0f, 1.0f, 1.0f, 1.0f };
		box->GetComponent<MaterialComponent>().FresnelFactor = 2.0f;
		box->GetComponent<MaterialComponent>().texture.hGPUHandle = m_textureMap["stone"]->hGPUHandle;
		box->GetComponent<MaterialComponent>().texture.m_texHeap = m_textureMap["stone"]->m_texResource;

		XMMATRIX model;
		model = XMMatrixTranslation(0.0f, 0.125f, 0.0f);
	
		box->AddComponent<TransformComponent>();
		box->GetComponent<TransformComponent>().SetModelMatrix(model);
		
		m_meshes[Pipeline::Opaque].push_back(box);
		m_meshes[Pipeline::StencilReflection].push_back(box);
		m_meshes[Pipeline::StencilShadow].push_back(box);

		ID3D12Resource* temp;
		ZeroMemory(&temp, sizeof(ID3D12Resource));
		m_cbvResources[Pipeline::Opaque].push_back(temp);
		m_cbvResources[Pipeline::StencilReflection].push_back(temp);
		m_cbvResources[Pipeline::StencilShadow].push_back(temp);

	}
	{
		auto grid = std::make_shared<MeshGeometry>();	
		grid = geoGen.CreateGrid(m_device.Get(), 3.0f, 5.0f, 6, 10);
		
		grid->AddComponent<MaterialComponent>(); 
		grid->GetComponent<MaterialComponent>().DiffuseAlbedo = { 1.0f, 1.0f, 1.0f, 0.5f };
		grid->GetComponent<MaterialComponent>().FresnelFactor = 2.0f;
		grid->GetComponent<MaterialComponent>().texture.hGPUHandle = m_textureMap["glass"]->hGPUHandle;
		grid->GetComponent<MaterialComponent>().texture.m_texHeap = m_textureMap["glass"]->m_texResource;
	
		XMMATRIX model;
		model = XMMatrixTranslation(0.0f, 0.0f, 0.0f);
	
		grid->AddComponent<TransformComponent>();
		grid->GetComponent<TransformComponent>().SetModelMatrix(model);
	
		m_meshes[Pipeline::Transparent].push_back(grid);
		m_meshes[Pipeline::StencilMirror].push_back(grid);
	
		ID3D12Resource* temp;
		ZeroMemory(&temp, sizeof(ID3D12Resource));
		m_cbvResources[Pipeline::Transparent].push_back(temp);
		m_cbvResources[Pipeline::StencilMirror].push_back(temp);
	}
	
	BuildConstantBufferViews();

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();

	font = io.Fonts->AddFontFromFileTTF("assets/arial.ttf", 14.0f);

	IM_ASSERT(font);

	m_himguiCPUHandle = m_imguiDescriptorHeap->GetCPUDescriptorHandleForHeapStart();
	m_himguiGPUHandle = m_imguiDescriptorHeap->GetGPUDescriptorHandleForHeapStart();

	assert(ImGui_ImplWin32_Init(window->Get()));
	assert(ImGui_ImplDX12_Init(m_device.Get(), 3, DXGI_FORMAT_R8G8B8A8_UNORM, m_imguiDescriptorHeap.Get(), m_himguiCPUHandle, m_himguiGPUHandle));
	ImGui::StyleColorsDark();

	m_constantBuffer->AmbientLight = XMFLOAT4(0.2f, 0.2f, 0.2f, 1.0f);
	// View port and scissors rect is in d3dApp since I need the window dimensions.
}

void Engine::Update(GameTimer* gameTimer)
{
	//OutputDebugString(L"\tTime Now: ");
	//OutputDebugString(std::to_wstring(gameTimer->GetTimeNow()).c_str());
	//
	//OutputDebugString(L"Delta Time: ");
	//OutputDebugString(std::to_wstring(gameTimer->GetDeltaTime()).c_str());
	//OutputDebugString()
	//OutputDebugString(L"\n");

	// { 0, 1, 2, 0, 1, 2, ... }
	// Next buffer
	m_iBufferIndex = (m_iBufferIndex + 1) % m_iNumBuffers;
	// bufferIndex++; if (bufferIndex == 3) bufferIndex = 0;
}
/**Don�t forget to use delete[]*/

void Engine::Draw(GameTimer* gameTimer)
{
	ImGui_ImplDX12_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();	
	ImGui::PushFont(font);

	// 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).


	ImGui::Begin("Debug");                          // Create a window called "Hello, world!" and append into it.

	ImGui::SetWindowPos(ImVec2(0.f, 0.f));
	ImGui::SetWindowSize(ImVec2(426.f, 448.f));
	ImGui::Separator();

	float temp[4] = { m_constantBuffer->AmbientLight.x, m_constantBuffer->AmbientLight.y, m_constantBuffer->AmbientLight.z, m_constantBuffer->AmbientLight.w };

	if (ImGui::SliderFloat4("Ambient Light", temp, 0.0f, 1.0f, "%.1f"))
	{
		m_constantBuffer->AmbientLight = { temp[0], temp[1], temp[2], temp[3] };
	}

	ImGui::NewLine();
	ImGui::Text("Stencil Buffer:");               // Display some text (you can use a format strings too)
	ImGui::Separator();
	ImGui::Checkbox("Reflections On", &m_bReflectionsEnabled);      // Edit bools storing our window open/close state
	ImGui::Checkbox("Shadows On", &m_bShadowsEnabled);

	ImGui::NewLine();
	ImGui::Text("Swap Chain:");
	ImGui::Separator();
	ImGui::Checkbox("VSync On", &m_bVSyncEnabled);
	ImGui::SliderFloat4("Render Target Color", clear_color, 0.0f, 1.0f, "%.1f");

	ImGui::NewLine();
	ImGui::Text("Light Information");
	ImGui::Separator();
	ImGui::ColorEdit3("Color", light_color);

	ImGui::NewLine();
	ImGui::Text("Adapters Detected:");
	ImGui::Separator();
	for (auto adapter : m_adapters)
	{
		std::string s(adapter.begin(), adapter.end());

		ImGui::Text(s.c_str());
	}

	
	ImGui::NewLine();
	ImGui::Text("DirectX v12.0 Window (%.0fx%.0f)", ImGui::GetWindowSize().x, ImGui::GetWindowSize().y);
	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	ImGui::Text("By: Liam Blake (C) 2020 All Rights Reserved.");

	ImGui::PopFont();
	ImGui::End();

	// Offset to the current render target based on what buffer we are on.
	CD3DX12_CPU_DESCRIPTOR_HANDLE currentRTVHandle(m_d3dRTVDescriptorHeap->GetCPUDescriptorHandleForHeapStart());
	currentRTVHandle.Offset(1, m_iBufferIndex * m_device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV));

	// Indicate that the back buffer will be used as a render target (according to Hooman's slides)
	m_d3dCommandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(
		m_renderTargets[m_iBufferIndex].Get(), D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET));

	m_d3dCommandList->OMSetRenderTargets(1, &currentRTVHandle, true, &m_dsvHeap->GetCPUDescriptorHandleForHeapStart());

	m_d3dCommandList->ClearRenderTargetView(currentRTVHandle, clear_color, 0, nullptr);
	m_d3dCommandList->ClearDepthStencilView(m_dsvHeap->GetCPUDescriptorHandleForHeapStart(),
		D3D12_CLEAR_FLAG_DEPTH | D3D12_CLEAR_FLAG_STENCIL, 1.0f, 0, 0, nullptr);

	m_d3dCommandList->SetGraphicsRootSignature(m_rootSignature.Get());
	m_d3dCommandList->RSSetViewports(1, &m_viewPort);
	m_d3dCommandList->RSSetScissorRects(1, &m_scissorsRect);

	
	m_d3dCommandList->SetPipelineState(m_pipelineState[Pipeline::Opaque].Get());

	
	m_d3dCommandList->SetDescriptorHeaps(1, m_d3dSRVDescriptorHeap.GetAddressOf());

	for (std::uint16_t i = 0; i < (std::uint16_t)m_meshes[Pipeline::Opaque].size(); i++)
	{
		auto mesh = m_meshes[Pipeline::Opaque];

		m_constantBuffer->Model = mesh[i]->GetComponent<TransformComponent>().GetModelMatrix();
		m_constantBuffer->DiffuseAlbedo = mesh[i]->GetComponent<MaterialComponent>().DiffuseAlbedo;
		m_constantBuffer->FresnelFactor = mesh[i]->GetComponent<MaterialComponent>().FresnelFactor;
		this->UpdateConstants();

		void* data;
		m_cbvResources[Pipeline::Opaque][i]->Map(0, nullptr, reinterpret_cast<void**>(&data));
		CopyMemory(data, m_constantBuffer.get(), sizeof(ConstantBuffer));
		m_cbvResources[Pipeline::Opaque][i]->Unmap(0, nullptr);

		m_d3dCommandList->SetGraphicsRootConstantBufferView(0, m_cbvResources[Pipeline::Opaque][i]->GetGPUVirtualAddress());
		m_d3dCommandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		m_d3dCommandList->SetGraphicsRootDescriptorTable(1, mesh[i]->GetComponent<MaterialComponent>().texture.hGPUHandle);
	
		m_d3dCommandList->IASetVertexBuffers(0, 1, &mesh[i]->GetComponent<VertexBufferComponent<Vertex>>().Get());
		m_d3dCommandList->IASetIndexBuffer(&mesh[i]->GetComponent<IndexBufferComponent<std::uint16_t>>().Get());

		m_d3dCommandList->DrawIndexedInstanced(mesh[i]->IndexCount, 1, 0, 0, 0);
	}
	m_d3dCommandList->SetPipelineState(m_pipelineState[Pipeline::StencilMirror].Get());
	m_d3dCommandList->OMSetStencilRef(1);

	for (std::uint16_t i = 0; i < (std::uint16_t)m_meshes[Pipeline::StencilMirror].size(); i++)
	{
		auto mesh = m_meshes[Pipeline::StencilMirror];

		m_constantBuffer->Model = mesh[i]->GetComponent<TransformComponent>().GetModelMatrix();
		m_constantBuffer->DiffuseAlbedo = mesh[i]->GetComponent<MaterialComponent>().DiffuseAlbedo;
		m_constantBuffer->FresnelFactor = mesh[i]->GetComponent<MaterialComponent>().FresnelFactor;
		this->UpdateConstants();

		void* data;
		m_cbvResources[Pipeline::StencilMirror][i]->Map(0, nullptr, reinterpret_cast<void**>(&data));
		CopyMemory(data, m_constantBuffer.get(), sizeof(ConstantBuffer));
		m_cbvResources[Pipeline::StencilMirror][i]->Unmap(0, nullptr);

		m_d3dCommandList->SetGraphicsRootConstantBufferView(0, m_cbvResources[Pipeline::StencilMirror][i]->GetGPUVirtualAddress());
		m_d3dCommandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		m_d3dCommandList->SetGraphicsRootDescriptorTable(1, mesh[i]->GetComponent<MaterialComponent>().texture.hGPUHandle);

		m_d3dCommandList->IASetVertexBuffers(0, 1, &mesh[i]->GetComponent<VertexBufferComponent<Vertex>>().Get());
		m_d3dCommandList->IASetIndexBuffer(&mesh[i]->GetComponent<IndexBufferComponent<std::uint16_t>>().Get());

		m_d3dCommandList->DrawIndexedInstanced(mesh[i]->IndexCount, 1, 0, 0, 0);
	}
	m_d3dCommandList->SetPipelineState(m_pipelineState[Pipeline::StencilShadow].Get());
	m_d3dCommandList->OMSetStencilRef(1);

	for (std::uint16_t i = 0; i < (std::uint16_t)m_meshes[Pipeline::StencilShadow].size(); i++)
	{
		if (!m_bShadowsEnabled)
			break;

		auto mesh = m_meshes[Pipeline::StencilShadow];

		XMVECTOR planeReflect = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
		XMMATRIX R = XMMatrixReflect(planeReflect);
		XMVECTOR lightDir = XMVector3Normalize(XMLoadFloat3(&m_constantBuffer->pLight[0].Direction));
		XMVECTOR refLightDir = XMVector3TransformNormal(lightDir, R);
		XMVECTOR toMainLight = -XMVector3Normalize(XMLoadFloat3(&m_constantBuffer->pLight[0].Direction));
		XMMATRIX S = XMMatrixShadow(planeReflect, toMainLight);

		m_constantBuffer->Model = mesh[i]->GetComponent<TransformComponent>().GetModelMatrix() * S;
		m_constantBuffer->DiffuseAlbedo = { 0.0f, 0.0f, 0.0f, 1.0f };
		m_constantBuffer->FresnelFactor = mesh[i]->GetComponent<MaterialComponent>().FresnelFactor;
		this->UpdateConstants();

		void* data;
		m_cbvResources[Pipeline::StencilShadow][i]->Map(0, nullptr, reinterpret_cast<void**>(&data));
		CopyMemory(data, m_constantBuffer.get(), sizeof(ConstantBuffer));
		m_cbvResources[Pipeline::StencilShadow][i]->Unmap(0, nullptr);

		m_d3dCommandList->SetGraphicsRootConstantBufferView(0, m_cbvResources[Pipeline::StencilShadow][i]->GetGPUVirtualAddress());
		m_d3dCommandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		m_d3dCommandList->SetGraphicsRootDescriptorTable(1, mesh[i]->GetComponent<MaterialComponent>().texture.hGPUHandle);

		m_d3dCommandList->IASetVertexBuffers(0, 1, &mesh[i]->GetComponent<VertexBufferComponent<Vertex>>().Get());
		m_d3dCommandList->IASetIndexBuffer(&mesh[i]->GetComponent<IndexBufferComponent<std::uint16_t>>().Get());

		m_d3dCommandList->DrawIndexedInstanced(mesh[i]->IndexCount, 1, 0, 0, 0);
	}

	m_d3dCommandList->SetPipelineState(m_pipelineState[Pipeline::StencilReflection].Get());
	m_d3dCommandList->OMSetStencilRef(1);

	for (std::uint16_t i = 0; i < (std::uint16_t)m_meshes[Pipeline::StencilReflection].size(); i++)
	{
		if (!m_bReflectionsEnabled)
			break;

		auto mesh = m_meshes[Pipeline::StencilReflection];

		XMVECTOR planeReflect = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
		XMMATRIX R = XMMatrixReflect(planeReflect);

		m_constantBuffer->Model = mesh[i]->GetComponent<TransformComponent>().GetModelMatrix() * R;
		m_constantBuffer->DiffuseAlbedo = mesh[i]->GetComponent<MaterialComponent>().DiffuseAlbedo;
		m_constantBuffer->FresnelFactor = mesh[i]->GetComponent<MaterialComponent>().FresnelFactor;
		this->UpdateConstants();

		void* data;
		m_cbvResources[Pipeline::StencilReflection][i]->Map(0, nullptr, reinterpret_cast<void**>(&data));
		CopyMemory(data, m_constantBuffer.get(), sizeof(ConstantBuffer));
		m_cbvResources[Pipeline::StencilReflection][i]->Unmap(0, nullptr);

		m_d3dCommandList->SetGraphicsRootConstantBufferView(0, m_cbvResources[Pipeline::StencilReflection][i]->GetGPUVirtualAddress());
		m_d3dCommandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		m_d3dCommandList->SetGraphicsRootDescriptorTable(1, mesh[i]->GetComponent<MaterialComponent>().texture.hGPUHandle);

		m_d3dCommandList->IASetVertexBuffers(0, 1, &mesh[i]->GetComponent<VertexBufferComponent<Vertex>>().Get());
		m_d3dCommandList->IASetIndexBuffer(&mesh[i]->GetComponent<IndexBufferComponent<std::uint16_t>>().Get());

		m_d3dCommandList->DrawIndexedInstanced(mesh[i]->IndexCount, 1, 0, 0, 0);
	}
	m_d3dCommandList->SetPipelineState(m_pipelineState[Pipeline::Transparent].Get());

	for (std::uint16_t i = 0; i < (std::uint16_t)m_meshes[Pipeline::Transparent].size(); i++)
	{
		auto mesh = m_meshes[Pipeline::Transparent];
		XMVECTOR s, r, t;
		XMMATRIX m = mesh[i]->GetComponent<TransformComponent>().GetModelMatrix();
		XMFLOAT3 translate;
		XMMatrixDecompose(&s, &r, &t, m);
		XMStoreFloat3(&translate, t);
		// Draw the mirror slightly below to avoid flickering from the shadows and mirrors being
		// directly overlapped. Comment this line out to see that effect:
		translate.y += 0.001f;	
		
		m_constantBuffer->Model = XMMatrixTranslation(translate.x, translate.y, translate.z);
		m_constantBuffer->DiffuseAlbedo = mesh[i]->GetComponent<MaterialComponent>().DiffuseAlbedo;
		m_constantBuffer->FresnelFactor = mesh[i]->GetComponent<MaterialComponent>().FresnelFactor;
		this->UpdateConstants();

		void* data;
		m_cbvResources[Pipeline::Transparent][i]->Map(0, nullptr, reinterpret_cast<void**>(&data));
		CopyMemory(data, m_constantBuffer.get(), sizeof(ConstantBuffer));
		m_cbvResources[Pipeline::Transparent][i]->Unmap(0, nullptr);

		m_d3dCommandList->SetGraphicsRootConstantBufferView(0, m_cbvResources[Pipeline::Transparent][i]->GetGPUVirtualAddress());
		m_d3dCommandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		m_d3dCommandList->SetGraphicsRootDescriptorTable(1, mesh[i]->GetComponent<MaterialComponent>().texture.hGPUHandle);

		m_d3dCommandList->IASetVertexBuffers(0, 1, &mesh[i]->GetComponent<VertexBufferComponent<Vertex>>().Get());
		m_d3dCommandList->IASetIndexBuffer(&mesh[i]->GetComponent<IndexBufferComponent<std::uint16_t>>().Get());

		m_d3dCommandList->DrawIndexedInstanced(mesh[i]->IndexCount, 1, 0, 0, 0);
	}

	m_d3dCommandList->SetDescriptorHeaps(1, m_imguiDescriptorHeap.GetAddressOf());

	ImGui::Render();
	ImGui_ImplDX12_RenderDrawData(ImGui::GetDrawData(), m_d3dCommandList.Get());


	// Indicate that the back buffer will be used to present (according to Hooman's slides)
	m_d3dCommandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(
		m_renderTargets[m_iBufferIndex].Get(), D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT));

	m_d3dCommandList->Close();

	ID3D12CommandList* cmdLists[] = { m_d3dCommandList.Get() };
	m_d3dCommandQueue->ExecuteCommandLists(_countof(cmdLists), cmdLists);
}

void Engine::SwapBuffers(GameTimer* gameTimer) const
{
	ThrowIfFailed(m_dxgiSwapChain->Present(m_bVSyncEnabled, 0));
}

void Engine::Clean(GameTimer* gameTimer)
{
	this->SyncPreviousFrame();
	gameTimer->Reset();
}

void Engine::UpdateConstants()
{
	Camera::UpdateEyePosition();

	m_constantBuffer->EyeWorldSpace = { Camera::Eye.x, Camera::Eye.y, Camera::Eye.z };

	m_constantBuffer->pLight[0].Position = { 0.0f, 0.0f, 0.0f };
	m_constantBuffer->pLight[0].Strength = { light_color[0], light_color[1], light_color[2] };
	m_constantBuffer->pLight[0].FallOffStart = 0.1f;
	m_constantBuffer->pLight[0].FallOffEnd = 15.0f;
	m_constantBuffer->pLight[0].Direction = { cos(XMConvertToRadians(0.25f*m_iCurrentFence)), -1.0f, sin(XMConvertToRadians(0.25f*m_iCurrentFence)) };
	m_constantBuffer->pLight[0].SpecularStrength = 1.0f;

	// Left hand coordinate space
	m_constantBuffer->View = XMMatrixLookAtLH
	(
		XMLoadFloat4(&Camera::Eye),
		XMLoadFloat4(&Camera::Focus),
		XMLoadFloat4(&Camera::Up)
	);
	m_constantBuffer->Projection = XMMatrixPerspectiveFovLH(XMConvertToRadians(45.0f), 4.0f / 3.0f, 0.1f, 300.0f);

	m_constantBuffer->World = XMMatrixTranspose
	(
		m_constantBuffer->Model *
		m_constantBuffer->View *
		m_constantBuffer->Projection
	);
	m_constantBuffer->Model = XMMatrixTranspose(m_constantBuffer->Model);
}

void Engine::BuildDescriptorHeaps()
{
	// Render Target View
	D3D12_DESCRIPTOR_HEAP_DESC rtvDescHeapDesc;
	ZeroMemory(&rtvDescHeapDesc, sizeof(D3D12_DESCRIPTOR_HEAP_DESC));

	rtvDescHeapDesc.NumDescriptors = m_iNumBuffers;
	rtvDescHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;

	ThrowIfFailed(m_device->CreateDescriptorHeap(&rtvDescHeapDesc, IID_PPV_ARGS(m_d3dRTVDescriptorHeap.GetAddressOf())));

	// Depth Stencil View
	D3D12_DESCRIPTOR_HEAP_DESC depthStencilHeap = {};
	depthStencilHeap.NumDescriptors = 1;
	depthStencilHeap.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;

	ThrowIfFailed(m_device->CreateDescriptorHeap(&depthStencilHeap, IID_PPV_ARGS(&m_dsvHeap)));

	// Shader Resource View (textures)
	D3D12_DESCRIPTOR_HEAP_DESC srvDescHeap;
	ZeroMemory(&srvDescHeap, sizeof(D3D12_DESCRIPTOR_HEAP_DESC));

	srvDescHeap.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	srvDescHeap.NumDescriptors = 3;
	srvDescHeap.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;	// Why all 3? We can store all of these (<-) in a heap and upload that to the GPU

	ThrowIfFailed(m_device->CreateDescriptorHeap(&srvDescHeap, IID_PPV_ARGS(m_d3dSRVDescriptorHeap.GetAddressOf())));

	D3D12_DESCRIPTOR_HEAP_DESC imgui_desc_heap_desc = {};
	imgui_desc_heap_desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	imgui_desc_heap_desc.NumDescriptors = 1;
	imgui_desc_heap_desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;

	ThrowIfFailed(m_device->CreateDescriptorHeap(&imgui_desc_heap_desc, IID_PPV_ARGS(m_imguiDescriptorHeap.GetAddressOf())));

}

void Engine::BuildConstantBufferViews()
{
	m_constantBuffer = std::make_unique<ConstantBuffer>();

	const UINT cBufferSize = sizeof(ConstantBuffer);
	UINT AlignedBufferSize = 0U;
		
	// Liam's Note:
	// We have to determine an allligned size for our constant buffer where our constant buffer is a multiple of 256 bytes.
	// Remember that ALL constant buffers have to be a multiple of 256 bytes. No more AND No less!!!
	// std::ceil rounds up to the next whole integer since we can't have slightly less memory allocated than what is needed in our CB.

	for (UINT i = 0; i < (UINT)std::ceil((float)cBufferSize / 256U); i++)
		if (cBufferSize > 256U * i)
			AlignedBufferSize += 256U;
	
	// Since the GPU cannot read the same data while its changing multiple times per frame,
	// we need a way to allow the GPU to point to different memory locations which will
	// let us modify our data as many times as we want. For each time we want to change the data,
	// we need a new resource. There for, we need a resource for each object!!!

	// An alternate way of doing this would be to store these in a descriptor heap. Although since we can only use one 
	// descriptor table at a time - we would have to store our CBV and SRV handles in the same heap.
	
	// This would be really messy and tedious!! ^^^
	for (int j = 0; j < Pipeline::Count; j++)
	{
		for (std::uint16_t i = 0; i < (std::uint16_t)m_cbvResources[j].size(); i++)
		{
			ThrowIfFailed(m_device->CreateCommittedResource(
				&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
				D3D12_HEAP_FLAG_NONE,
				&CD3DX12_RESOURCE_DESC::Buffer(AlignedBufferSize),
				D3D12_RESOURCE_STATE_GENERIC_READ,
				nullptr,
				IID_PPV_ARGS(m_cbvResources[j][i].GetAddressOf())
			));
		}
	}
}

void Engine::BuildShaderResourceViews()
{
	m_textureMap["glass"] = std::make_unique<Texture>();
	m_textureMap["stone"] = std::make_unique<Texture>();
	m_textureMap["roughstone"] = std::make_unique<Texture>();

	m_d3dCommandList->Reset(m_d3dCommandAllocator.Get(), nullptr);

	ThrowIfFailed(DirectX::CreateDDSTextureFromFile12(
		m_device.Get(),
		m_d3dCommandList.Get(),
		L"assets/glass.dds",
		m_textureMap["glass"]->m_texResource,
		m_textureMap["glass"]->m_texHeap
	));
	ThrowIfFailed(DirectX::CreateDDSTextureFromFile12(
		m_device.Get(),
		m_d3dCommandList.Get(),
		L"assets/stone.dds",
		m_textureMap["stone"]->m_texResource,
		m_textureMap["stone"]->m_texHeap
	));
	ThrowIfFailed(DirectX::CreateDDSTextureFromFile12(
		m_device.Get(),
		m_d3dCommandList.Get(),
		L"assets/roughstone.dds",
		m_textureMap["roughstone"]->m_texResource,
		m_textureMap["roughstone"]->m_texHeap
	));

	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc;
	ZeroMemory(&srvDesc, sizeof(D3D12_SHADER_RESOURCE_VIEW_DESC));

	srvDesc.Format = m_textureMap["glass"]->m_texResource->GetDesc().Format;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.Texture2D.MipLevels = m_textureMap["glass"]->m_texResource->GetDesc().MipLevels;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;

	// Point to the start of the descriptor heap
	// We need GPU access to the texture location in the heap, so we need to store that address in our texture class
	CD3DX12_GPU_DESCRIPTOR_HANDLE hGPUDescriptor(m_d3dSRVDescriptorHeap->GetGPUDescriptorHandleForHeapStart());
	CD3DX12_CPU_DESCRIPTOR_HANDLE hCPUDescriptor(m_d3dSRVDescriptorHeap->GetCPUDescriptorHandleForHeapStart());

	// Glass texture
	{
		srvDesc.Format = m_textureMap["glass"]->m_texResource->GetDesc().Format;
		srvDesc.Texture2D.MipLevels = m_textureMap["glass"]->m_texResource->GetDesc().MipLevels;

		m_textureMap["glass"]->hGPUHandle = hGPUDescriptor;
		m_device->CreateShaderResourceView(m_textureMap["glass"]->m_texResource.Get(), &srvDesc, hCPUDescriptor);
	}

	// Point to the next descriptor in the heap
	hCPUDescriptor.Offset(1, m_device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV));
	hGPUDescriptor.Offset(1, m_device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV));

	// Char Coal tile texture
	{
		srvDesc.Format = m_textureMap["roughstone"]->m_texResource->GetDesc().Format;
		srvDesc.Texture2D.MipLevels = m_textureMap["roughstone"]->m_texResource->GetDesc().MipLevels;

		m_textureMap["roughstone"]->hGPUHandle = hGPUDescriptor;
		m_device->CreateShaderResourceView(m_textureMap["roughstone"]->m_texResource.Get(), &srvDesc, hCPUDescriptor);
	}

	// Point to the next descriptor in the heap
	hCPUDescriptor.Offset(1, m_device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV));
	hGPUDescriptor.Offset(1, m_device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV));

	// Stone texture
	{
		srvDesc.Format = m_textureMap["stone"]->m_texResource->GetDesc().Format;
		srvDesc.Texture2D.MipLevels = m_textureMap["stone"]->m_texResource->GetDesc().MipLevels;

		m_textureMap["stone"]->hGPUHandle = hGPUDescriptor;
		m_device->CreateShaderResourceView(m_textureMap["stone"]->m_texResource.Get(), &srvDesc, hCPUDescriptor);
	}

}

void Engine::BuildRootSignature()
{
	/*
		Liam's Notes:

		Slot Parameters in our Root Signature:
		--------------------------------------

		1. We have a Constant Buffer declared at base register 0 (look in VS/PS.hlsl)
		2. We have a Shader Resource View declared at base register 0 (look in PS.hlsl)
		3. We have a Static Sampler State declared at base register 0 (look in PS.hlsl)
	*/
	// Note: The sampler state is not declared in a root parameter but rather the actual 
	// root signature itself

	CD3DX12_STATIC_SAMPLER_DESC samplerState;
	ZeroMemory(&samplerState, sizeof(CD3DX12_STATIC_SAMPLER_DESC));

	samplerState.Init
	(
		0, // shaderRegister
		D3D12_FILTER_MIN_MAG_MIP_LINEAR, // filter
		D3D12_TEXTURE_ADDRESS_MODE_WRAP,  // addressU
		D3D12_TEXTURE_ADDRESS_MODE_WRAP,  // addressV
		D3D12_TEXTURE_ADDRESS_MODE_WRAP
	);

	CD3DX12_DESCRIPTOR_RANGE descRange;
	descRange.Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0);

	CD3DX12_ROOT_PARAMETER slotParameters[2];

	// Base register 0 at "cbuffer ConstantBuffer"
	slotParameters[0].InitAsConstantBufferView(0);

	// Shader resource view (texture) at base register 0
	slotParameters[1].InitAsDescriptorTable(1, &descRange);

	CD3DX12_ROOT_SIGNATURE_DESC rootSignatureDesc;
	ZeroMemory(&rootSignatureDesc, sizeof(CD3DX12_ROOT_SIGNATURE_DESC));

	rootSignatureDesc.Init(_countof(slotParameters), slotParameters, 1, &samplerState, D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

	ID3DBlob* signatureBlob;
	ThrowIfFailed(D3D12SerializeRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1, &signatureBlob, nullptr));
	
	ThrowIfFailed(m_device->CreateRootSignature(
		0,
		signatureBlob->GetBufferPointer(),
		signatureBlob->GetBufferSize(),
		IID_PPV_ARGS(m_rootSignature.GetAddressOf()
		)));
}

void Engine::AssemblePipeline(const LPCWSTR vsPath, const LPCWSTR dafaultpsPath, const LPCWSTR shadowpsPath)
{
	ID3DBlob* vs, *default_ps, *shadow_ps;

	// Look at -> (A) (B) (C)
	// These haven't changed since DirectX 11, but we need to attach them to the PSO instead of directly from the 
	// device context.
	
	// (A)
	ID3DBlob* errorQueue;
	ZeroMemory(&errorQueue, sizeof(ID3DBlob));

	ThrowIfFailed(D3DCompileFromFile(vsPath, 0, 0, "VSMain", "vs_4_0", 0, 0, &vs, &errorQueue));
	ThrowIfFailed(D3DCompileFromFile(dafaultpsPath, 0, 0, "PSMain", "ps_4_0", 0, 0, &default_ps, 0));
	ThrowIfFailed(D3DCompileFromFile(shadowpsPath, 0, 0, "PSMain", "ps_4_0", 0, 0, &shadow_ps, 0));

	// (B)
	D3D12_INPUT_ELEMENT_DESC inputLayout[3] =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
		{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
		{"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 20, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0}
	};

	// (C)
	D3D12_RASTERIZER_DESC rasDesc;
	ZeroMemory(&rasDesc, sizeof(D3D12_RASTERIZER_DESC));
	
	rasDesc.CullMode = D3D12_CULL_MODE_NONE;
	rasDesc.FillMode = D3D12_FILL_MODE_SOLID;
	rasDesc.AntialiasedLineEnable = true;
	rasDesc.MultisampleEnable = false;
	

	D3D12_GRAPHICS_PIPELINE_STATE_DESC opaquePSODesc = { 0 };
	opaquePSODesc.VS = CD3DX12_SHADER_BYTECODE(vs);
	opaquePSODesc.PS = CD3DX12_SHADER_BYTECODE(default_ps);
	opaquePSODesc.pRootSignature = m_rootSignature.Get();
	opaquePSODesc.SampleDesc.Count = 1;
	opaquePSODesc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
	opaquePSODesc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
	opaquePSODesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
	opaquePSODesc.SampleMask = UINT_MAX;

	//step4:
	opaquePSODesc.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);
	opaquePSODesc.DSVFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;
	opaquePSODesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	opaquePSODesc.InputLayout = { inputLayout, _countof(inputLayout) };
	opaquePSODesc.NumRenderTargets = 1;

	D3D12_RENDER_TARGET_BLEND_DESC transparencyBlendDesc;
	transparencyBlendDesc.BlendEnable = true;
	transparencyBlendDesc.LogicOpEnable = false;
	transparencyBlendDesc.SrcBlend = D3D12_BLEND_SRC_ALPHA;
	transparencyBlendDesc.DestBlend = D3D12_BLEND_INV_SRC_ALPHA;
	transparencyBlendDesc.BlendOp = D3D12_BLEND_OP_ADD;
	transparencyBlendDesc.SrcBlendAlpha = D3D12_BLEND_ONE;
	transparencyBlendDesc.DestBlendAlpha = D3D12_BLEND_ZERO;
	transparencyBlendDesc.BlendOpAlpha = D3D12_BLEND_OP_ADD;
	transparencyBlendDesc.LogicOp = D3D12_LOGIC_OP_NOOP;
	transparencyBlendDesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;
	//transparencyBlendDesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_BLUE;
	//Direct3D supports rendering to up to eight render targets simultaneously.
	opaquePSODesc.BlendState.RenderTarget[0] = transparencyBlendDesc;

	ThrowIfFailed(m_device->CreateGraphicsPipelineState(&opaquePSODesc, IID_PPV_ARGS(m_pipelineState[Pipeline::Opaque].GetAddressOf())));

	// ----------
	//step5:
	D3D12_GRAPHICS_PIPELINE_STATE_DESC transparencyPSODesc = opaquePSODesc;

	transparencyBlendDesc.BlendEnable = true;
	transparencyBlendDesc.LogicOpEnable = false;
	transparencyBlendDesc.SrcBlend = D3D12_BLEND_SRC_ALPHA;
	transparencyBlendDesc.DestBlend = D3D12_BLEND_INV_SRC_ALPHA;
	transparencyBlendDesc.BlendOp = D3D12_BLEND_OP_ADD;
	transparencyBlendDesc.SrcBlendAlpha = D3D12_BLEND_ONE;
	transparencyBlendDesc.DestBlendAlpha = D3D12_BLEND_ZERO;
	transparencyBlendDesc.BlendOpAlpha = D3D12_BLEND_OP_ADD;
	transparencyBlendDesc.LogicOp = D3D12_LOGIC_OP_NOOP;
	transparencyBlendDesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;
	//transparencyBlendDesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_BLUE;
	//Direct3D supports rendering to up to eight render targets simultaneously.
	transparencyPSODesc.BlendState.RenderTarget[0] = transparencyBlendDesc;

	ThrowIfFailed(m_device->CreateGraphicsPipelineState(&transparencyPSODesc, IID_PPV_ARGS(m_pipelineState[Pipeline::Transparent].GetAddressOf())));

	//step6:
	CD3DX12_BLEND_DESC mirrorBlendState(D3D12_DEFAULT);
	//// Turn off render target writes.
	mirrorBlendState.RenderTarget[0].RenderTargetWriteMask = 0;

	D3D12_DEPTH_STENCIL_DESC mirrorDSS;
	mirrorDSS.DepthEnable = true;
	mirrorDSS.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ZERO; //which portion of the depth/stencil buffer that depth data can be written to.
	mirrorDSS.DepthFunc = D3D12_COMPARISON_FUNC_LESS; //the function the depth test should use
	mirrorDSS.StencilEnable = true;
	mirrorDSS.StencilReadMask = 0xff; //This is the portion of the depth/stencil buffer that the stencil test can read from
	mirrorDSS.StencilWriteMask = 0xff; //This is the portion of the depth/stencil buffer that the stencil test can write to

	// This is a D3D12_DEPTH_STENCILOP_DESC structure which describes what the depth or stencil test is supposed to do with pixels whose surface normals are facing camera (like the frontside of a triangle)
	mirrorDSS.FrontFace.StencilFailOp = D3D12_STENCIL_OP_KEEP; //when a pixel fragment fails the stencil test.
	mirrorDSS.FrontFace.StencilDepthFailOp = D3D12_STENCIL_OP_KEEP; // when the stencil test passes but the depth test fails
	mirrorDSS.FrontFace.StencilPassOp = D3D12_STENCIL_OP_REPLACE; // when stencil and depth tests both pass
	mirrorDSS.FrontFace.StencilFunc = D3D12_COMPARISON_FUNC_ALWAYS; //the function the stencil test should use

	// We are not rendering backfacing polygons, so these settings do not matter.
	mirrorDSS.BackFace.StencilFailOp = D3D12_STENCIL_OP_KEEP;
	mirrorDSS.BackFace.StencilDepthFailOp = D3D12_STENCIL_OP_KEEP;
	mirrorDSS.BackFace.StencilPassOp = D3D12_STENCIL_OP_REPLACE;
	mirrorDSS.BackFace.StencilFunc = D3D12_COMPARISON_FUNC_ALWAYS;

	D3D12_GRAPHICS_PIPELINE_STATE_DESC markMirrorsPsoDesc = opaquePSODesc;
	markMirrorsPsoDesc.BlendState = mirrorBlendState;
	markMirrorsPsoDesc.DepthStencilState = mirrorDSS;

	ThrowIfFailed(m_device->CreateGraphicsPipelineState(&markMirrorsPsoDesc, IID_PPV_ARGS(m_pipelineState[Pipeline::StencilMirror].GetAddressOf())));

	//step7:
	D3D12_DEPTH_STENCIL_DESC reflectionsDSS;
	//By using the depth/stencil buffer, we can block the reflected from being rendered on the wall
	//what happens when you set reflectionsDSS.DepthEnable = false; and reflectionsDSS.StencilEnable = false;
	//the mirror would not occlude the reflection
	reflectionsDSS.DepthEnable = true;
	reflectionsDSS.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;
	reflectionsDSS.DepthFunc = D3D12_COMPARISON_FUNC_LESS;
	//By using the stencil buffer, we can block  the reflected skull from being rendered unless it is being drawn in the mirror.
	//what happens when you set reflectionsDSS.StencilEnable = false;
	reflectionsDSS.StencilEnable = true;
	reflectionsDSS.StencilReadMask = 0xff;
	reflectionsDSS.StencilWriteMask = 0xff;

	reflectionsDSS.FrontFace.StencilFailOp = D3D12_STENCIL_OP_KEEP;
	reflectionsDSS.FrontFace.StencilDepthFailOp = D3D12_STENCIL_OP_KEEP;
	reflectionsDSS.FrontFace.StencilPassOp = D3D12_STENCIL_OP_KEEP;
	//we set the stencil test to only succeed if the value in the stencil buffer equals 1 (stencil ref)
	reflectionsDSS.FrontFace.StencilFunc = D3D12_COMPARISON_FUNC_EQUAL;

	// We are not rendering backfacing polygons, so these settings do not matter.
	reflectionsDSS.BackFace.StencilFailOp = D3D12_STENCIL_OP_KEEP;
	reflectionsDSS.BackFace.StencilDepthFailOp = D3D12_STENCIL_OP_KEEP;
	reflectionsDSS.BackFace.StencilPassOp = D3D12_STENCIL_OP_KEEP;
	reflectionsDSS.BackFace.StencilFunc = D3D12_COMPARISON_FUNC_EQUAL;

	D3D12_GRAPHICS_PIPELINE_STATE_DESC drawReflectionsPsoDesc = opaquePSODesc;
	drawReflectionsPsoDesc.DepthStencilState = reflectionsDSS;
	drawReflectionsPsoDesc.RasterizerState.CullMode = D3D12_CULL_MODE_BACK;
	drawReflectionsPsoDesc.RasterizerState.FrontCounterClockwise = true;
	ThrowIfFailed(m_device->CreateGraphicsPipelineState(&drawReflectionsPsoDesc, IID_PPV_ARGS(m_pipelineState[Pipeline::StencilReflection].GetAddressOf())))

		//step8:
	D3D12_GRAPHICS_PIPELINE_STATE_DESC shadowPSODesc = {};

	D3D12_DEPTH_STENCIL_DESC shadowDSS;
	shadowDSS.DepthEnable = true;
	shadowDSS.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;
	shadowDSS.DepthFunc = D3D12_COMPARISON_FUNC_LESS;
	shadowDSS.StencilEnable = true;
	shadowDSS.StencilReadMask = 0xff;
	shadowDSS.StencilWriteMask = 0xff;

	shadowDSS.FrontFace.StencilFailOp = D3D12_STENCIL_OP_KEEP;
	shadowDSS.FrontFace.StencilDepthFailOp = D3D12_STENCIL_OP_KEEP;
	shadowDSS.FrontFace.StencilPassOp = D3D12_STENCIL_OP_INCR;
	shadowDSS.FrontFace.StencilFunc = D3D12_COMPARISON_FUNC_EQUAL;

	// We are not rendering backfacing polygons, so these settings do not matter.
	shadowDSS.BackFace.StencilFailOp = D3D12_STENCIL_OP_KEEP;
	shadowDSS.BackFace.StencilDepthFailOp = D3D12_STENCIL_OP_KEEP;
	shadowDSS.BackFace.StencilPassOp = D3D12_STENCIL_OP_INCR;
	shadowDSS.BackFace.StencilFunc = D3D12_COMPARISON_FUNC_EQUAL;

	shadowPSODesc = transparencyPSODesc;
	shadowPSODesc.PS = CD3DX12_SHADER_BYTECODE(shadow_ps);

	shadowPSODesc.DepthStencilState = shadowDSS;

	ThrowIfFailed(m_device->CreateGraphicsPipelineState(&shadowPSODesc, IID_PPV_ARGS(m_pipelineState[Pipeline::StencilShadow].GetAddressOf())));
}