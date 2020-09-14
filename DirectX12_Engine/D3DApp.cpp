#include "D3DApp.h"
#include "Win32App.h"
#include "D3DCommon.h"
D3DApp::D3DApp()
{
}

D3DApp::~D3DApp()
{
}

void D3DApp::Initialize(const std::shared_ptr<Win32App> window, const LPCWSTR vsPath, const LPCWSTR dafaultpsPath, const LPCWSTR shadowpsPath)
{
}

void D3DApp::Update()
{
}

void D3DApp::Draw()
{
}

void D3DApp::Clean()
{
}

bool D3DApp::Ready()
{
	return m_ready;
}

void D3DApp::ResetCommandObjects()
{
	m_commandAllocator->Reset();
	m_commandList->Reset(m_commandAllocator.Get(), nullptr);
}

void D3DApp::OnResize()
{
	if (!this->Ready())
		return;

	/*
	//WaitForPreviousFrame();
	//m_commandAllocator->Reset();
	//m_commandList->Reset(m_commandAllocator.Get(), nullptr);
	
	for (int i = 0; i < m_iNumBuffers; i++)
		m_renderTargets[i].Reset();

	m_depthStencilResource.Reset();

	ThrowIfFailed(m_dxgiSwapChain1->ResizeBuffers(
		m_iNumBuffers, 
		Util::ClientSize.x, Util::ClientSize.y, 
		DXGI_FORMAT_R8G8B8A8_UNORM, 
		DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH
	));
	m_iBufferIndex = 0;

	this->BuildRenderTargetViews();
	this->BuildDepthStencilViews();
	
	//m_viewPort.TopLeftX = 0;
	//m_viewPort.TopLeftY = 0;
	//m_viewPort.Width = Util::ClientSize.x;
	//m_viewPort.Height = Util::ClientSize.y;
	//
	//m_scissorsRect.left = 0;
	//m_scissorsRect.top = 0;
	//m_scissorsRect.right = Util::ClientSize.x;
	//m_scissorsRect.bottom = Util::ClientSize.y;

	*/
}

void D3DApp::BuildDeviceAndSwapChain(const std::shared_ptr<Win32App> window)
{
	ZeroMemory(m_debugController.GetAddressOf(), sizeof(ID3D12Debug));

	ThrowIfFailed(D3D12GetDebugInterface(IID_PPV_ARGS(m_debugController.GetAddressOf())));
	m_debugController->EnableDebugLayer();

	ThrowIfFailed(CreateDXGIFactory(IID_PPV_ARGS(m_dxgiFactory2.GetAddressOf())));

	ZeroMemory(m_device.GetAddressOf(), sizeof(ID3D12Device));

	ThrowIfFailed(D3D12CreateDevice(
		nullptr,
		D3D_FEATURE_LEVEL_12_1,
		IID_PPV_ARGS(m_device.GetAddressOf())
	));

	D3D12_COMMAND_QUEUE_DESC cmdQueueDesc;
	ZeroMemory(&cmdQueueDesc, sizeof(D3D12_COMMAND_QUEUE_DESC));

	cmdQueueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;

	ThrowIfFailed(m_device->CreateCommandQueue(
		&cmdQueueDesc,
		IID_PPV_ARGS(m_commandQueue.GetAddressOf())
	));
	
	DXGI_SWAP_CHAIN_DESC1 scd;
	ZeroMemory(&scd, sizeof(DXGI_SWAP_CHAIN_DESC1));
	
	scd.BufferCount = m_iNumBuffers;
	scd.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	scd.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
	scd.SampleDesc.Count = 1;
	scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	scd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

	ThrowIfFailed(m_dxgiFactory2->CreateSwapChainForHwnd(
		m_commandQueue.Get(),
		window->Get(),
		&scd,
		nullptr,
		nullptr,
		m_dxgiSwapChain.GetAddressOf()
	));

	m_viewPort.TopLeftX = 0;
	m_viewPort.TopLeftY = 0;
	m_viewPort.Width = window->GetDimensions().right;
	m_viewPort.Height = window->GetDimensions().bottom;
	m_viewPort.MinDepth = 0.0f;
	m_viewPort.MaxDepth = 1.0f;

	m_scissorsRect.top = 0;
	m_scissorsRect.left = 0;
	m_scissorsRect.right = window->GetDimensions().right;
	m_scissorsRect.bottom = window->GetDimensions().bottom;
}

