#pragma once

#include "Light.h"

namespace Engine
{
	class DirectionalLight : public Light
	{
	private:
		glm::vec3 direction;

	public:
		DirectionalLight(std::string name, glm::vec3 dir);
		~DirectionalLight();

		const glm::vec3 getDirection() const;

		void translate(const glm::vec3 & translation);
	};
}