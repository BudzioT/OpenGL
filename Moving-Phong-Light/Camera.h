#ifndef CAMERA_H
#define CAMERA_H

#include "glad/glad.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

/* Camera default values */
namespace Camera_consts {
	const float YAW = -90.0f;
	const float PITCH = 0.0f;
	const float ZOOM = 45.0f;
	const float SENSITIVITY = 0.1f;
	const float SPEED = 2.5f;
}

/* Movement directions enum */
enum class Camera_movement {
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT
};


/* Camera class */
class Camera
{
public:
	/* Constructors */

	/* Vector */
	Camera(glm::vec3 position, glm::vec3 up, float yaw, float pitch);
	/* Scalar */
	Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch);

	/* Get functions */
	glm::mat4 GetViewMatrix() const;

	/* Process functions */
	void ProcessKeyboard(Camera_movement direction, float deltaTime);
	void ProcessMouseMovement(float xOffset, float yOffset, GLboolean constraintPitch);
	void ProcessMouseScroll(float yOffset);
private:
	void updateCameraVectors();
public:
	glm::vec3 Position;
	glm::vec3 Front;
	glm::vec3 Up;
	glm::vec3 Right;
	glm::vec3 WorldUp;
	/* Euler angles */
	float Yaw;
	float Pitch;
	/* Camera options */
	float MouseSensitivity;
	float MoveSpeed;
	float Zoom;
};


void Camera::updateCameraVectors()
{
	glm::vec3 front;
	front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
	front.y = sin(glm::radians(Pitch));
	front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
	Front = glm::normalize(front);

	Right = glm::normalize(glm::cross(Front, WorldUp));
	Up = glm::normalize(glm::cross(Right, Front));
}

Camera::Camera(glm::vec3 position = { 0.0f, 0.0f, 0.0f }, glm::vec3 up = { 0.0f, 1.0f, 0.0f }, 
	float yaw = Camera_consts::YAW, float pitch = Camera_consts::PITCH)
	: Position(position), Front({ 0.0f, 0.0f, -1.0f }), WorldUp(up), Yaw(yaw), Pitch(pitch),
	MouseSensitivity(Camera_consts::SENSITIVITY), MoveSpeed(Camera_consts::SPEED), Zoom(Camera_consts::ZOOM)
{
	updateCameraVectors();
}

Camera::Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, 
	float yaw = Camera_consts::YAW, float pitch = Camera_consts::PITCH) : Position(posX, posY, posZ), 
	Front({ 0.0f, 0.0f, -1.0f }), WorldUp(upX, upY, upZ), Yaw(yaw), Pitch(pitch), 
	MouseSensitivity(Camera_consts::SENSITIVITY), MoveSpeed(Camera_consts::SPEED), Zoom(Camera_consts::ZOOM)
{
	updateCameraVectors();
}

glm::mat4 Camera::GetViewMatrix() const
{
	return glm::lookAt(Position, Position + Front, WorldUp);
}

void Camera::ProcessKeyboard(Camera_movement direction, float deltaTime)
{
	float velocity = MoveSpeed * deltaTime;
	if (direction == Camera_movement::FORWARD)
		Position += Front * velocity;
	else if (direction == Camera_movement::BACKWARD)
		Position -= Front * velocity;
	else if (direction == Camera_movement::LEFT)
		Position -= Right * velocity;
	else if (direction == Camera_movement::RIGHT)
		Position += Right * velocity;
	updateCameraVectors();
}

void Camera::ProcessMouseMovement(float xOffset, float yOffset, GLboolean constraintPitch = GL_TRUE)
{
	xOffset *= MouseSensitivity;
	yOffset *= MouseSensitivity;

	Yaw += xOffset;
	Pitch += yOffset;
	
	if (constraintPitch) {
		if (Pitch > 89.0f)
			Pitch = 89.0f;
		else if (Pitch < -89.0f)
			Pitch = -89.0f;
	}
	updateCameraVectors();
}

void Camera::ProcessMouseScroll(float yOffset)
{
	Zoom -= yOffset;
	if (Zoom < 1.0f)
		Zoom = 1.0f;
	else if (Zoom > 45.0f)
		Zoom = 45.0f;
}

#endif