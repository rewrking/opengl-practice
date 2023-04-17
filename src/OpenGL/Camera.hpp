#pragma once

#include "OpenGL/GLM.hpp"

namespace ogl
{
enum class CameraMovement
{
	Forward,
	Backward,
	Left,
	Right
};

struct Camera
{
	explicit Camera(const Vec3f& inPosition);
	Camera(const Vec3f& inPosition, const Vec3f& inUp, const f32 inYaw, const f32 inPitch);
	Camera(const f32 posX, const f32 posY, const f32 posZ);
	Camera(f32 posX, f32 posY, f32 posZ, f32 upX, f32 upY, f32 upZ, const f32 inYaw, const f32 inPitch);

	// in radians
	f32 getFieldOfView() const;
	Mat4f getViewMatrix() const;

	const Vec3f& position() const noexcept;

	void processKeyboard(const CameraMovement inDirection, const f32 inDeltaTime);
	void processMouseMovement(f32 offsetX, f32 offsetY, const bool inConstrainPitch = true);
	void processMouseScroll(const f32 inYOffset);

private:
	void updateCameraVectors();

	// camera Attributes
	Vec3f m_position{ 0.0f, 0.0f, 0.0f };
	Vec3f m_front{ 0.0f, 0.0f, -1.0f };
	Vec3f m_up{ 0.0f, 0.0f, 0.0f };
	Vec3f m_right{ 0.0f, 0.0f, 0.0f };
	Vec3f m_worldUp{ 0.0f, 1.0f, 0.0f };

	// euler Angles
	f32 m_yaw = -90.0f;
	f32 m_pitch = 0.0f;

	// camera options
	f32 m_movementSpeed = 2.5f;
	f32 m_mouseSensitivity = 0.05f;
	f32 m_zoom = 45.0f;
};
}
