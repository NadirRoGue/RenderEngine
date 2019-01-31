/*
 * @author Nadir Román Guerrero
 * @email nadir.ro.gue@gmail.com
 */

#pragma once
#define GLM_FORCE_RADIANS

#include <glm\glm.hpp>

namespace Engine
{
	// Represents a camera, with its one view and projection matrices
	class Camera
	{
	private:
		float nearPlane;
		float farPlane;

		float fovy;
		glm::mat4 projMatrix;

		glm::vec3 translation;
		glm::vec3 rotation;
		glm::mat4 viewMatrix;

		glm::mat4 invViewMatrix;
		glm::mat4 transposeInvViewMatrix;
		glm::mat4 oldViewMatrix;

		glm::vec3 forward;

	public:
		Camera(float n, float f, float fov);
		Camera(const Camera & other);
		~Camera();

		void onWindowResize(int width, int height);

		float getFOV();

		glm::mat4 & getProjectionMatrix();
		glm::mat4 & getViewMatrix();
		glm::mat4 & getInvViewMatrix();
		glm::mat4 & getTransposeInvViewMatrix();
		glm::mat4 & getOldViewMatrix();

		void setPosition(glm::vec3 pos);
		void translateView(glm::vec3 translation);
		void rotateView(glm::vec3 rotation);

		void setLookAt(glm::vec3 eye, glm::vec3 target);

		const glm::vec3 & getForwardVector() const
		{
			return forward;
		}

		const glm::vec3 & getPosition() const
		{
			return translation;
		}

		const glm::vec3 & getRotation() const
		{
			return rotation;
		}

	private:
		void initProjectionMatrix();
		void initViewMatrix();
		void updateViewMatrix();
	};
}