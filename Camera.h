#ifndef EBO_CLASS_H
#define EBO_CLASS_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glad/glad.h>

enum Camera_Movement {
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT
};

class Camera {
	glm::vec3 position;
	glm::vec3 up;
	glm::vec3 front;
	glm::vec3 right;
	glm::vec3 worldUp;
	float fov;
	float sensitivity;
	float zoom;
	float movementSpeed;
	float yaw;
	float pitch;
	float lastX;
	float lastY;

	bool firstTouch;

	Camera(glm::vec3 cameraPosition, glm::vec3 worldUp , float yaw, float pitch, float fov, float sensitivity, float movementSpeed, float zoom);
	glm::mat4 GetViewMatrix(); 
	void ProcessKeyboard(Camera_Movement direction, float deltaTime);
	void ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch = true);
	void ProcessMouseScroll(float yoffset);

private:
	void updateCameraVectors();
};

#endif
