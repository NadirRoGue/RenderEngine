/*
* @author Nadir Román Guerrero
* @email nadir.ro.gue@gmail.com
*/

#define GLM_FORCE_RADIANS

#include "Camera.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>

#include <math.h>
#include <iostream>

Engine::Camera::Camera(float n, float f, float fov) :nearPlane(n), farPlane(f), fovy(fov)
{
	// Initialize projection matrix
	initProjectionMatrix();
	initViewMatrix();
}

Engine::Camera::Camera(const Engine::Camera & other)
{
	farPlane = other.farPlane;
	nearPlane = other.nearPlane;
	fovy = other.fovy;
	projMatrix = other.projMatrix;
	translation = other.translation;
	rotation = other.rotation;
	viewMatrix = other.viewMatrix;
}

Engine::Camera::~Camera()
{
}

void Engine::Camera::initProjectionMatrix()
{
	projMatrix = glm::mat4(0.0f);
	projMatrix[0].x = 1.0f / tan(fovy*3.14159f / 180.0f);
	projMatrix[1].y = 1.0f / tan(fovy*3.14159f / 180.0f);
	projMatrix[2].z = -(farPlane + nearPlane) / (farPlane - nearPlane);
	projMatrix[3].z = -2 * nearPlane*farPlane / (farPlane - nearPlane);
	projMatrix[2].w = -1.0;
}

void Engine::Camera::initViewMatrix()
{
	updateViewMatrix();
	oldViewMatrix = viewMatrix;
}

void Engine::Camera::setPosition(glm::vec3 pos)
{
	translation = glm::vec3(0, 0, 0);
	rotation = glm::vec3(0, 0, 0);
	translateView(pos);
}

void Engine::Camera::translateView(glm::vec3 t)
{
	float forwardDelta = t.z;
	float strafeDelta = t.x;
	float upDelta = t.y;

	glm::vec3 forward(viewMatrix[0][2], viewMatrix[1][2], viewMatrix[2][2]);
	glm::vec3 up(viewMatrix[0][1], viewMatrix[1][1], viewMatrix[2][1]);
	glm::vec3 strafe(viewMatrix[0][0], viewMatrix[1][0], viewMatrix[2][0]);

	glm::vec3 traslatedPosition = forward * forwardDelta + strafe * strafeDelta + up * upDelta;

	translation += traslatedPosition;
	updateViewMatrix();
}

void Engine::Camera::setLookAt(glm::vec3 eye, glm::vec3 target)
{
	oldViewMatrix = viewMatrix;
	viewMatrix = glm::lookAt(eye, target, glm::vec3(0, 1, 0));
	translation = -eye;
	forward = glm::normalize(glm::vec3(viewMatrix[0][2], viewMatrix[1][2], viewMatrix[2][2]));
}

void Engine::Camera::rotateView(glm::vec3 rot)
{
	rotation += rot;
	updateViewMatrix();
}

void Engine::Camera::updateViewMatrix()
{
	oldViewMatrix = viewMatrix;
	glm::mat4 identity(1.0f);

	glm::quat yawQ = glm::quat(glm::vec3(0.0f, rotation.y, 0.0f));
	yawQ = glm::normalize(yawQ);
	glm::mat4 yawMat = glm::mat4_cast(yawQ);

	glm::quat pitch = glm::quat(glm::vec3(rotation.x, 0.0f, 0.0f));
	pitch = glm::normalize(pitch);
	glm::mat4 pitchMat = glm::mat4_cast(pitch);

	viewMatrix = pitchMat * yawMat * glm::translate(identity, translation);

	//invViewMatrix = glm::inverse(viewMatrix);
	//transposeInvViewMatrix = glm::transpose(invViewMatrix);

	forward = glm::normalize(glm::vec3(viewMatrix[0][2], viewMatrix[1][2], viewMatrix[2][2]));
}

void Engine::Camera::onWindowResize(int width, int height)
{
	float fWidth = (float)width;
	float fHeight = (float)height;

	float hAngle = fovy;
	float vAngle = fovy;

	if (width > height)
	{
		//hAngle /= (1.0f / (fWidth / fHeight));
	}
	else if (height > width)
	{
		//vAngle /= (1.0f / (fHeight / fWidth));
	}

	projMatrix = glm::mat4(0.0f);
	projMatrix[0].x = 1.0f / (tan(fovy*3.14159f / 180.0f) * (fWidth / fHeight));
	projMatrix[1].y = 1.0f / tan(fovy*3.14159f / 180.0f);
	projMatrix[2].z = (farPlane + nearPlane) / (nearPlane - farPlane);
	projMatrix[3].z = 2.0f * nearPlane*farPlane / (nearPlane - farPlane);
	projMatrix[2].w = -1.0f;
}

glm::mat4 & Engine::Camera::getProjectionMatrix()
{
	return projMatrix;
}

glm::mat4 & Engine::Camera::getViewMatrix()
{
	return viewMatrix;
}

glm::mat4 & Engine::Camera::getInvViewMatrix()
{
	return invViewMatrix;
}

glm::mat4 & Engine::Camera::getTransposeInvViewMatrix()
{
	return transposeInvViewMatrix;
}

glm::mat4 & Engine::Camera::getOldViewMatrix()
{
	return oldViewMatrix;
}

float Engine::Camera::getFOV()
{
	return fovy;
}