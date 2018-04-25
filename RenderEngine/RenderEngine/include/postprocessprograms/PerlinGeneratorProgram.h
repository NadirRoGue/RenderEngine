#pragma once

#include "PostProcessProgram.h"

namespace Engine
{
	class PerlinGeneratorProgram : public PostProcessProgram
	{
	private:
		float scale;
		float amplitude;
		float frequency;
		float octaves;

		unsigned int perlinScale;
		unsigned int perlinAmplitude;
		unsigned int perlinFrequency;
		unsigned int perlinOctaves;
	public:
		PerlinGeneratorProgram(std::string name);
		PerlinGeneratorProgram(const PerlinGeneratorProgram & other);
		~PerlinGeneratorProgram();

		void configureProgram();
		void onRenderObject(const Object * obj, const glm::mat4 & view, const glm::mat4 &proj);
	};
}