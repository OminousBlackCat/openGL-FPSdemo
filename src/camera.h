#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>

// const camera initialize value
const float YAW = -90.0f;
const float PITCH = 0.0f;
const glm::vec3 WORLD_UP = glm::vec3(0.0f, 1.0f, 0.0f);
const float MOVEMENT_SPEED = 5.0f;
const float CURSOR_SEN = 0.05f;
const float SCROOL_SEN = 0.1f;
const float ZOOM_INIT = 45.0f;



class Camera {
public:
	// attribute
	glm::vec3 position; // camera world positon
	glm::vec3 frontVec; // camera front Vector, negtive with direction vector
	glm::vec3 up; // camera up vector
	glm::vec3 right; // camera right vector
	// euler angles, using euler angle to determine the front vec
	float yaw;
	float pitch;
	// options
	float movementSpeed; // control camera move
	float cursorSensitivity; // control cursor
	float scrollSensitivity; // control scroll
	float zoomAngle; // control zooming

	// movement direction
	const static int CAMERA_FORWARD = 0;
	const static int CAMERA_BACKWARD = 1;
	const static int CAMERA_LEFT = 2;
	const static int CAMERA_RIGHT = 3;

	//constructor
	Camera(glm::vec3 init_postion = glm::vec3(0.0f, 0.0f, 3.0f), float yaw = YAW, float pitch = PITCH) {
		this->position = init_postion;
		this->yaw = yaw;
		this->pitch = pitch;
		updateCameraVector();
		// give default value
		this->movementSpeed = MOVEMENT_SPEED;
		this->cursorSensitivity = CURSOR_SEN;
		this->scrollSensitivity = SCROOL_SEN;
		this->zoomAngle = ZOOM_INIT;
	}

	glm::mat4 getLookAtMat() {
		return glm::lookAt(this->position, this->position + this->frontVec, this->up);
	}

	void movement(int direction, float deltaTime) {
		if (direction < 0 || direction > 3) {
			return;
		}
		else{
			float velocity = movementSpeed * deltaTime;
			switch (direction)
			{
			case CAMERA_FORWARD:
				this->position += frontVec * velocity;
				break;
			case CAMERA_BACKWARD:
				this->position -= velocity * frontVec;
				break;
			case CAMERA_LEFT:
				this->position += glm::normalize(glm::cross(up, frontVec)) * velocity;
				break;
			case CAMERA_RIGHT:
				this->position += glm::normalize(glm::cross(frontVec, up)) * velocity;
				break;
			default:
				break;
			}
			updateCameraVector();
		}
	}


	void yawAndPitch(float xoffset, float yoffset) {
		this->yaw += xoffset * this->cursorSensitivity;
		this->pitch += yoffset * this->cursorSensitivity;

		if (this->pitch > 89.0f)
			this->pitch = 89.0f;
		if (this->pitch < -89.0f)
			this->pitch = -89.0f;
		
		updateCameraVector();
	}


	void zoom(float offset) {
		this->zoomAngle -= offset * scrollSensitivity;
		if (this->zoomAngle < 1.0f)
			this->zoomAngle = 1.0f;
		if (this->zoomAngle > 45.0f)
			this->zoomAngle = 45.0f;
	}

private:
	void updateCameraVector() {
		glm::vec3 front;
		front.x = cos(glm::radians(this->yaw)) * cos(glm::radians(this->pitch));
		front.y = sin(glm::radians(this->pitch));
		front.z = sin(glm::radians(this->yaw)) * cos(glm::radians(this->pitch));
		this->frontVec = glm::normalize(front);

		this->right = glm::normalize(glm::cross(frontVec, WORLD_UP));
		this->up = glm::normalize(glm::cross(right, frontVec));
	}
};