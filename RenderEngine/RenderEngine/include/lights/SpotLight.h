#pragma once

#include "Light.h"

namespace Engine
{
	typedef struct SpotLightData
	{
		float position[4];
		float direction[4]; // 0 - 2: direction, 3: aperture
		float color[4];
		float attenuation[4];
		float kFactors[4];  // 0 - 2: Ka, Kd, Ks
	} SpotLightData;

	class SpotLight : public Light
	{
	private:
		SpotLightData shaderData;

		glm::mat4 dirMatrix;
	public:
		SpotLight(std::string name);
		~SpotLight();

		const glm::mat4 & getDirModelMatrix() const;

		void setAttenuation(const glm::vec3 & att);
		void setPosition(const glm::vec3 & pos);
		void setDirection(const glm::vec3 & dir);
		void setApperture(float app);
		void setM(float m);
		void setColor(const glm::vec3 & color);
		void setKa(float a);
		void setKd(float d);
		void setKs(float s);

		SpotLightData & getData();

		void translate(const glm::vec3 & translation);
		void directTo(const glm::vec3 & dir);
	};
}