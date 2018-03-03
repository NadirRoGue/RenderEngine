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