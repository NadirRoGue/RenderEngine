/**
* @author Nadir Román Guerrero
* @email nadir.ro.gue@gmail.com
*/
#pragma once

#include "Camera.h"

namespace Engine
{
	/**
	 * Parent class of a skybox implementation. All skyboxes
	 * must inherit from this class as it is the type accepted
	 * by the scene object
	 */
	class AbstractSkyBox
	{
	public:
		virtual void render(Camera * camera) = 0;
	};
}