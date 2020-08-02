#include "GeometryGenerator.h"

std::shared_ptr<MeshGeometry> GeometryGenerator::CreateTriangle(ID3D12Device * device)
{
	std::shared_ptr<MeshGeometry> mesh = std::make_shared<MeshGeometry>();


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

std::shared_ptr<MeshGeometry> GeometryGenerator::CreateBox(ID3D12Device * device, float width, float height, float depth)
{
	std::shared_ptr<MeshGeometry> mesh = std::make_shared<MeshGeometry>();


	Vertex vertices[] =
	{
		//								[  vertex position  ]					[texture coord]				   [	  normal	  ]
		/*0*/	Vertex(DirectX::XMFLOAT3(-width * 0.5f, -height * 0.5f, -depth * 0.5f), DirectX::XMFLOAT2(0.0f, 0.0f), DirectX::XMFLOAT3(0.0f, -1.0f, 0.0f)),
		/*1*/	Vertex(DirectX::XMFLOAT3(+width * 0.5f, -height * 0.5f, -depth * 0.5f), DirectX::XMFLOAT2(1.0f, 0.0f), DirectX::XMFLOAT3(0.0f, -1.0f, 0.0f)),
		/*2*/	Vertex(DirectX::XMFLOAT3(+width * 0.5f, -height * 0.5f, +depth * 0.5f), DirectX::XMFLOAT2(1.0f, 1.0f), DirectX::XMFLOAT3(0.0f, -1.0f, 0.0f)),
		/*3*/	Vertex(DirectX::XMFLOAT3(-width * 0.5f, -height * 0.5f, +depth * 0.5f), DirectX::XMFLOAT2(0.0f, 1.0f), DirectX::XMFLOAT3(0.0f, -1.0f, 0.0f)),

		/*4*/	Vertex(DirectX::XMFLOAT3(-width * 0.5f, +height * 0.5f, -depth * 0.5f), DirectX::XMFLOAT2(0.0f, 0.0f), DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f)),
		/*5*/	Vertex(DirectX::XMFLOAT3(+width * 0.5f, +height * 0.5f, -depth * 0.5f), DirectX::XMFLOAT2(1.0f, 0.0f), DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f)),
		/*6*/	Vertex(DirectX::XMFLOAT3(+width * 0.5f, +height * 0.5f, +depth * 0.5f), DirectX::XMFLOAT2(1.0f, 1.0f), DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f)),
		/*7*/	Vertex(DirectX::XMFLOAT3(-width * 0.5f, +height * 0.5f, +depth * 0.5f), DirectX::XMFLOAT2(0.0f, 1.0f), DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f)),

		/*8*/	Vertex(DirectX::XMFLOAT3(-width * 0.5f, -height * 0.5f, -depth * 0.5f), DirectX::XMFLOAT2(0.0f, 0.0f), DirectX::XMFLOAT3(-1.0f, 0.0f, 0.0f)),
		/*9*/	Vertex(DirectX::XMFLOAT3(-width * 0.5f, -height * 0.5f, +depth * 0.5f), DirectX::XMFLOAT2(1.0f, 0.0f), DirectX::XMFLOAT3(-1.0f, 0.0f, 0.0f)),
		/*10*/	Vertex(DirectX::XMFLOAT3(-width * 0.5f, +height * 0.5f, +depth * 0.5f), DirectX::XMFLOAT2(1.0f, 1.0f), DirectX::XMFLOAT3(-1.0f, 0.0f, 0.0f)),
		/*11*/	Vertex(DirectX::XMFLOAT3(-width * 0.5f, +height * 0.5f, -depth * 0.5f), DirectX::XMFLOAT2(0.0f, 1.0f), DirectX::XMFLOAT3(-1.0f, 0.0f, 0.0f)),

		/*12*/	Vertex(DirectX::XMFLOAT3(+width * 0.5f, -height * 0.5f, -depth * 0.5f), DirectX::XMFLOAT2(0.0f, 0.0f), DirectX::XMFLOAT3(+1.0f, 0.0f, 0.0f)),
		/*13*/	Vertex(DirectX::XMFLOAT3(+width * 0.5f, -height * 0.5f, +depth * 0.5f), DirectX::XMFLOAT2(1.0f, 0.0f), DirectX::XMFLOAT3(+1.0f, 0.0f, 0.0f)),
		/*14*/	Vertex(DirectX::XMFLOAT3(+width * 0.5f, +height * 0.5f, +depth * 0.5f), DirectX::XMFLOAT2(1.0f, 1.0f), DirectX::XMFLOAT3(+1.0f, 0.0f, 0.0f)),
		/*15*/	Vertex(DirectX::XMFLOAT3(+width * 0.5f, +height * 0.5f, -depth * 0.5f), DirectX::XMFLOAT2(0.0f, 1.0f), DirectX::XMFLOAT3(+1.0f, 0.0f, 0.0f)),

		/*16*/	Vertex(DirectX::XMFLOAT3(-width * 0.5f, -height * 0.5f, +depth * 0.5f), DirectX::XMFLOAT2(0.0f, 0.0f), DirectX::XMFLOAT3(0.0f, 0.0f, 1.0f)),
		/*17*/	Vertex(DirectX::XMFLOAT3(+width * 0.5f, -height * 0.5f, +depth * 0.5f), DirectX::XMFLOAT2(1.0f, 0.0f), DirectX::XMFLOAT3(0.0f, 0.0f, 1.0f)),
		/*18*/	Vertex(DirectX::XMFLOAT3(+width * 0.5f, +height * 0.5f, +depth * 0.5f), DirectX::XMFLOAT2(1.0f, 1.0f), DirectX::XMFLOAT3(0.0f, 0.0f, 1.0f)),
		/*19*/	Vertex(DirectX::XMFLOAT3(-width * 0.5f, +height * 0.5f, +depth * 0.5f), DirectX::XMFLOAT2(0.0f, 1.0f), DirectX::XMFLOAT3(0.0f, 0.0f, 1.0f)),

		/*20*/	Vertex(DirectX::XMFLOAT3(-width * 0.5f, -height * 0.5f, -depth * 0.5f), DirectX::XMFLOAT2(0.0f, 0.0f), DirectX::XMFLOAT3(0.0f, 0.0f, -1.0f)),
		/*21*/	Vertex(DirectX::XMFLOAT3(+width * 0.5f, -height * 0.5f, -depth * 0.5f), DirectX::XMFLOAT2(1.0f, 0.0f), DirectX::XMFLOAT3(0.0f, 0.0f, -1.0f)),
		/*22*/	Vertex(DirectX::XMFLOAT3(+width * 0.5f, +height * 0.5f, -depth * 0.5f), DirectX::XMFLOAT2(1.0f, 1.0f), DirectX::XMFLOAT3(0.0f, 0.0f, -1.0f)),
		/*23*/	Vertex(DirectX::XMFLOAT3(-width * 0.5f, +height * 0.5f, -depth * 0.5f), DirectX::XMFLOAT2(0.0f, 1.0f), DirectX::XMFLOAT3(0.0f, 0.0f, -1.0f)),
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

