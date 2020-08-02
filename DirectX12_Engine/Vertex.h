#pragma once
#include <DirectXMath.h>
using namespace DirectX;
class Vertex
{
public:
	void SetPosition(XMFLOAT3 position);
	void SetUV(XMFLOAT2 uv);
	void SetNormal(XMFLOAT3 normal);

	XMFLOAT3 GetPosition()const;
	XMFLOAT2 GetUV()const;
	XMFLOAT3 GetNormal()const;

	Vertex();
	Vertex(Vertex* other);
	Vertex(XMFLOAT3 position, XMFLOAT2 uv, XMFLOAT3 normal);
	
	void Zero();
	void operator=(Vertex other);
	bool operator==(Vertex other);


private:
	XMFLOAT3 Position;
	XMFLOAT2 UV;
	XMFLOAT3 Normal;
};