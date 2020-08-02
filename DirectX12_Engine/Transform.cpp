#include "Transform.h"

Transform::Transform()
{
}

Transform::~Transform()
{
}

Transform::Transform(Transform * other)
{
	this->Translate = other->GetTranslation();
	this->RotationAxis = other->GetRotationAxis();
	this->Scale = other->GetScaling();
	this->Angle = other->GetAngle();
}

Transform::Transform(XMFLOAT3 translate, XMFLOAT3 rotAxis, XMFLOAT3 scale, FLOAT angle)
{
	this->Translate = translate;
	this->RotationAxis = rotAxis;
	this->Scale = scale;
	this->Angle = angle;
}

void Transform::SetTranslation(XMFLOAT3 translate)
{
	this->Translate = translate;
}

void Transform::SetRotationAxis(XMFLOAT3 rotAxis)
{
	this->RotationAxis = rotAxis;
}

void Transform::SetScaling(XMFLOAT3 scale)
{
	this->Scale = scale;
}

void Transform::SetAngle(FLOAT angle)
{
	this->Angle = angle;
}

void Transform::SetModelMatrix(XMMATRIX matrix)
{
	this->Model = matrix;
}

void Transform::Zero()
{
	this->Model = XMMatrixIdentity();
	this->Scale = this->RotationAxis = this->Translate = XMFLOAT3(0.0f, 0.0f, 0.0f);
	this->Angle = 0.0f;

}

XMMATRIX Transform::GetModelMatrix() const
{
	return Model;
}

XMFLOAT3  Transform::GetTranslation()const
{
	return this->Translate;
}

XMFLOAT3  Transform::GetRotationAxis()const
{
	return this->RotationAxis;
}

XMFLOAT3  Transform::GetScaling()const
{
	return this->Scale;
}

FLOAT Transform::GetAngle()const
{
	return this->Angle;
}