std::shared_ptr<MeshGeometry> GeometryGenerator::CreateGrid(ID3D12Device * device, float width, float depth, int cols, int rows)
{
	std::shared_ptr<MeshGeometry> mesh = std::make_shared<MeshGeometry>();

	const int triangleCount = (rows * cols) * 6;
	const int vertexCount =   (rows * cols) * 4;

	Vertex* vertices = new Vertex[vertexCount];
	std::uint16_t* indices = new std::uint16_t[triangleCount];

	float stepOver = width / cols;
	float stepDown = depth / rows;

	float x = -width * 0.5f;
	float z = -depth * 0.5f;

	for (int i = 0; i < vertexCount; i+=4)
	{
		vertices[i + 0] = Vertex(DirectX::XMFLOAT3(x + stepOver, 0.0f, z           ), DirectX::XMFLOAT2(0.0f, 1.0f), DirectX::XMFLOAT3(0.0f, +1.0f, 0.0f));
		vertices[i + 1] = Vertex(DirectX::XMFLOAT3(x,			 0.0f, z           ), DirectX::XMFLOAT2(1.0f, 1.0f), DirectX::XMFLOAT3(0.0f, +1.0f, 0.0f));
		vertices[i + 2] = Vertex(DirectX::XMFLOAT3(x,			 0.0f, z + stepDown), DirectX::XMFLOAT2(1.0f, 0.0f), DirectX::XMFLOAT3(0.0f, +1.0f, 0.0f));											  	
		vertices[i + 3] = Vertex(DirectX::XMFLOAT3(x + stepOver, 0.0f, z + stepDown), DirectX::XMFLOAT2(0.0f, 0.0f), DirectX::XMFLOAT3(0.0f, +1.0f, 0.0f));
		
		x += stepOver;

		if (x == width*0.5f)
		{
			x = -width * 0.5f;
			z += stepDown;
		}
	}

	int index = 0;
	for (int i = 0; i < vertexCount; i+=4)
	{
		indices[index + 0] = i;
		indices[index + 1] = i + 1;
		indices[index + 2] = i + 2;

		indices[index + 3] = i + 2;
		indices[index + 4] = i + 3;
		indices[index + 5] = i;

		index += 6;
	}


	mesh->VertexCount = vertexCount;
	mesh->IndexCount = triangleCount;

	mesh->AddComponent<VertexBufferComponent<Vertex>>();
	mesh->GetComponent<VertexBufferComponent<Vertex>>().Initialize(device, vertices, vertexCount);

	mesh->AddComponent<IndexBufferComponent<std::uint16_t>>();
	mesh->GetComponent<IndexBufferComponent<std::uint16_t>>().Initialize(device, indices, triangleCount);

	return mesh;
}

