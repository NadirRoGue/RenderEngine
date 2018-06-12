/**
* @author Nadir Román Guerrero
* @email nadir.ro.gue@gmail.com
*/

#pragma once

#include "Light.h"

namespace Engine
{
	// Structure to mirror shader data strucutre
	typedef struct PointLightData
	{
		float position[4];	// light position
		float color[4];		// light color
		float attenuation[4];	// attenuation factors (0=constant,1=lineal,2=cuadratic)
		float kFactors[4];		// 0=Ka,1=Kd,2=Ks
	} PointLightData;

	/**
	 * Class to hold and manage point light data
	 */
	class PointLight : public Light
	{
	private:
		// Shader-ready data strucutre
		PointLightData shaderData;
	public:
		PointLight(std::string name);
		~PointLight();

		// Setters
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