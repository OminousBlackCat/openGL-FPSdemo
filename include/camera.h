#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include "object.h"

// const camera initialize value
const float YAW = -90.0f;
const float PITCH = 0.0f;
const glm::vec3 WORLD_UP = glm::vec3(0.0f, 1.0f, 0.0f);
const float MOVEMENT_SPEED = 3.0f;
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

	// jump parameter
	const float jumpAcceleration = 10.0f; // 重力加速度
	const float jumpInitVelocity = 10.0f;
	float jumpVelocity;
	bool isJumping = false;

	// FPS mode or free mode
	bool ifFpsMode = true;
    // if has collision
    bool ifContinue = true;

    // 碰撞盒的宽度与高度
    // 注意: 摄像机所在位置处于碰撞盒顶层面的中心
    float boxWidth = 0.5f;
    float boxHeight = 1.0f;
    float boxLength = 0.5f;

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
		this->jumpVelocity = this->jumpInitVelocity;
	}

	glm::mat4 getLookAtMat() const {
		return glm::lookAt(this->position, this->position + this->frontVec, this->up);
	}

	void movement(int direction, float deltaTime) {
        glm::vec3 currentPosition(this->position);
		if (direction < 0 || direction > 3) {
			return;
		}
		else{
			float velocity = movementSpeed * deltaTime;
			if(this -> ifFpsMode){
				switch (direction)
				{
					case CAMERA_FORWARD:
						this->position += glm::vec3(frontVec.x, 0.0f, frontVec.z) * velocity;
						break;
					case CAMERA_BACKWARD:
						this->position -= velocity * glm::vec3(frontVec.x, 0.0f, frontVec.z);
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
			}else{
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
			}		
			updateCameraVector();
		}
	}

	void down(float deltaTime){
		if(!this->ifFpsMode)
			return;
        position.y += jumpVelocity * deltaTime;
        jumpVelocity -= this->jumpAcceleration * deltaTime;
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
		if (this->zoomAngle < 44.0f){
			this->zoomAngle = 44.0f;
		}
		if (this->zoomAngle > 45.0f)
			this->zoomAngle = 45.0f;
	}

    bool ifCollision(const Object& currentObject) const{
        // 判断照相机的box是否和输入的object(碰撞盒也是个长方体)
        // 首先在xz平面使用2d-aabb算法判断是否有相交
        // 计算照相机碰撞盒的x边缘是否在object的两个x边之内
        bool XZ_collision = false;
        bool collisionX_one =   this->position.x + this->boxLength / 2 <= currentObject.position.x + currentObject.boxLength / 2 &
                                this->position.x + this->boxLength / 2 >= currentObject.position.x - currentObject.boxLength / 2;
        bool collisionX_two =   this->position.x - this->boxLength / 2 <= currentObject.position.x + currentObject.boxLength / 2 &
                                this->position.x + this->boxLength / 2 >= currentObject.position.x - currentObject.boxLength / 2;
        if(collisionX_one | collisionX_two){
            bool collisionZ_one =   this->position.z + this->boxWidth / 2 <= currentObject.position.z + currentObject.boxWidth / 2 &
                                    this->position.z + this->boxWidth / 2 >= currentObject.position.z - currentObject.boxWidth / 2;
            bool collisionZ_two =    this->position.z - this->boxWidth / 2 <= currentObject.position.z + currentObject.boxWidth / 2 &
                                     this->position.z + this->boxWidth / 2 >= currentObject.position.z - currentObject.boxWidth / 2;
            XZ_collision = collisionZ_two | collisionZ_one;
        }

        // 判断Y平面是否发生碰撞
        if(XZ_collision){
            bool collisionY_one =   this->position.y <= currentObject.position.y + currentObject.boxHeight / 2 &&
                                    this->position.y > currentObject.position.y - currentObject.boxHeight / 2;
            bool collisionY_two =   this->position.y - this->boxHeight <= currentObject.position.y + currentObject.boxHeight / 2 &&
                                    this->position.y - this->boxHeight > currentObject.position.y - currentObject.boxHeight / 2;
            return (collisionY_one | collisionY_two);
        } else{
            return (false | (this->position.y < 1.0f));
        }
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