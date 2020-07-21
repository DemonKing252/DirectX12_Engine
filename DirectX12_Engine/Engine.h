#pragma once
#include "D3DApp.h"
#include "Vertex.h"
#include "D3DCommon.h"
/* Engine takes care of our game (example: ImGui etc) */
/* D3DApp takes care of setting up D3D */

class Win32App;
typedef class Engine : public D3DApp
{
public:
	Engine(const Engine& rhs) = delete;
	void operator=(Engine& rhs) = delete;

	Engine();
	~Engine();
	static Engine* GetApp();
	
	void Initialize(const std::shared_ptr<Win32App> window, const LPCWSTR vsPath, const LPCWSTR psPath) override;
	void Update() override;
	void Draw() override;
	void SwapBuffers() const;

	void BuildDescriptorHeaps();	// Used for RTV
	void BuildConstantBufferViews();
	void BuildRootSignature();

private:
	std::unique_ptr<ConstantBuffer> m_constantBuffer;

	Microsoft::WRL::ComPtr<ID3D12RootSignature> m_rootSignature;
	Microsoft::WRL::ComPtr<ID3D12Resource> m_cbvResource;
	
	ID3D12Resource* m_vertexBuffer;
	D3D12_VERTEX_BUFFER_VIEW m_vertexBufferView;
	

	static Engine* s_pInstance;

} Engine, D3D12Engine, D3D12App;