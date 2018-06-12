/**
* @author Nadir Román Guerrero
* @email nadir.ro.gue@gmail.com
*/

#pragma once

#include "Light.h"

namespace Engine
{
	// Data strucutre to mirror shader data strucutre
	typedef struct SpotLightData
	{
		float position[4];	// light position
		float direction[4]; // 0 - 2: direction, 3: aperture
		float color[4];		// light color
		float attenuation[4];	// attenuation coeficents (0=constant,1=lineal,2=cuadratic)
		float kFactors[4];  // 0 - 2: Ka, Kd, Ks
	} SpotLightData;

	/**
	 * Class to hold and manage a spot light's data
	 */
	class SpotLight : public Light
	{
	private:
		// Shader-ready data structure
		SpotLightData shaderData;

		// Matrix to apply transformations to the spot direction
		glm::mat4 dirMatrix;
	public:
		SpotLight(std::string name);
		~SpotLight();

		// Returns the model matrix for the light direction
		const glm::mat4 & getDirModelMatrix() const;

		// Setters
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