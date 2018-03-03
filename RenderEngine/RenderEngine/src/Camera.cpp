/*
* @author Nadir Román Guerrero
* @email nadir.ro.gue@gmail.com
*/

#define GLM_FORCE_RADIANS

#include "Camera.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>

#include <math.h>

using namespace Engine;

Camera::Camera(float n, float f, float fy, float fx) :nearPlane(n), farPlane(f), fovy(fy), fovx(fx)
{
	// Initialize projection matrix
	initProjectionMatrix();
	initViewMatrix();
}

Camera::Camera(const Camera & other)
{
	farPlane = other.farPlane;
	nearPlane = other.nearPlane;
	fovx = other.fovx;
	fovy = other.fovy;
	projMatrix = other.projMatrix;
	translation = other.translation;
	rotation = other.rotation;
	viewMatrix = other.viewMatrix;
}

Camera::~Camera() 
{
}

void Camera::initProjectionMatrix()
{
	projMatrix = glm::mat4(0.0f);
	projMatrix[0].x = 1.0f / tan(fovx*3.14159f / 180.0f);
	projMatrix[1].y = 1.0f / tan(fovy*3.14159f / 180.0f);
	projMatrix[2].z = -(farPlane + nearPlane) / (farPlane - nearPlane);
	projMatrix[3].z = -2 * nearPlane*farPlane / (farPlane - nearPlane);
	projMatrix[2].w = -1.0;
}

void Camera::initViewMatrix()
{
	updateViewMatrix();
}

void Camera::translateView(glm::vec3 t)
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

void Camera::rotateView(glm::vec3 rot)
{
	rotation += rot;
	updateViewMatrix();
}

void Camera::updateViewMatrix()
{
	glm::mat4 identity(1.0f);

	glm::quat yawQ = glm::quat(glm::vec3(0.0f, rotation.y, 0.0f));
	yawQ = glm::normalize(yawQ);
	glm::mat4 yawMat = glm::mat4_cast(yawQ);

	glm::quat pitch = glm::quat(glm::vec3(rotation.x, 0.0f, 0.0f));
	pitch = glm::normalize(pitch);
	glm::mat4 pitchMat = glm::mat4_cast(pitch);

	viewMatrix = pitchMat * yawMat * glm::translate(identity, translation);
}

void Camera::onWindowResize(int width, int height)
{
	float fWidth = (float)width;
	float fHeight = (float)height;

	float hAngle = fovx;
	float vAngle = fovy;

	if (width > height)
	{
		vAngle *= (1.0f / (fWidth / fHeight));
	}
	else if (height > width)
	{
		hAngle *= (1.0f / (fHeight / fWidth));
	}

	projMatrix = glm::mat4(0.0f);
	projMatrix[0].x = 1.0f / tan(hAngle*3.14159f / 180.0f);
	projMatrix[1].y = 1.0f / tan(vAngle*3.14159f / 180.0f);
	projMatrix[2].z = -(farPlane + nearPlane) / (farPlane - nearPlane);
	projMatrix[3].z = -2.0f * nearPlane*farPlane / (farPlane - nearPlane);
	projMatrix[2].w = -1.0f;
}

glm::mat4 & Camera::getProjectionMatrix()
{
	return projMatrix;
}

glm::mat4 & Camera::getViewMatrix()
{
	return viewMatrix;
}