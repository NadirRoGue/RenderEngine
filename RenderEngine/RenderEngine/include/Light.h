/*
* @author Nadir Román Guerrero
* @email nadir.ro.gue@gmail.com
*/

#pragma once

#include <glm\glm.hpp>
#include <string>

namespace Engine
{
	class Light
	{
	private:
		bool enabled;
		std::string name;

	protected:

		glm::mat4 modelMatrix;

		float ambientIntensity[3];
		float diffuseIntensity[3];
		float specularIntensity[3];
	public:
		Light(std::string name);
		~Light();

		const std::string & getName() const;

		void setEnabled(bool val);
		const bool isEnabled() const;

		void setAmbientIntensity(float r, float g, float b);
		void setDiffuseIntensity(float r, float g, float b);
		void setSpecularIntensity(float r, float g, float b);

		const float * getAmbientIntensity() const;
		const float * getDiffuseIntensity() const;
		const float * getSpecularIntensity() const;

		void modifyIntensity(float deltaR, float deltaG, float deltaB);

		virtual void translate(const glm::vec3 & translation) = 0;

		const glm::mat4 & getModelMatrix() const;
	};
}