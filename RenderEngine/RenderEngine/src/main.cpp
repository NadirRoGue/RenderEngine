#include <windows.h>


#include "windowmanagers/GLUTWindow.h"
#include "windowmanagers/GLFWWindow.h"
#include "windowmanagers/WindowManager.h"

#include "userinterfaces/WorldControllerUI.h"

#define SOLVE_FGLUT_WARNING
#include <gl/freeglut.h> 
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <iostream>


#include "Scene.h"
#include "Renderer.h"
#include "PostProcessManager.h"
#include "Terrain.h"

#include "StorageTable.h"
#include "datatables/ProgramTable.h"
#include "datatables/MeshTable.h"
#include "datatables/TextureTable.h"

#include "defaultobjects/Cube.h"
#include "defaultobjects/Plane.h"

#include "programs/ProceduralTerrainProgram.h"
#include "programs/ProceduralWaterProgram.h"
#include "postprocessprograms/DeferredShadingProgram.h"
#include "postprocessprograms/SSAAProgram.h"

#include "inputhandlers/keyboardhandlers/CameraMovementHandler.h"
#include "inputhandlers/keyboardhandlers/ToggleUIHandler.h"
#include "inputhandlers/mousehandlers/CameraRotationHandler.h"



void initOpenGL();
void initScene();
void initTables();
void initSceneObj();
void initHandlers();
void initRenderEngine();

void destroy();

/**
* @author Nadir Román Guerrero
* @email nadir.ro.gue@gmail.com
*/

int main(int argc, char** argv)
{
	std::locale::global(std::locale("spanish")); // acentos ;)

	initOpenGL();
	initScene();
	initTables();
	initRenderEngine();
	initSceneObj();
	initHandlers();
	
	Engine::Window::WindowManager::getInstance().getWindowToolkit()->mainLoop();

	destroy();

	return 0;
}
	
// ======================================================================
// ======================================================================

void initOpenGL()
{
	//std::unique_ptr<Engine::Window::GLUTWindow> win = std::make_unique<Engine::Window::GLUTWindow>("Procedural World", 0, 0, 1024, 1024);
	std::unique_ptr<Engine::Window::GLFWWindow> win = std::make_unique<Engine::Window::GLFWWindow>("Procedural World", 0, 30, 1024, 1024);
	win->setOGLVersion(4, 1);
	//win->setContextProfile(GLUT_CORE_PROFILE);
	win->setContextProfile(GLFW_OPENGL_CORE_PROFILE);
	//win->setDisplayFlags(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);

	Engine::Window::WindowManager::getInstance().setToolkit(std::move(win));
}

void initScene()
{
	Engine::TableManager::getInstance().registerTable(&Engine::MeshTable::getInstance());
	Engine::TableManager::getInstance().registerTable(&Engine::TextureTable::getInstance());
	Engine::TableManager::getInstance().registerTable(&Engine::ProgramTable::getInstance());


	Engine::Camera * camera = new Engine::Camera(0.1f, 1000.0f, 45.0f, 45.0f);
	camera->translateView(glm::vec3(30.0f, -5.0f, -50.0f));
	camera->rotateView(glm::vec3(glm::radians(30.0f), glm::radians(60.0f), 0.0f));

	// Parameters: name, direction
	Engine::DirectionalLight * dl = new Engine::DirectionalLight("directional_light", glm::vec3(1,1,0));
	dl->setAmbientIntensity(0.2f, 0.2f, 0.2f);
	dl->setDiffuseIntensity(1.0f, 1.0f, 1.0f);
	dl->setSpecularIntensity(1.0f, 1.0f, 1.0f);

	Engine::Scene * scene = new Engine::Scene();
	scene->setCamera(camera);
	scene->setDirectionalLight(dl);

	Engine::SceneManager::getInstance().registerScene("scene_0", scene);
	Engine::SceneManager::getInstance().activateScene("scene_0");
}

void initTables()
{
	// Texture table
	Engine::TextureTable::getInstance().checkForAnisotropicFilterSupport();
	Engine::CubemapLoadData data;
	data.rightFace = "img/skyboxes/Daylight_Box_Right.png";
	data.leftFace = "img/skyboxes/Daylight_Box_Left.png";
	data.topFace = "img/skyboxes/Daylight_Box_Top.png";
	data.bottomFace = "img/skyboxes/Daylight_Box_Bottom.png";
	data.frontFace = "img/skyboxes/Daylight_Box_Front.png";
	data.backFace = "img/skyboxes/Daylight_Box_Back.png";
	Engine::TextureTable::getInstance().cacheCubemapTexture(data, "DaylightCubemap");

	// Shader table
	Engine::ProgramTable::getInstance().registerProgramFactory(Engine::PostProcessProgram::PROGRAM_NAME, new Engine::PostProcessProgramFactory());
	Engine::ProgramTable::getInstance().registerProgramFactory(Engine::DeferredShadingProgram::PROGRAM_NAME, new Engine::DeferredShadingProgramFactory());
	Engine::ProgramTable::getInstance().registerProgramFactory(Engine::ProceduralTerrainProgram::PROGRAM_NAME, new Engine::ProceduralTerrainProgramFactory());
	Engine::ProgramTable::getInstance().registerProgramFactory(Engine::ProceduralWaterProgram::PROGRAM_NAME, new Engine::ProceduralWaterProgramFactory());
	Engine::ProgramTable::getInstance().registerProgramFactory(Engine::SkyProgram::PROGRAM_NAME, new Engine::SkyProgramFactory());

	// Mesh table
	Engine::MeshTable::getInstance().addMeshToCache("cube", Engine::CreateCube());
	Engine::MeshTable::getInstance().addMeshToCache("plane", Engine::CreatePlane());
}

void initSceneObj()
{
	Engine::Scene * scene = Engine::SceneManager::getInstance().getActiveScene();

	scene->setTerrain(new Engine::Terrain(5.0f, 20));
	scene->setSkybox(new Engine::SkyBox(Engine::TextureTable::getInstance().instantiateTexture("DaylightCubemap")));
	
	scene->setClearColor(glm::vec3(0.85, 0.97, 1));
}

void initHandlers()
{
	// W A S D movement
	Engine::CameraMovement * cm = new Engine::CameraMovement(Engine::SceneManager::getInstance().getActiveScene()->getCamera());
	Engine::ToggleUIHandler * uiHandler = new Engine::ToggleUIHandler();
	Engine::KeyboardHandlersTable * handlers = Engine::SceneManager::getInstance().getActiveScene()->getKeyboardHandler();
	handlers->registerHandler(cm);
	handlers->registerHandler(uiHandler);

	// Mouse pitch & yaw
	Engine::CameraRotationHandler * camMotion = new Engine::CameraRotationHandler("camera_motion", Engine::SceneManager::getInstance().getActiveScene()->getCamera());
	Engine::MouseEventManager * mouseHandler = Engine::SceneManager::getInstance().getActiveScene()->getMouseHandler();
	mouseHandler->registerMouseMotionHandler(camMotion);
}

void initRenderEngine()
{
	Engine::DeferredRenderer * dr = Engine::TestImplementation::createDeferredRendererWithDoF();
	Engine::RenderManager::getInstance().setRenderer(dr);
	Engine::RenderManager::getInstance().doResize(1024, 1024);
}

void destroy()
{
	Engine::TableManager::getInstance().cleanUp();
}