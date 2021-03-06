#pragma once
#include "EntityComponentSystem.h"
#include <Windows.h>
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "Vertex.h"
#include <d3d12.h>
enum Pipeline
{
	Opaque,
	Transparent,
	StencilMirror,
	StencilReflection,
	StencilShadow,
	Count
};

class MeshGeometry : public Entity
{
public:
	MeshGeometry();
	~MeshGeometry();

	Pipeline Pipeline;
	UINT IndexStart;
	UINT IndexCount;
	UINT VertexCount;

	XMFLOAT4 DiffuseAlbedo;

	// This cannot be changed to a different list like a triangle fan or a quad for now.
	D3D12_PRIMITIVE_TOPOLOGY m_d3dPrimitiveTopology;
};