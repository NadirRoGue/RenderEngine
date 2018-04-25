#pragma once

#include "PostProcessProgram.h"

namespace Engine
{
	class SSAAProgram : public PostProcessProgram
	{
	private:
		unsigned int uTexelSize;
	public:
		SSAAProgram(std::string name);
		SSAAProgram(const SSAAProgram & other);
		~SSAAProgram();

		void configureProgram();
		void onRenderObject(const Object * obj, const glm::mat4 & view, const glm::mat4 &proj);
	};
}