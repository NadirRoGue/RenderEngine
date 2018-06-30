/**
* @author Nadir Román Guerrero
* @email nadir.ro.gue@gmail.com
*/
#pragma once

#include "skybox/AbstractSkyBox.h"

namespace Engine
{
	/**
	 * Dummy Skybox which renders nothing. Default skybox type
	 * used in the scene object unless replaced otherwise
	 */
	class DummySkyBox : public AbstractSkyBox
	{
	public:
		void render(Camera * camera)
		{
			// Do nothing
		}
	};
}