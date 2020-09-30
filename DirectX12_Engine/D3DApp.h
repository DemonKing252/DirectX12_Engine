#pragma once
#include <Windows.h>
#include <d3d12.h>
#include <dxgi1_4.h>
#include <d3dcompiler.h>
#include <memory>
#include <wrl.h>
#include "d3dx12.h"
#include <DirectXColors.h>
#include <DirectXMath.h>

#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3dcompiler.lib")

class GameTimer;
/* Kind of a lazy way of doing it. But not checking hresults is a big no no! */
#define ThrowIfFailed(hr)	\
{							\
	if (!SUCCEEDED(hr))		\
		DebugBreak();		\
}							\

class Win32App;
class D3DApp
{
private:
	// Not needed in derived class:
	Microsoft::WRL::ComPtr<IDXGIFactory2> m_dxgiFactory2;
	Microsoft::WRL::ComPtr<ID3D12Debug> m_debugController;
	Microsoft::WRL::ComPtr<ID3D12Fence> m_fence;

	HWND m_win32Wnd;
protected:
	// Too my discovery if a variable is to be used to define an array size in a class, it must be static!
	// ^ This is of course if its allocated on the stack.

	static const UINT m_iNumBuffers = 3;
	UINT m_iBufferIndex = m_iNumBuffers - 1;
	BOOL m_ready = false;
	UINT64 m_iCurrentFence;
	HANDLE m_fenceEvent;

	CD3DX12_RECT m_scissorsRect;
	D3D12_VIEWPORT m_viewPort;

	Microsoft::WRL::ComPtr<ID3D12Device> m_device;
	Microsoft::WRL::ComPtr<IDXGISwapChain1> m_dxgiSwapChain;
	Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> m_d3dCommandList;
	Microsoft::WRL::ComPtr<ID3D12CommandAllocator> m_d3dCommandAllocator;
	Microsoft::WRL::ComPtr<ID3D12Resource> m_renderTargets[m_iNumBuffers];
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> m_d3dRTVDescriptorHeap;
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> m_d3dSRVDescriptorHeap;
	Microsoft::WRL::ComPtr<ID3D12CommandQueue> m_d3dCommandQueue;

	ID3D12DescriptorHeap* m_dsvHeap;
	Microsoft::WRL::ComPtr<ID3D12Resource> m_depthStencilResource;
public:
	D3DApp();
	~D3DApp();

	// Delete the copy constructor and operator assignment function. We do not want more than one instance.
	D3DApp(const D3DApp& rhs) = delete;
	void operator=(const D3DApp& rhs) = delete;

	virtual void Initialize(GameTimer* gameTimer, const std::shared_ptr<Win32App> window, const LPCWSTR vsPath, const LPCWSTR dafaultpsPath, const LPCWSTR shadowpsPath);
	virtual void Update(GameTimer* gameTimer);
	virtual void Draw(GameTimer* gameTimer);
	virtual void Clean(GameTimer* gameTimer);
	
	bool Ready();	// ready for resize commands (ie: swap chain and command objects are set up)
	void ResetCommandObjects();			// Reset the command list and command allocator for a new frame
	void OnResize();
	void BuildDeviceAndSwapChain(const std::shared_ptr<Win32App> window);
	void BuildCommandObjects();		
	void BuildRenderTargetViews();	// For each frame
	void BuildDepthStencilViews();

	std::vector<std::wstring> m_adapters;

	/* Sync the CPU and GPU */
	void SyncPreviousFrame();		
};