#pragma once
#include "MeshGeometry.h"
#include "D3DCommon.h"

// This class is almost directly copy-pasted from my DirectX 11 Engine, since DirectX Math doesn't change
class GeometryGenerator
{
public:
	// Example 
	std::shared_ptr<MeshGeometry> CreateTriangle(ID3D12Device* device);

	std::shared_ptr<MeshGeometry> CreateBox(ID3D12Device * device, float width, float height, float depth);
	std::shared_ptr<MeshGeometry> CreateGrid(ID3D12Device * device, float width, float depth, int cols, int rows);
	std::shared_ptr<MeshGeometry> CreateCylinder(ID3D12Device * device, float topRadius, float bottomRadius, float height, int sliceCount);
	std::shared_ptr<MeshGeometry> CreateSphere(ID3D12Device * device, float radius, int sliceCount, int stackCount);

};