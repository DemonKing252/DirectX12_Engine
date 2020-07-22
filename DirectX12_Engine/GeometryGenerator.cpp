#include "GeometryGenerator.h"

MeshGeometry * GeometryGenerator::CreateTriangle(ID3D12Device * device)
{
	MeshGeometry* mesh = new MeshGeometry();


	Vertex vertices[3] =
	{
		/*0*/Vertex(DirectX::XMFLOAT3(-0.5f, -0.5f, +0.0f), DirectX::XMFLOAT2(0.0f, 0.0f), DirectX::XMFLOAT3(1.0f, 0.0f, 0.0f)),
		/*1*/Vertex(DirectX::XMFLOAT3(+0.0f, +0.5f, +0.0f), DirectX::XMFLOAT2(0.0f, 0.0f), DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f)),
		/*2*/Vertex(DirectX::XMFLOAT3(+0.5f, -0.5f, +0.0f), DirectX::XMFLOAT2(0.0f, 0.0f), DirectX::XMFLOAT3(0.0f, 0.0f, 1.0f)),
	};

	std::uint16_t indices[3] = { 0, 1, 2 };

	mesh->VertexCount = _countof(vertices);
	mesh->IndexCount = _countof(indices);

	mesh->AddComponent<VertexBufferComponent<Vertex>>();
	mesh->GetComponent<VertexBufferComponent<Vertex>>().Initialize(device, vertices, _countof(vertices));

	// Triangle Index Buffer Component
	mesh->AddComponent<IndexBufferComponent<std::uint16_t>>();
	mesh->GetComponent<IndexBufferComponent<std::uint16_t>>().Initialize(device, indices, _countof(indices));

	return mesh;
}

MeshGeometry* GeometryGenerator::CreateBox(ID3D12Device * device, float width, float height, float depth)
{
	MeshGeometry* mesh = new MeshGeometry();


	Vertex vertices[] =
	{
		//								[  vertex position  ]					[texture coord]				   [	  normal	  ]
		/*0*/	Vertex(DirectX::XMFLOAT3(-width * 0.5f, -height * 0.5f, -depth * 0.5f), DirectX::XMFLOAT2(0.0f, 0.0f), DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f)),
		/*1*/	Vertex(DirectX::XMFLOAT3(+width * 0.5f, -height * 0.5f, -depth * 0.5f), DirectX::XMFLOAT2(1.0f, 0.0f), DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f)),
		/*2*/	Vertex(DirectX::XMFLOAT3(+width * 0.5f, -height * 0.5f, +depth * 0.5f), DirectX::XMFLOAT2(1.0f, 1.0f), DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f)),
		/*3*/	Vertex(DirectX::XMFLOAT3(-width * 0.5f, -height * 0.5f, +depth * 0.5f), DirectX::XMFLOAT2(0.0f, 1.0f), DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f)),

		/*4*/	Vertex(DirectX::XMFLOAT3(-width * 0.5f, +height * 0.5f, -depth * 0.5f), DirectX::XMFLOAT2(0.0f, 0.0f), DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f)),
		/*5*/	Vertex(DirectX::XMFLOAT3(+width * 0.5f, +height * 0.5f, -depth * 0.5f), DirectX::XMFLOAT2(1.0f, 0.0f), DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f)),
		/*6*/	Vertex(DirectX::XMFLOAT3(+width * 0.5f, +height * 0.5f, +depth * 0.5f), DirectX::XMFLOAT2(1.0f, 1.0f), DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f)),
		/*7*/	Vertex(DirectX::XMFLOAT3(-width * 0.5f, +height * 0.5f, +depth * 0.5f), DirectX::XMFLOAT2(0.0f, 1.0f), DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f)),

		/*8*/	Vertex(DirectX::XMFLOAT3(-width * 0.5f, -height * 0.5f, -depth * 0.5f), DirectX::XMFLOAT2(0.0f, 0.0f), DirectX::XMFLOAT3(1.0f, 0.0f, 0.0f)),
		/*9*/	Vertex(DirectX::XMFLOAT3(-width * 0.5f, -height * 0.5f, +depth * 0.5f), DirectX::XMFLOAT2(1.0f, 0.0f), DirectX::XMFLOAT3(1.0f, 0.0f, 0.0f)),
		/*10*/	Vertex(DirectX::XMFLOAT3(-width * 0.5f, +height * 0.5f, +depth * 0.5f), DirectX::XMFLOAT2(1.0f, 1.0f), DirectX::XMFLOAT3(1.0f, 0.0f, 0.0f)),
		/*11*/	Vertex(DirectX::XMFLOAT3(-width * 0.5f, +height * 0.5f, -depth * 0.5f), DirectX::XMFLOAT2(0.0f, 1.0f), DirectX::XMFLOAT3(1.0f, 0.0f, 0.0f)),

		/*12*/	Vertex(DirectX::XMFLOAT3(+width * 0.5f, -height * 0.5f, -depth * 0.5f), DirectX::XMFLOAT2(0.0f, 0.0f), DirectX::XMFLOAT3(1.0f, 0.0f, 0.0f)),
		/*13*/	Vertex(DirectX::XMFLOAT3(+width * 0.5f, -height * 0.5f, +depth * 0.5f), DirectX::XMFLOAT2(1.0f, 0.0f), DirectX::XMFLOAT3(1.0f, 0.0f, 0.0f)),
		/*14*/	Vertex(DirectX::XMFLOAT3(+width * 0.5f, +height * 0.5f, +depth * 0.5f), DirectX::XMFLOAT2(1.0f, 1.0f), DirectX::XMFLOAT3(1.0f, 0.0f, 0.0f)),
		/*15*/	Vertex(DirectX::XMFLOAT3(+width * 0.5f, +height * 0.5f, -depth * 0.5f), DirectX::XMFLOAT2(0.0f, 1.0f), DirectX::XMFLOAT3(1.0f, 0.0f, 0.0f)),

		/*16*/	Vertex(DirectX::XMFLOAT3(-width * 0.5f, -height * 0.5f, +depth * 0.5f), DirectX::XMFLOAT2(0.0f, 0.0f), DirectX::XMFLOAT3(0.0f, 0.0f, 1.0f)),
		/*17*/	Vertex(DirectX::XMFLOAT3(+width * 0.5f, -height * 0.5f, +depth * 0.5f), DirectX::XMFLOAT2(1.0f, 0.0f), DirectX::XMFLOAT3(0.0f, 0.0f, 1.0f)),
		/*18*/	Vertex(DirectX::XMFLOAT3(+width * 0.5f, +height * 0.5f, +depth * 0.5f), DirectX::XMFLOAT2(1.0f, 1.0f), DirectX::XMFLOAT3(0.0f, 0.0f, 1.0f)),
		/*19*/	Vertex(DirectX::XMFLOAT3(-width * 0.5f, +height * 0.5f, +depth * 0.5f), DirectX::XMFLOAT2(0.0f, 1.0f), DirectX::XMFLOAT3(0.0f, 0.0f, 1.0f)),

		/*20*/	Vertex(DirectX::XMFLOAT3(-width * 0.5f, -height * 0.5f, -depth * 0.5f), DirectX::XMFLOAT2(0.0f, 0.0f), DirectX::XMFLOAT3(0.0f, 0.0f, 1.0f)),
		/*21*/	Vertex(DirectX::XMFLOAT3(+width * 0.5f, -height * 0.5f, -depth * 0.5f), DirectX::XMFLOAT2(1.0f, 0.0f), DirectX::XMFLOAT3(0.0f, 0.0f, 1.0f)),
		/*22*/	Vertex(DirectX::XMFLOAT3(+width * 0.5f, +height * 0.5f, -depth * 0.5f), DirectX::XMFLOAT2(1.0f, 1.0f), DirectX::XMFLOAT3(0.0f, 0.0f, 1.0f)),
		/*23*/	Vertex(DirectX::XMFLOAT3(-width * 0.5f, +height * 0.5f, -depth * 0.5f), DirectX::XMFLOAT2(0.0f, 1.0f), DirectX::XMFLOAT3(0.0f, 0.0f, 1.0f)),
	};
	std::uint16_t indices[]
	{
		// bottom
		0, 1, 2,
		2, 3, 0,

		// top
		6, 5, 4,
		4, 7, 6,

		// left
		8, 9, 10,
		10, 11, 8,

		// right
		14, 13, 12,
		12, 15, 14,

		// back 
		16, 17, 18,
		18, 19, 16,

		// front
		22, 21, 20,
		20, 23, 22

	};

	mesh->VertexCount = ARRAYSIZE(vertices);
	mesh->IndexCount = ARRAYSIZE(indices);
	
	mesh->AddComponent<VertexBufferComponent<Vertex>>();
	mesh->GetComponent<VertexBufferComponent<Vertex>>().Initialize(device, vertices, _countof(vertices));

	mesh->AddComponent<IndexBufferComponent<std::uint16_t>>();
	mesh->GetComponent<IndexBufferComponent<std::uint16_t>>().Initialize(device, indices, _countof(indices));

	return mesh;
}

