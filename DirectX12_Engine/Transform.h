#pragma once
#include <DirectXMath.h>
#include <Windows.h>
#include "EntityComponentSystem.h"
using namespace DirectX;
typedef class Transform : public Component
{
private:
	XMFLOAT3 Translate;
	XMFLOAT3 Scale;
	XMFLOAT3 RotationAxis;
	FLOAT Angle;

	XMMATRIX Model;
public:

	Transform();
	~Transform();

	Transform(Transform* other);
	Transform(XMFLOAT3 translate, XMFLOAT3 rotAxis, XMFLOAT3 scale, FLOAT angle);
	
	void SetTranslation(XMFLOAT3 translate);
	void SetRotationAxis(XMFLOAT3 rotAxis);
	void SetScaling(XMFLOAT3 scale);
	void SetAngle(FLOAT angle);
	void SetModelMatrix(XMMATRIX matrix);
	void Zero();

	XMFLOAT3 GetTranslation()const;
	XMFLOAT3 GetRotationAxis()const;
	XMFLOAT3 GetScaling()const;
	FLOAT GetAngle()const;
	XMMATRIX GetModelMatrix() const;

} Transform, TransformComponent;