/*
* @author Nadir Román Guerrero
* @email nadir.ro.gue@gmail.com
*/
#pragma once

#include <glm/glm.hpp>

#include "WorldConfig.h"
#include "Camera.h"
#include "Program.h"

namespace Engine
{
	// Parent class of all terrain components that give a common acess interface
	class TerrainComponent
	{
	protected:
		float scale;
		bool isShadowable;
	public:
		TerrainComponent()
		{

		}

		void init(float scale, bool shadowable)
		{
			this->scale = scale;
			this->isShadowable = shadowable;
			initialize();
		}

		bool castShadows()
		{
			return isShadowable;
		}

		virtual void initialize()
		{

		}

		virtual unsigned int getRenderRadius() = 0;
		
		virtual Program * getActiveShader()
		{
			return NULL;
		}

		virtual Program * getShadowMapShader()
		{
			return NULL;
		}

		virtual void preRenderComponent()
		{

		}

		virtual void renderComponent(int i, int j, Engine::Camera * camera)
		{

		}

		virtual void postRenderComponent()
		{

		}

		virtual void renderShadow(const glm::mat4 & projection, int i, int j, Engine::Camera * cam)
		{

		}

		virtual void notifyRenderModeChange(Engine::RenderMode mode)
		{

		}
	};
}