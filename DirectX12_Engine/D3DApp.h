#pragma once
#include <Windows.h>
#include <d3d12.h>
#include <dxgi1_4.h>
#include <d3dcompiler.h>
#include <memory>
#include <wrl.h>
#include "d3dx12.h"
class Win32App;

#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3dcompiler.lib")
/* Kind of a lazy way of doing it. But not checking hresults is a big no no! */
#define ThrowIfFailed(hr)	\
{							\
	if (!SUCCEEDED(hr))		\
		DebugBreak();		\
}							\

class D3DApp
{
private:
	// Not needed in base class:
	Microsoft::WRL::ComPtr<IDXGIFactory2> m_dxgiFactory2;
	Microsoft::WRL::ComPtr<ID3D12Debug> m_debugController;
	Microsoft::WRL::ComPtr<ID3D12CommandAllocator> m_commandAllocator;
	Microsoft::WRL::ComPtr<ID3D12Fence> m_fence;

	HWND m_win32Wnd;
protected:
	const int m_iNumBuffers = 3;
	UINT m_iBufferIndex = m_iNumBuffers - 1;

	UINT64 m_iCurrentFence;
	HANDLE m_fenceEvent;

	CD3DX12_RECT m_scissorsRect;
	D3D12_VIEWPORT m_viewPort;

	Microsoft::WRL::ComPtr<ID3D12Device> m_device;
	Microsoft::WRL::ComPtr<IDXGISwapChain1> m_dxgiSwapChain1;
	Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> m_commandList;
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> m_rtvDescriptorHeap;
	Microsoft::WRL::ComPtr<ID3D12CommandQueue> m_commandQueue;
	Microsoft::WRL::ComPtr<ID3D12PipelineState> m_pipelineState;
public:
	D3DApp();
	~D3DApp();

	// Delete the copy constructor and operator assignment function. We do not want more than one instance.
	D3DApp(const D3DApp& rhs) = delete;
	void operator=(const D3DApp& rhs) = delete;

	virtual void Initialize(const std::shared_ptr<Win32App> window);
	virtual void Update();
	virtual void Draw();
	
	void NewFrame();			// Reset the command list and command allocator for a new frame
	void BuildDeviceAndSwapChain(const std::shared_ptr<Win32App> window);
	void BuildCommandObjects();		
	void BuildDescriptorHeaps();	// Used for RTV
	void BuildRenderTargetViews();	// For each frame
	void WaitForPreviousFrame();		// Sync the CPU and GPU
};

