#pragma once
#include <DirectXMath.h>
#include "EntityComponentSystem.h"
#include <d3d12.h>
#include "d3dx12.h"
#include <wrl.h>

using namespace DirectX;


#define cos_radians(_x) cos(static_cast<float>((_x)* XM_PI / 180.0f))
#define sin_radians(_x) sin(static_cast<float>((_x)* XM_PI / 180.0f))

// Future Project: Move this to be its own component of entity MeshGeometry
struct Texture
{
	Microsoft::WRL::ComPtr<ID3D12Resource> m_texResource;
	Microsoft::WRL::ComPtr<ID3D12Resource> m_texHeap;

	CD3DX12_GPU_DESCRIPTOR_HANDLE hGPUHandle;
};

struct MaterialComponent : public Component
{
	XMFLOAT4 DiffuseAlbedo;
	Texture texture;
};
constexpr int NUM_POINT_LIGHTS = 1;

struct PointLight
{
	XMFLOAT3 Position;
	float FallOffStart;

	XMFLOAT3 Strength;
	float FallOffEnd;

	XMFLOAT3 Direction;
	float SpecularStrength;
};

struct ConstantBuffer
{
	XMMATRIX World;
	XMMATRIX Model;
	XMMATRIX View;
	XMMATRIX Projection;

	// Because of issues with my component class messing up byte alignment
	XMFLOAT4 DiffuseAlbedo;
	XMFLOAT4 EyeWorldSpace;

	PointLight pLight[NUM_POINT_LIGHTS];
};

struct Camera
{
	static void UpdateEyePosition();

	static XMFLOAT4 Eye;
	static XMFLOAT4 Focus;
	static XMFLOAT4 Up;

	static float m_Radius;
	static float m_Pitch;
	static float m_Yaw;
};