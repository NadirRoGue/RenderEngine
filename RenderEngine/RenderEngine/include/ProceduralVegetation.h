#pragma once

#include "Mesh.h"

namespace Engine
{
	class ProceduralVegetation
	{
	private:
		unsigned long seed;

	public:
		ProceduralVegetation(unsigned long seed);

		virtual Mesh * generate() = 0;
	};
}