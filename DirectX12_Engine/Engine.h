#pragma once
#include "D3DApp.h"
#include "Vertex.h"
#include "D3DCommon.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "MeshGeometry.h"
#include "GeometryGenerator.h"
#include "Transform.h"
#include "DDSTextureLoader.h"
#include <cmath>
#include <vector>
#include "ImGui/imgui_impl_dx12.h"
#include "ImGui/imgui_impl_win32.h"
#include <cassert>
#include "GameTimer.h"
#include <unordered_map>
/* Engine takes care of our game */
/* D3DApp takes care of setting up D3D */

class Win32App;
typedef class Engine : public D3DApp
{
public:
	Engine(const Engine& rhs) = delete;
	void operator=(const Engine& rhs) = delete;

	Engine();
	~Engine();
	static Engine* GetApp();
	
	// Can be overrided in another class if we want:
	virtual void Initialize(GameTimer* gameTimer, const std::shared_ptr<Win32App> window, const LPCWSTR vsPath, const LPCWSTR dafaultpsPath, const LPCWSTR shadowpsPath) override;
	virtual void Update(GameTimer* gameTimer) override;
	virtual void Draw(GameTimer* gameTimer) override;
	virtual void SwapBuffers(GameTimer* gameTimer) const;
	virtual void Clean(GameTimer* gameTimer) override;
	
	void UpdateConstants();
	void BuildDescriptorHeaps();	// Used for RTV
	void BuildConstantBufferViews();
	void BuildShaderResourceViews();
	void BuildRootSignature();
	void AssemblePipeline(const LPCWSTR vsPath, const LPCWSTR dafaultpsPath, const LPCWSTR shadowpsPath);

private:
	std::unique_ptr<ConstantBuffer> m_constantBuffer;
	std::vector<std::shared_ptr<MeshGeometry>> m_meshes[Pipeline::Count];
	
	Microsoft::WRL::ComPtr<ID3D12RootSignature> m_rootSignature;
	
	std::unordered_map<std::string, std::unique_ptr<Texture>> m_textureMap;

	Microsoft::WRL::ComPtr<ID3D12PipelineState> m_pipelineState[Pipeline::Count];

	std::vector<Microsoft::WRL::ComPtr<ID3D12Resource>> m_cbvResources[Pipeline::Count];

	bool m_bShadowsEnabled = true;
	bool m_bReflectionsEnabled = true;
	bool m_bVSyncEnabled = true;
	float clear_color[4] = { 0.678431392f, 0.847058892f, 0.901960850f, 1.000000000f };
	float light_color[3] = { 0.7f, 0.7f, 0.7f };

	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> m_imguiDescriptorHeap;
	CD3DX12_CPU_DESCRIPTOR_HANDLE m_himguiCPUHandle;
	CD3DX12_GPU_DESCRIPTOR_HANDLE m_himguiGPUHandle;

	ImFont* font;

	static Engine* s_pInstance;

} Engine, D3D12Engine, D3D12App;