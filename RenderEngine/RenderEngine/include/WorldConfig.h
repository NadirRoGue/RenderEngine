#pragma once

#define GLM_FORCE_RADIANS

#include "glm/glm.hpp"

#include <list>
#include "IRenderable.h"
#include "Light.h"

namespace Engine
{
	enum TravelMethod
	{
		TRAVEL_MANUAL,
		TRAVEL_BEZIER,
		TRAVEL_STRAIGHT
	};

	class Settings
	{
	public:
		static unsigned int worldSeed;

		static unsigned int drawingMethod;
		static unsigned int travelMethod;

		static glm::vec3 lightColor;
		static glm::vec3 lightDirection;

		static float terrainAmplitude;
		static float terrainFrecuency;
		static float terrainScale;
		static unsigned int terrainOctaves;

		static glm::vec3 waterColor;
		static float waterSpeed;
		static float waterHeight;

		static bool showUI;
	};

	class RenderableNotifier
	{
	private:
		static RenderableNotifier * INSTANCE;
	private:
		std::list<IRenderable *> renderables;
		std::list<Light*> lights;

		unsigned int previousDrawingMethod;
		glm::vec3 previousLightColor;
		glm::vec3 previousLightDir;
	private:
		RenderableNotifier();
	public:
		static RenderableNotifier & getInstance();
		
		void registerRenderable(IRenderable * renderable);
		void registerLight(Light * light);
		void checkUpdatedConfig();
	};
}