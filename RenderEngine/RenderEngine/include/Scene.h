/*
* @author Nadir Román Guerrero
* @email nadir.ro.gue@gmail.com
*/

#pragma once

#include "Camera.h"
#include "Program.h"
#include "Object.h"
#include "Light.h"

#include "KeyboardHandler.h"
#include "MouseHandler.h"
#include "Animation.h"
#include "Terrain.h"
#include "skybox/AbstractSkyBox.h"

#include <map>
#include <list>

namespace Engine
{
	class ProgramRenderables
	{
	public:
		Program * program;
		std::map<unsigned int, std::list<Object *>> objects;

		ProgramRenderables(Program * prog);
	};

	class Scene
	{
	private:
		KeyboardHandlersTable * keyboardHandlers;
		MouseEventManager * mouseHandlers;
		AnimationTable * animations;

		Camera * camera;

		std::map<std::string, ProgramRenderables *> renders;

		DirectionalLight * directionalLight;
		std::map<std::string, PointLight *> pointLights;
		std::map<std::string, SpotLight *> spotLights;

		Terrain * terrain;
		AbstractSkyBox * skybox;

		glm::vec3 clearColor;

	public:
		Scene();
		~Scene();

		const std::map<std::string, ProgramRenderables *> & getObjects() const;

		void addPointLight(PointLight * pl);
		void addSpotLight(SpotLight * sl);
		void setDirectionalLight(DirectionalLight * dl);
		const std::map<std::string, PointLight *> & getPointLights() const;
		const std::map<std::string, SpotLight *> & getSpotLights() const;
		PointLight * getLightByName(std::string name);
		SpotLight * getSpotLightByName(std::string name);
		DirectionalLight * getDirectionalLight();
		
		void setTerrain(Terrain * terrain);
		Terrain * getTerrain();
		void setSkybox(AbstractSkyBox * skybox);
		AbstractSkyBox * getSkyBox();
		void setCamera(Camera * cam);
		Camera * getCamera();
		void addObject(Object * obj);

		void initialize();

		void setClearColor(glm::vec3 cc);
		const glm::vec3 & getClearColor() const;

		void onViewportResize(int width, int height);

		KeyboardHandlersTable * getKeyboardHandler() const;
		MouseEventManager * getMouseHandler() const;
		AnimationTable * getAnimationHandler() const;
	};

	// =====================================================================

	class SceneManager
	{
	private:
		static SceneManager * INSTANCE;

	public:
		static SceneManager & getInstance();

	private:

		std::map<std::string, Scene *> scenes;
		Scene * activeScene;

		SceneManager();

	public:
		~SceneManager();
		
		bool registerScene(std::string name, Scene * s);
		Scene * getScene(std::string name);
		Scene * getActiveScene();
		
		void activateScene(std::string name);
	};
}
