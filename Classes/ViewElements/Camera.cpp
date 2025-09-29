#include "Camera.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

Camera::Camera(glm::vec3 cameraPosition, glm::vec3 _worldUp, float _yaw, float _pitch, float _fov, float _sensitivity, float _movementSpeed) {
	position = cameraPosition;
	worldUp = _worldUp;
	yaw = _yaw;
	pitch = _pitch;
	fov = _fov;
	sensitivity = _sensitivity;
	movementSpeed = _movementSpeed;
	firstTouch = true;
	UpdateCameraVectors();
}	

glm::mat4 Camera::GetViewMatrix()
{
	return glm::lookAt(position, position + front, up);
}

void Camera::ProcessKeyboardInput(Camera_Movement direction, float deltaTime)
{
	float velocity = movementSpeed * deltaTime;
	if (direction == FORWARD)
		position += front * velocity;
	if (direction == BACKWARD)
		position -= front * velocity;
	if (direction == LEFT)
		position -= right * velocity;
	if (direction == RIGHT)
		position += right * velocity;

	position.y = 0.0;
}

void Camera::ProcessMouseMovement(float deltaX, float deltaY, GLboolean constrainPitch)
{
	deltaX *= sensitivity;
	deltaY *= sensitivity;

	yaw += deltaX;
	pitch += deltaY;

	// make sure that when pitch is out of bounds, screen doesn't get flipped
	if (constrainPitch)
	{
		if (pitch > 89.0f)
			pitch = 89.0f;
		if (pitch < -89.0f)
			pitch = -89.0f;

	}
	// update Front, Right and Up Vectors using the updated Euler angles
	UpdateCameraVectors();
}

void Camera::ProcessMouseScroll(float deltaY)
{
	fov -= (float)deltaY;
	if (fov < 1.0f)
		fov = 1.0f;
	if (fov > 70.0f)
		fov = 70.0f;
}

void Camera::UpdateCameraVectors() {
        glm::vec3 _front;
		_front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
		_front.y = sin(glm::radians(pitch));
		_front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
		front = glm::normalize(_front);
        // also re-calculate the Right and Up vector
        right = glm::normalize(glm::cross(front, worldUp));  // normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
        up = glm::normalize(glm::cross(right, front));
}