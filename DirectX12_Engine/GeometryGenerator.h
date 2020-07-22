#pragma once
#include "SubMeshGeometry.h"
class GeometryGenerator
{
public:
	// Example 
	MeshGeometry* CreateTriangle(ID3D12Device* device);

	MeshGeometry* CreateBox(ID3D12Device * device, float width, float height, float depth);
	MeshGeometry* CreateGrid(ID3D12Device * device, float width, float depth);
};