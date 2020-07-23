#pragma once
#include <DirectXMath.h>
#include "EntityComponentSystem.h"

struct ConstantBuffer
{
	DirectX::XMMATRIX World;
	DirectX::XMMATRIX Model;
	DirectX::XMMATRIX View;
	DirectX::XMMATRIX Projection;

	DirectX::XMFLOAT4 DiffuseAlbedo;
};

struct Camera
{
	static DirectX::XMFLOAT4 Eye;
	static DirectX::XMFLOAT4 Focus;
	static DirectX::XMFLOAT4 Up;
};