void D3DApp::BuildCommandObjects()
{
	ThrowIfFailed(m_device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(m_commandAllocator.GetAddressOf())));

	ThrowIfFailed(m_device->CreateCommandList(
		0,
		D3D12_COMMAND_LIST_TYPE_DIRECT,
		m_commandAllocator.Get(),
		nullptr,
		IID_PPV_ARGS(m_commandList.GetAddressOf())
	));
	// d3d doc states that this should be closed before the main loop
	m_commandList->Close();

	ThrowIfFailed(m_device->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(m_fence.GetAddressOf())));
	m_fenceEvent = CreateEvent(0, 0, 0, 0);
}

void D3DApp::BuildRenderTargetViews()
{
	CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle(m_rtvDescriptorHeap->GetCPUDescriptorHandleForHeapStart());
	const UINT rtvSize = m_device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
	
	for (UINT frame = 0; frame < m_iNumBuffers; frame++)
	{
		m_dxgiSwapChain->GetBuffer(frame, IID_PPV_ARGS(m_renderTargets[frame].GetAddressOf()));
		 
		m_device->CreateRenderTargetView(
			m_renderTargets[frame].Get(),
			nullptr,
			rtvHandle
		);

		rtvHandle.Offset(1, rtvSize);
	}
}

void D3DApp::BuildDepthStencilViews()
{
	// Depth Stencil Resources:

	DXGI_FORMAT mDepthStencilFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;
	// Create the depth/stencil buffer and view.
	D3D12_RESOURCE_DESC depthStencilDesc = {};
	depthStencilDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	depthStencilDesc.Width = Util::ClientSize.x;
	depthStencilDesc.Height = Util::ClientSize.y;
	depthStencilDesc.DepthOrArraySize = 1;
	depthStencilDesc.MipLevels = 1;
	depthStencilDesc.Format = DXGI_FORMAT_R24G8_TYPELESS;
	depthStencilDesc.SampleDesc.Count = 1;
	depthStencilDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;

	D3D12_CLEAR_VALUE optClear;
	optClear.Format = mDepthStencilFormat;
	optClear.DepthStencil.Depth = 1.0f;
	optClear.DepthStencil.Stencil = 0;

	ThrowIfFailed(m_device->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
		D3D12_HEAP_FLAG_NONE,
		&depthStencilDesc,
		D3D12_RESOURCE_STATE_COMMON,
		&optClear,
		IID_PPV_ARGS(&m_depthStencilResource)));

	// Create descriptor to mip level 0 of entire resource using the format of the resource.
	D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc;
	dsvDesc.Flags = D3D12_DSV_FLAG_NONE;
	dsvDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
	dsvDesc.Format = mDepthStencilFormat;
	dsvDesc.Texture2D.MipSlice = 0;
	
	m_device->CreateDepthStencilView(m_depthStencilResource.Get(), &dsvDesc, m_dsvHeap->GetCPUDescriptorHandleForHeapStart());

	m_commandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(m_depthStencilResource.Get(),
		D3D12_RESOURCE_STATE_COMMON, D3D12_RESOURCE_STATE_DEPTH_WRITE));

	m_ready = true;
}

void D3DApp::SyncPreviousFrame()
{
	ThrowIfFailed(m_commandQueue->Signal(m_fence.Get(), m_iCurrentFence));
	// Wait for the GPU to finish marking commands up to this fence point
	if (m_fence->GetCompletedValue() < m_iCurrentFence)
	{
		ThrowIfFailed(m_fence->SetEventOnCompletion(m_iCurrentFence, m_fenceEvent));
		WaitForSingleObject(m_fenceEvent, INFINITE);
	}
	// Increment the fence
	m_iCurrentFence++;
}