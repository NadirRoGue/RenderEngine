#pragma once

#include "Light.h"

namespace Engine
{
	typedef struct DirectionalLightData
	{
		float direction[4];
		float color[4];
		float kFactors[4];
	} DirectionalLightData;

	class DirectionalLight : public Light
	{
	private:
		glm::vec3 direction;
		DirectionalLightData shaderData;
	public:
		DirectionalLight(std::string name = "");
		~DirectionalLight();

		const glm::vec3 getDirection() const;
		void translate(const glm::vec3 & translation);

		void setDirection(const glm::vec3 & dir);
		void setColor(float r, float g, float b);
		void setKa(float a);
		void setKd(float d);
		void setKs(float s);

		DirectionalLightData & getData();
	};
}