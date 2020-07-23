#pragma once
#include "SubMeshGeometry.h"

#define cos_radians(_x) cos(static_cast<float>((_x)* DirectX::XM_PI / 180.0f))
#define sin_radians(_x) sin(static_cast<float>((_x)* DirectX::XM_PI / 180.0f))
class GeometryGenerator
{
public:
	// Example 
	std::shared_ptr<MeshGeometry> CreateTriangle(ID3D12Device* device);

	std::shared_ptr<MeshGeometry> CreateBox(ID3D12Device * device, float width, float height, float depth);
	std::shared_ptr<MeshGeometry> CreateGrid(ID3D12Device * device, float width, float depth);
	std::shared_ptr<MeshGeometry> CreateCylinder(ID3D12Device * device, float topRadius, float bottomRadius, float height, int sliceCount);
};