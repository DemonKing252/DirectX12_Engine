#pragma once
#include "D3DApp.h"
#include "Vertex.h"
#include "D3DCommon.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "SubMeshGeometry.h"
#include "GeometryGenerator.h"
#include "Transform.h"
#include <cmath>
/* Engine takes care of our game (example: ImGui etc) */
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
	
	void Initialize(const std::shared_ptr<Win32App> window, const LPCWSTR vsPath, const LPCWSTR psPath) override;
	void Update() override;
	void Draw() override;
	void SwapBuffers() const;
	void Clean() override;

	void BuildDescriptorHeaps();	// Used for RTV
	void BuildConstantBufferViews();
	void BuildRootSignature();
	void AssemblePipeline(const LPCWSTR vsPath, const LPCWSTR psPath);

private:
	std::unique_ptr<ConstantBuffer> m_constantBuffer;
	std::shared_ptr<MeshGeometry> m_boxMesh;
	
	Microsoft::WRL::ComPtr<ID3D12RootSignature> m_rootSignature;
	Microsoft::WRL::ComPtr<ID3D12Resource> m_cbvResource;

	static Engine* s_pInstance;

} Engine, D3D12Engine, D3D12App;