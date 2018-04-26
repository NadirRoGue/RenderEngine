#pragma once

#include "PostProcessProgram.h"

namespace Engine
{
	class PerlinGeneratorProgram : public PostProcessProgram
	{
	public:
		static std::string PROGRAM_NAME;
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
		PerlinGeneratorProgram(std::string name, unsigned long long params);
		PerlinGeneratorProgram(const PerlinGeneratorProgram & other);
		~PerlinGeneratorProgram();

		void configureProgram();
		void onRenderObject(const Object * obj, const glm::mat4 & view, const glm::mat4 &proj);
	};

	class PerlinGeneratorProgramFactory : public ProgramFactory
	{
	protected:
		Program * createProgram(unsigned long long parameters);
	};
}