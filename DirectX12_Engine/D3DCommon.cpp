#include "D3DCommon.h"
XMFLOAT4 Camera::Eye = { 0.0f, 3.0f, -3.0f, 1.0f };
XMFLOAT4 Camera::Focus = { 0.0f, 0.0f, 0.0f, 1.0f };
XMFLOAT4 Camera::Up = { 0.0f, 1.0f, 0.0f, 1.0f };
float Camera::m_Pitch = 30.0f;
float Camera::m_Yaw = -90.0f;
float Camera::m_Radius = 5.0f;
XMFLOAT2 Util::ClientSize = { 0.0f, 0.0f };
void Camera::UpdateEyePosition()
{
	Eye.x = m_Radius * (cos_radians(m_Yaw) * cos_radians(m_Pitch));
	Eye.y = m_Radius * (sin_radians(m_Pitch));
	Eye.z = m_Radius * (sin_radians(m_Yaw) * cos_radians(m_Pitch));
}

XMVECTOR& Util::CalculateAverageNormal(XMFLOAT3 t1, XMFLOAT3 t2, XMFLOAT3 t3)
{
	XMFLOAT3 uNorm;
	XMFLOAT3 vNorm;

	uNorm.x = t2.x = t1.x;
	uNorm.y = t2.y = t1.y;
	uNorm.z = t2.z = t1.z;

	vNorm.x = t3.x = t1.x;
	vNorm.y = t3.y = t1.y;
	vNorm.z = t3.z = t1.z;

	XMVECTOR Normal = XMVector3Normalize(XMVector3Cross(XMLoadFloat3(&uNorm), XMLoadFloat3(&vNorm)));

	return Normal;
}
