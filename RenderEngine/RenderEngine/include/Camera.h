/*
 * @author Nadir Román Guerrero
 * @email nadir.ro.gue@gmail.com
 */

#pragma once
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

	public:
		Camera(float n, float f, float fy, float fx);
		Camera(const Camera & other);
		~Camera();

		void onWindowResize(int width, int height);

		glm::mat4 & getProjectionMatrix();
		glm::mat4 & getViewMatrix();

		void translateView(glm::vec3 translation);
		void rotateView(glm::vec3 rotation);

	private:
		void initProjectionMatrix();
		void initViewMatrix();
		void updateViewMatrix();
	};
}