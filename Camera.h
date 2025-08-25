#ifndef CAMERA_CLASS_H
#define CAMERA_CLASS_H

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
public:
	glm::vec3 position;
	glm::vec3 up;
	glm::vec3 front;
	glm::vec3 right;
	glm::vec3 worldUp;
	float fov;
	float sensitivity;
	float movementSpeed;
	float yaw;
	float pitch;
 
	bool firstTouch;

	Camera(glm::vec3 cameraPosition, glm::vec3 _worldUp , float _yaw, float _pitch, float _fov, float _sensitivity, float _movementSpeed);
	glm::mat4 GetViewMatrix(); 
	void ProcessKeyboardInput(Camera_Movement direction, float deltaTime);
	void ProcessMouseMovement(float detaX, float deltaY, GLboolean constrainPitch);
	void ProcessMouseScroll(float deltaY);
	void UpdateCameraVectors();
};

#endif