MeshGeometry* GeometryGenerator::CreateGrid(ID3D12Device * device, float width, float depth)
{
	MeshGeometry* mesh = new MeshGeometry();


	Vertex vertices[] =
	{
		/*0*/	Vertex(DirectX::XMFLOAT3(-width * 0.5f, 0.0f, +depth * 0.5f), DirectX::XMFLOAT2(0.0f, 1.0f), DirectX::XMFLOAT3(0.0f, +1.0f, 0.0f)),
		/*1*/	Vertex(DirectX::XMFLOAT3(+width * 0.5f, 0.0f, +depth * 0.5f), DirectX::XMFLOAT2(1.0f, 1.0f), DirectX::XMFLOAT3(0.0f, +1.0f, 0.0f)),
		/*2*/	Vertex(DirectX::XMFLOAT3(+width * 0.5f, 0.0f, -depth * 0.5f), DirectX::XMFLOAT2(1.0f, 0.0f), DirectX::XMFLOAT3(0.0f, +1.0f, 0.0f)),
		/*3*/	Vertex(DirectX::XMFLOAT3(-width * 0.5f, 0.0f, -depth * 0.5f), DirectX::XMFLOAT2(0.0f, 0.0f), DirectX::XMFLOAT3(0.0f, +1.0f, 0.0f)),

	};
	std::uint16_t indices[]
	{
		// bottom
		0, 1, 2,
		2, 3, 0

	};

	mesh->VertexCount = ARRAYSIZE(vertices);
	mesh->IndexCount = ARRAYSIZE(indices);

	mesh->AddComponent<VertexBufferComponent<Vertex>>();
	mesh->GetComponent<VertexBufferComponent<Vertex>>().Initialize(device, vertices, _countof(vertices));

	mesh->AddComponent<IndexBufferComponent<std::uint16_t>>();
	mesh->GetComponent<IndexBufferComponent<std::uint16_t>>().Initialize(device, indices, _countof(indices));

	return mesh;
}