std::shared_ptr<MeshGeometry> GeometryGenerator::CreateCylinder(ID3D12Device * device, float topRadius, float bottomRadius, float height, int sliceCount)
{
	_STL_VERIFY(sliceCount <= 360, "No more than 360 Sub-Divisions!");

	std::shared_ptr<MeshGeometry> mesh = std::make_shared<MeshGeometry>();

	UINT index = 0;
	float angle = 0.0f;

	float stepOver = 360/sliceCount;

	float h = -height * 0.5f;

	int vertexCount = (int)((2 * (1080 / (int)stepOver)) + 2160 / (int)stepOver);
	
	Vertex* vertices = new Vertex[vertexCount];
	for (index; index < 1080 / stepOver; index += 3)
	{
		vertices[index].SetPosition({ 0.0f,h, 0.0f });
		vertices[index].SetNormal({ 0.0f,h, 0.0f });
		vertices[index].SetUV({ 0.5f, 0.5f });

		vertices[index + 1].SetPosition({ bottomRadius * cos_radians(angle), h, bottomRadius * sin_radians(angle) });
		vertices[index + 1].SetNormal({ bottomRadius * cos_radians(angle), h, bottomRadius * sin_radians(angle) });
		vertices[index + 1].SetUV({ (0.5f + (0.5f * cos_radians((angle)))), (0.5f + (0.5f * sin_radians((angle)))) });

		vertices[index + 2].SetPosition({ bottomRadius * cos_radians(angle + stepOver), h, bottomRadius * sin_radians(angle + stepOver) });
		vertices[index + 2].SetNormal({ bottomRadius * cos_radians(angle + stepOver), h, bottomRadius * sin_radians(angle + stepOver) });
		vertices[index + 2].SetUV({ (0.5f + (0.5f * cos_radians(angle + stepOver))), (0.5f + (0.5f * sin_radians(angle + stepOver))) });

		angle += stepOver;
	}
	h = +height * 0.5f;
	angle = 0.0f;
	for (index; index < 2*(1080 / stepOver); index += 3)
	{
		vertices[index].SetPosition({ 0.0f,h, 0.0f });
		vertices[index].SetNormal({ 0.0f,h, 0.0f });
		vertices[index].SetUV({ 0.5f, 0.5f });
	
		vertices[index + 1].SetPosition({ topRadius * cos_radians(angle), h, topRadius * sin_radians(angle) });
		vertices[index + 1].SetNormal({ topRadius * cos_radians(angle), h, topRadius * sin_radians(angle) });
		vertices[index + 1].SetUV({ (0.5f + (0.5f * cos_radians((angle)))), (0.5f + (0.5f * sin_radians((angle)))) });

		vertices[index + 2].SetPosition({ topRadius * cos_radians(angle - stepOver), h, topRadius * sin_radians(angle - stepOver) });
		vertices[index + 2].SetNormal({ topRadius * cos_radians(angle - stepOver), h, topRadius * sin_radians(angle - stepOver) });
		vertices[index + 2].SetUV({ (0.5f + (0.5f * cos_radians(angle - stepOver))), (0.5f + (0.5f * sin_radians(angle - stepOver))) });

		angle -= stepOver;
	}

	DirectX::XMFLOAT3 Pos;
	angle = 0.0f;
	for (index; index < (2 * (1080 / stepOver))+2160 / stepOver; index += 6)
	{
		Pos = { bottomRadius * cos_radians(angle), -0.5f * height, bottomRadius * sin_radians(angle) };
		vertices[index].SetPosition(Pos);
		vertices[index].SetNormal(Pos);
		vertices[index].SetUV({ angle / 360.0f, 0.0f });

		Pos = { topRadius * cos_radians(angle), +0.5f * height, topRadius * sin_radians(angle) };
		vertices[index + 1].SetPosition(Pos);
		vertices[index + 1].SetNormal(Pos);
		vertices[index + 1].SetUV({ angle / 360.0f, 1.0f });
		
		Pos = { topRadius * cos_radians(angle + stepOver), +0.5f * height, topRadius * sin_radians(angle + stepOver) };
		vertices[index + 2].SetPosition(Pos);
		vertices[index + 2].SetNormal(Pos);
		vertices[index + 2].SetUV({ (angle + stepOver) / 360.0f, 1.0f });

		Pos = { topRadius * cos_radians(angle + stepOver), +0.5f * height, topRadius * sin_radians(angle + stepOver) };
		vertices[index + 3].SetPosition(Pos);
		vertices[index + 3].SetNormal(Pos);
		vertices[index + 3].SetUV({ (angle + stepOver) / 360.0f, 1.0f });

		Pos = { bottomRadius * cos_radians(angle + stepOver), -0.5f * height, bottomRadius * sin_radians(angle + stepOver) };
		vertices[index + 4].SetPosition(Pos);
		vertices[index + 4].SetNormal(Pos);
		vertices[index + 4].SetUV({ (angle + stepOver) / 360.0f, 0.0f });

		Pos = { bottomRadius * cos_radians(angle), -0.5f * height, bottomRadius * sin_radians(angle) };
		vertices[index + 5].SetPosition(Pos);
		vertices[index + 5].SetNormal(Pos);
		vertices[index + 5].SetUV({ angle / 360.0f, 0.0f });

		angle += stepOver;
	}

	std::uint16_t* indices = new std::uint16_t[vertexCount];
	
	for (UINT i = 0; i < vertexCount; i++)
		indices[i] = i;

	mesh->VertexCount = vertexCount;
	mesh->IndexCount  = vertexCount;

	mesh->AddComponent<VertexBufferComponent<Vertex>>();
	mesh->GetComponent<VertexBufferComponent<Vertex>>().Initialize(device, vertices, vertexCount);

	mesh->AddComponent<IndexBufferComponent<std::uint16_t>>();
	mesh->GetComponent<IndexBufferComponent<std::uint16_t>>().Initialize(device, indices, vertexCount);

	return mesh;
}

