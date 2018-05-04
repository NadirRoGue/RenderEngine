#pragma once

#include "postprocessprograms/SSAAProgram.h"

#include "DeferredRenderObject.h"

namespace Engine
{
	class BloomProgram : public SSAAProgram
	{
	public:
		const static std::string PROGRAM_NAME;
	private:
		typedef struct SwitchableBuffers
		{
			DeferredRenderObject * pass;
			TextureInstance * color;
			TextureInstance * depth;
			TextureInstance * emissive;
		} SwitchableBuffers;
	private:
		SwitchableBuffers buf[2];
		unsigned int passes;

		unsigned int uHorizontal;
		unsigned int uBlend;
	public:
		BloomProgram(std::string name, unsigned long long parameters);
		BloomProgram(const BloomProgram & other);

		void configureProgram();
		void onRenderObject(const Object * obj, const glm::mat4 & view, const glm::mat4 &proj);
	};

	// ============================================================

	class BloomProgramFactory : public ProgramFactory
	{
	protected:
		Program * createProgram(unsigned long long parameters);
	};
}