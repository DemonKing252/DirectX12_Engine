#pragma once
#include "SubMeshGeometry.h"
class GeometryGenerator
{
public:
	// Example 
	std::shared_ptr<MeshGeometry> CreateTriangle(ID3D12Device* device);

	std::shared_ptr<MeshGeometry> CreateBox(ID3D12Device * device, float width, float height, float depth);
	std::shared_ptr<MeshGeometry> CreateGrid(ID3D12Device * device, float width, float depth);
};