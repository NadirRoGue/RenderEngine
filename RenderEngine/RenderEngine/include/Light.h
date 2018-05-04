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
		unsigned int bufferIndex;
	protected:
		glm::mat4 modelMatrix;
		bool update;
	public:
		Light(std::string name);
		~Light();

		const std::string & getName() const;

		void setEnabled(bool val);
		const bool isEnabled() const;

		virtual void setColor(const glm::vec3 & color) = 0;
		virtual void setKa(float a) = 0;
		virtual void setKd(float d) = 0;
		virtual void setKs(float s) = 0;

		virtual void translate(const glm::vec3 & translation) = 0;

		const glm::mat4 & getModelMatrix() const;

		bool requiresUpdate();
		void clearUpdateFlag();

		void setBufferIndex(unsigned int bi);
		unsigned int getBufferIndex();
	};
}