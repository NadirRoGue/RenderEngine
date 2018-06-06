/**
* @author Nadir Román Guerrero
* @email nadir.ro.gue@gmail.com
*/

#pragma once

#include "Light.h"

namespace Engine
{
	// Data structure used to mirror shader's data strucutre
	typedef struct DirectionalLightData
	{
		float direction[4]; // light direction
		float color[4];		// light color
		float kFactors[4];	// 0->Ka, 1->Kd, 2->Ks
	} DirectionalLightData;

	/**
	 * Class to store and manipulate directional lights (1 per scene)
	 */
	class DirectionalLight : public Light
	{
	private:
		// Light direction
		glm::vec3 direction;
		// Shader-ready data
		DirectionalLightData shaderData;
	public:
		DirectionalLight(std::string name = "");
		~DirectionalLight();

		const glm::vec3 getDirection() const;

		// Will change light direction (directional lights have no position)
		void translate(const glm::vec3 & translation);

		void setDirection(const glm::vec3 & dir);
		void setColor(const glm::vec3 & color);
		void setKa(float a);
		void setKd(float d);
		void setKs(float s);

		DirectionalLightData & getData();
	};
}