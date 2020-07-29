#pragma once
#include <DirectXMath.h>
#include "EntityComponentSystem.h"
#include <d3d12.h>
#include "d3dx12.h"
#include <wrl.h>
#define cos_radians(_x) cos(static_cast<float>((_x)* DirectX::XM_PI / 180.0f))
#define sin_radians(_x) sin(static_cast<float>((_x)* DirectX::XM_PI / 180.0f))

// Future Project: Move this to be its own component of entity MeshGeometry
struct Texture
{
	Microsoft::WRL::ComPtr<ID3D12Resource> m_texResource;
	Microsoft::WRL::ComPtr<ID3D12Resource> m_texHeap;

	CD3DX12_GPU_DESCRIPTOR_HANDLE hGPUHandle;
};

struct MaterialComponent : public Component
{
	DirectX::XMFLOAT4 DiffuseAlbedo;
	Texture texture;
};

struct ConstantBuffer
{
	DirectX::XMMATRIX World;
	DirectX::XMMATRIX Model;
	DirectX::XMMATRIX View;
	DirectX::XMMATRIX Projection;

	// Because of issues with my component class messing up byte alignment
	DirectX::XMFLOAT4 DiffuseAlbedo;
};

struct Camera
{
	static void UpdateEyePosition();

	static DirectX::XMFLOAT4 Eye;
	static DirectX::XMFLOAT4 Focus;
	static DirectX::XMFLOAT4 Up;

	static float m_Radius;
	static float m_Pitch;
	static float m_Yaw;
};