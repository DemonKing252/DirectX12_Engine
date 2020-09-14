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
	
	void Initialize(const std::shared_ptr<Win32App> window, const LPCWSTR vsPath, const LPCWSTR dafaultpsPath, const LPCWSTR shadowpsPath) override;
	void Update() override;
	void Draw() override;
	void SwapBuffers() const;
	void Clean() override;
	
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
	std::unique_ptr<Texture> m_charCoalTex;
	std::unique_ptr<Texture> m_glassTex;
	std::unique_ptr<Texture> m_stoneTex;
	Microsoft::WRL::ComPtr<ID3D12PipelineState> m_pipelineState[Pipeline::Count];

	std::vector<Microsoft::WRL::ComPtr<ID3D12Resource>> m_cbvResources[Pipeline::Count];

	static Engine* s_pInstance;

} Engine, D3D12Engine, D3D12App;