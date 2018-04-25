#pragma once

#include "Light.h"

namespace Engine
{
	class SpotLight : public Light
	{
	private:
		glm::vec3 direction;
		float appertureAngle;
		float m;
		float attenuationFactor[3];

		glm::mat4 dirMatrix;
	public:
		SpotLight(std::string name, glm::vec3 pos, glm::vec3 dir, float apperture, float m);
		SpotLight(std::string name, glm::vec3 pos, glm::vec3 dir, float apperture, float m, glm::vec3 attenuation);
		~SpotLight();

		const glm::vec3 getPosition() const;
		const glm::vec3 getDirection() const;
		const float * getAttenuationFactor() const;
		const float getAppertureAngle() const;
		const float getM() const;

		const glm::mat4 & getDirModelMatrix() const;

		void translate(const glm::vec3 & translation);
		void directTo(const glm::vec3 & dir);
	};
}