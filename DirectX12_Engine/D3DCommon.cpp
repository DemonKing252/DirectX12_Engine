#include "D3DCommon.h"
XMFLOAT4 Camera::Eye = { 0.0f, 3.0f, -3.0f, 1.0f };
XMFLOAT4 Camera::Focus = { 0.0f, 0.0f, 0.0f, 1.0f };
XMFLOAT4 Camera::Up = { 0.0f, 1.0f, 0.0f, 1.0f };
float Camera::m_Pitch = 30.0f;
float Camera::m_Yaw = -90.0f;
float Camera::m_Radius = 5.0f;

void Camera::UpdateEyePosition()
{
	Eye.x = m_Radius * (cos_radians(m_Yaw) * cos_radians(m_Pitch));
	Eye.y = m_Radius * (sin_radians(m_Pitch));
	Eye.z = m_Radius * (sin_radians(m_Yaw) * cos_radians(m_Pitch));
}