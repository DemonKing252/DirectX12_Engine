#include "Vertex.h"
void Vertex::SetPosition(XMFLOAT3 position)
{
	this->Position = position;
}

void Vertex::SetUV(XMFLOAT2 uv)
{
	this->UV = uv;
}

void Vertex::SetNormal(XMFLOAT3 normal)
{
	this->Normal = normal;
}

XMFLOAT3 Vertex::GetPosition() const
{
	return Position;
}

XMFLOAT2 Vertex::GetUV() const
{
	return UV;
}

XMFLOAT3 Vertex::GetNormal() const
{
	return Normal;
}

Vertex::Vertex()
{
}

Vertex::Vertex(Vertex * other)
	: Position(other->Position), UV(other->UV), Normal(other->Normal)
{

}

Vertex::Vertex(XMFLOAT3 position, XMFLOAT2 uv, XMFLOAT3 normal)
	: Position(position), UV(uv), Normal(normal)
{
}

void Vertex::Zero()
{
	this->Position = XMFLOAT3(0.0f, 0.0f, 0.0f);
	this->UV = XMFLOAT2(0.0f, 0.0f);
	this->Normal = XMFLOAT3(0.0f, 0.0f, 0.0f);
}

void Vertex::operator=(Vertex other)
{
	this->Position = other.Position;
	this->UV = other.UV;
	this->Normal = other.Normal;
}

bool Vertex::operator==(Vertex other)
{
	return
	(
		(this->Position.x == other.Position.x) &&
		(this->Position.x == other.Position.x) &&
		(this->Position.x == other.Position.x) &&
		(this->UV.x == other.UV.x) &&
		(this->UV.y == other.UV.y) &&
		(this->Normal.x == other.Normal.x) &&
		(this->Normal.y == other.Normal.y) &&
		(this->Normal.z == other.Normal.z)
	);
}