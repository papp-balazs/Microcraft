#include "Camera.h"
#include "MathFunctions.h"

#include <algorithm>

Camera::Camera() noexcept
{
	Reset();
}

DirectX::XMMATRIX Camera::GetMatrix() const noexcept
{
	using namespace DirectX;

	const XMVECTOR forwardBaseVector = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
	const auto lookVector = XMVector3Transform(
		forwardBaseVector,
		XMMatrixRotationRollPitchYaw(pitch, yaw, 0.0f)
	);
	const auto camPosition = XMLoadFloat3(&pos);
	const auto camTarget = camPosition + lookVector;

	return XMMatrixLookAtLH(camPosition, camTarget, XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f));
}

void Camera::Reset() noexcept
{
	pos = { 0.0f,7.5f,-18.0f };
	pitch = 0.0f;
	yaw = 0.0f;
}

void Camera::SetPos(float x, float y, float z) noexcept
{
	pos = { x, y, z };
}

void Camera::Rotate(float DirectX, float dy) noexcept
{
	yaw = wrap_angle(yaw + DirectX * rotationSpeed);
	pitch = std::clamp(pitch + dy * rotationSpeed, 0.995f * -PI / 2.0f, 0.995f * PI / 2.0f);
}

void Camera::Translate(DirectX::XMFLOAT3 translation) noexcept
{
	DirectX::XMStoreFloat3(&translation, DirectX::XMVector3Transform(
		DirectX::XMLoadFloat3(&translation),
		DirectX::XMMatrixRotationRollPitchYaw(pitch, yaw, 0.0f) *
		DirectX::XMMatrixScaling(travelSpeed, travelSpeed, travelSpeed)
	));

	pos = {
		pos.x + translation.x,
		pos.y + translation.y,
		pos.z + translation.z
	};
}

void Camera::SetTravelSpeed(float speed)
{
	travelSpeed = speed;
}

float Camera::GetTravelSpeed()
{
	return travelSpeed;
}