std::shared_ptr<MeshGeometry> GeometryGenerator::CreateSphere(ID3D12Device * device, float radius, int sliceCount, int stackCount)
{
	std::shared_ptr<MeshGeometry> mesh = std::make_shared<MeshGeometry>();

	const UINT vertexCount = (stackCount * sliceCount) * 6;
	const UINT indexCount  = (stackCount * sliceCount) * 6;

	Vertex* vertices = new Vertex[vertexCount];
	std::uint16_t* indices = new std::uint16_t[indexCount];

	DirectX::XMFLOAT3 Pos;

	float stepOver = static_cast<float>(360.0f/sliceCount);
	float stepDown = static_cast<float>(180.0f/stackCount);

	float pitch = -90.0f;
	float yaw = 0.0f;
	
	for (UINT i = 0; i < vertexCount; i+=6)
	{
		Pos.x = radius * (cos_radians(yaw) * cos_radians(pitch));
		Pos.y = radius * (sin_radians(pitch));
		Pos.z = radius * (sin_radians(yaw) * cos_radians(pitch));
		vertices[i].SetPosition(Pos);
		vertices[i].SetNormal(Pos);
		vertices[i].SetUV({ yaw / 360.0f, pitch / 180.0f });
	
		Pos.x = radius * (cos_radians(yaw) * cos_radians(pitch + stepDown));
		Pos.y = radius * (sin_radians(pitch + stepDown));
		Pos.z = radius * (sin_radians(yaw) * cos_radians(pitch + stepDown));
		vertices[i + 1].SetPosition(Pos);
		vertices[i + 1].SetNormal(Pos);
		vertices[i + 1].SetUV({ yaw / 360.0f, (pitch + stepDown) / 180.0f });
	
		Pos.x = radius * (cos_radians(yaw + stepOver) * cos_radians(pitch + stepDown));
		Pos.y = radius * (sin_radians(pitch + stepDown));
		Pos.z = radius * (sin_radians(yaw + stepOver) * cos_radians(pitch + stepDown));
		vertices[i + 2].SetPosition(Pos);
		vertices[i + 2].SetNormal(Pos);
		vertices[i + 2].SetUV({ (yaw + stepOver) / 360.0f, (pitch + stepDown) / 180.0f });

		Pos.x = radius * (cos_radians(yaw + stepOver) * cos_radians(pitch + stepDown));
		Pos.y = radius * (sin_radians(pitch + stepDown));
		Pos.z = radius * (sin_radians(yaw + stepOver) * cos_radians(pitch + stepDown));
		vertices[i + 3].SetPosition(Pos);
		vertices[i + 3].SetNormal(Pos);
		vertices[i + 3].SetUV({ (yaw + stepOver) / 360.0f, (pitch + stepDown) / 180.0f });

		Pos.x = radius * (cos_radians(yaw + stepOver) * cos_radians(pitch));
		Pos.y = radius * (sin_radians(pitch));
		Pos.z = radius * (sin_radians(yaw + stepOver) * cos_radians(pitch));
		vertices[i + 4].SetPosition(Pos);
		vertices[i + 4].SetNormal(Pos);
		vertices[i + 4].SetUV({ (yaw + stepOver) / 360.0f, pitch / 180.0f });

		Pos.x = radius * (cos_radians(yaw) * cos_radians(pitch));
		Pos.y = radius * (sin_radians(pitch));
		Pos.z = radius * (sin_radians(yaw) * cos_radians(pitch));
		vertices[i + 5].SetPosition(Pos);
		vertices[i + 5].SetNormal(Pos);
		vertices[i + 5].SetUV({ yaw / 360.0f, pitch / 180.0f });
	
		yaw += stepOver;

		if (yaw >= 0.0f && (int)yaw % 360 == 0) { pitch += stepDown; yaw = 0.0f; }
	}

	for (UINT i = indexCount; i > 0; i--)
		indices[i] = i;

	mesh->VertexCount = vertexCount;
	mesh->IndexCount = indexCount;

	mesh->AddComponent<VertexBufferComponent<Vertex>>();
	mesh->GetComponent<VertexBufferComponent<Vertex>>().Initialize(device, vertices, vertexCount);

	mesh->AddComponent<IndexBufferComponent<std::uint16_t>>();
	mesh->GetComponent<IndexBufferComponent<std::uint16_t>>().Initialize(device, indices, indexCount);

	return mesh;
}
