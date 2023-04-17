#include "OpenGL/Camera.hpp"

#include "OpenGL/OpenGL.hpp"

namespace ogl
{
/*****************************************************************************/
Camera::Camera(const Vec3f& inPosition) :
	m_position(inPosition)
{
}

/*****************************************************************************/
Camera::Camera(const Vec3f& inPosition, const Vec3f& inUp, const f32 inYaw, const f32 inPitch) :
	m_position(inPosition),
	m_worldUp(inUp),
	m_yaw(inYaw),
	m_pitch(inPitch)
{
	updateCameraVectors();
}

/*****************************************************************************/
Camera::Camera(const f32 posX, const f32 posY, const f32 posZ) :
	m_position({ posX, posY, posZ })
{
}

/*****************************************************************************/
Camera::Camera(const f32 posX, const f32 posY, const f32 posZ, const f32 upX, const f32 upY, const f32 upZ, const f32 inYaw, const f32 inPitch) :
	m_position({ posX, posY, posZ }),
	m_worldUp({ upX, upY, upZ }),
	m_yaw(inYaw),
	m_pitch(inPitch)
{
	updateCameraVectors();
}

/*****************************************************************************/
f32 Camera::getFieldOfView() const
{
	return glm::radians(m_zoom);
}

/*****************************************************************************/
Mat4f Camera::getViewMatrix() const
{
	return glm::lookAt(m_position, m_position + m_front, m_up);
}

/*****************************************************************************/
void Camera::processKeyboard(const CameraMovement inDirection, const f32 inDeltaTime)
{
	const f32 velocity = m_movementSpeed * inDeltaTime;

	switch (inDirection)
	{
		case CameraMovement::Forward:
			m_position += m_front * velocity;
			break;

		case CameraMovement::Backward:
			m_position -= m_front * velocity;
			break;

		case CameraMovement::Left:
			m_position -= m_right * velocity;
			break;

		case CameraMovement::Right:
			m_position += m_right * velocity;
			break;

		default: break;
	}
}

/*****************************************************************************/
void Camera::processMouseMovement(f32 offsetX, f32 offsetY, const bool inConstrainPitch)
{
	offsetX *= m_mouseSensitivity;
	offsetY *= m_mouseSensitivity;

	m_yaw += offsetX;
	m_pitch += offsetY;

	if (inConstrainPitch)
	{
		if (m_pitch > 89.0f)
			m_pitch = 89.0f;

		if (m_pitch < -89.0f)
			m_pitch = -89.0f;
	}

	updateCameraVectors();
}

/*****************************************************************************/
void Camera::processMouseScroll(const f32 inYOffset)
{
	m_zoom -= inYOffset;

	if (m_zoom < 1.0f)
		m_zoom = 1.0f;

	if (m_zoom > 45.0f)
		m_zoom = 45.0f;
}

/*****************************************************************************/
void Camera::updateCameraVectors()
{
	Vec3f front;
	front.x = cos(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
	front.y = sin(glm::radians(m_pitch));
	front.z = sin(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
	m_front = glm::normalize(front);

	m_right = glm::normalize(glm::cross(m_front, m_worldUp));
	m_up = glm::normalize(glm::cross(m_right, m_front));
}

}