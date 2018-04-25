#pragma once

#include "Light.h"

namespace Engine
{
	class PointLight : public Light
	{
	private:
		float attenuationFactor[3];
	public:
		PointLight(std::string name, glm::vec3 pos);
		PointLight(std::string name, glm::vec3 pos, glm::vec3 attenuation);
		~PointLight();

		const glm::vec3 getPositioin() const;
		const float * getAttenuationFactor() const;
		void translate(const glm::vec3 & translation);
	};
}