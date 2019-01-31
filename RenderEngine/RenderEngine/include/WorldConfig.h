/*
* @author Nadir Román Guerrero
* @email nadir.ro.gue@gmail.com
*/
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

	// Holds all the system configuration, given access anywhere in the engine
	// where it is needed
	class Settings
	{
	public:
		static unsigned int worldSeed;

		static unsigned int drawingMethod;
		static unsigned int travelMethod;

		static float lightFactor;
		static glm::vec3 lightColor;
		static glm::vec3 realLightColor;
		static glm::vec3 lightDirection;

		static float worldTileScale;
		static unsigned int worldRenderRadius;

		static glm::vec3 windDirection;
		static float windStrength;

		static float terrainAmplitude;
		static float terrainFrecuency;
		static float terrainScale;
		static unsigned int terrainOctaves;
		static float vegetationMaxHeight;
		static float grassCoverage;
		static glm::vec3 grassColor;
		static glm::vec3 sandColor;
		static glm::vec3 rockColor;

		static glm::vec3 skyZenitColor;
		static glm::vec3 skyHorizonColor;

		static glm::vec3 waterColor;
		static float waterSpeed;
		static float waterHeight;

		static float cloudType;
		static float coverageMultiplier;
		static float innerSphereRadius;
		static float outerSphereRadius;
		static float sphereYOffset;
		static float cloudMaxRenderDistance;
		static float cloudTopOffset;
		static float weatherTextureScale;
		static float baseNoiseScale;
		static float highFrequencyNoiseScale;
		static float highFrequencyNoiseUVScale;
		static float highFrequencyNoiseHScale;
		static glm::vec3 cloudColor;

		static float hdrExposure;
		static float hdrGamma;
		static glm::vec3 hdrTint;

		static float dofFocalDist;
		static float dofMaxDist;

		static float godRaysExposure;
		static float godRaysDensity;
		static float godRaysDecay;
		static float godRaysWeight;

		static bool showUI;
	public:
		static void update();
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