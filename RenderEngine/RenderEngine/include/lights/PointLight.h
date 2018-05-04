#pragma once

#include "Light.h"

namespace Engine
{
	typedef struct PointLightData
	{
		float position[4];
		float color[4];
		float attenuation[4];
		float kFactors[4];
	} PointLightData;

	class PointLight : public Light
	{
	private:
		PointLightData shaderData;
	public:
		PointLight(std::string name);
		~PointLight();

		void setPosition(const glm::vec3 & pos);
		void setAttenuation(const glm::vec3 & att);
		void setColor(const glm::vec3 & color);
		void setKa(float a);
		void setKd(float d);
		void setKs(float s);
		void translate(const glm::vec3 & translation);

		PointLightData & getData();
	};
}