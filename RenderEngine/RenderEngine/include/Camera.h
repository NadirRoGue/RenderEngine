/*
 * @author Nadir Román Guerrero
 * @email nadir.ro.gue@gmail.com
 */

#pragma once
#define GLM_FORCE_RADIANS

#include <glm\glm.hpp>

namespace Engine
{
	class Camera
	{
	private:
		float nearPlane;
		float farPlane;

		float fovy;
		float fovx;
		glm::mat4 projMatrix;

		glm::vec3 translation;
		glm::vec3 rotation;
		glm::mat4 viewMatrix;

		glm::vec3 forward;

	public:
		Camera(float n, float f, float fy, float fx);
		Camera(const Camera & other);
		~Camera();

		void onWindowResize(int width, int height);

		glm::mat4 & getProjectionMatrix();
		glm::mat4 & getViewMatrix();

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