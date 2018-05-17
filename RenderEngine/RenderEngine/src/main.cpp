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
#include "renderers/DeferredRenderer.h"
#include "Terrain.h"
#include "skybox/SkyBox.h"

#include "StorageTable.h"
#include "datatables/ProgramTable.h"
#include "datatables/MeshTable.h"
#include "datatables/TextureTable.h"
#include "datatables/DeferredObjectsTable.h"
#include "LightBufferManager.h"

#include "defaultobjects/Cube.h"
#include "defaultobjects/Plane.h"
#include "defaultobjects/TreeShapes.h"

#include "programs/ProceduralTerrainProgram.h"
#include "programs/ProceduralWaterProgram.h"
#include "programs/TreeProgram.h"
#include "programs/SkyProgram.h"
#include "postprocessprograms/DeferredShadingProgram.h"
#include "postprocessprograms/SSAAProgram.h"
#include "postprocessprograms/BloomProgram.h"

#include "inputhandlers/keyboardhandlers/CameraMovementHandler.h"
#include "inputhandlers/keyboardhandlers/ToggleUIHandler.h"
#include "inputhandlers/mousehandlers/CameraRotationHandler.h"

#include "animations/CameraBezier.h"

#include "datatables/VegetationTable.h"

#include "WorldConfig.h"

void initOpenGL();
void initScene();
void initTables();
void initSceneObj();
void initHandlers();
void initRenderEngine();
void destroy();

Engine::PostProcessChainNode * createBloomNode();
Engine::PostProcessChainNode * createSSAANode();


/**
* @author Nadir Román Guerrero
* @email nadir.ro.gue@gmail.com
*/

int main(int argc, char** argv)
{
	std::locale::global(std::locale("spanish")); // acentos ;)

	initOpenGL();
	initTables();
	initScene();
	initRenderEngine();
	initSceneObj();
	initHandlers();
	
	Engine::Window::WindowManager::getInstance().getWindowToolkit()->mainLoop();

	destroy();

	system("Pause");

	return 0;
}
	
// ======================================================================
// ======================================================================

void initOpenGL()
{
	std::unique_ptr<Engine::Window::GLFWWindow> win = std::make_unique<Engine::Window::GLFWWindow>("Procedural World", 0, 30, 1024, 1024);
	win->setOGLVersion(4, 1);
	win->setContextProfile(GLFW_OPENGL_CORE_PROFILE);

	Engine::Window::WindowManager::getInstance().setToolkit(std::move(win));
}

void initScene()
{
	Engine::Camera * camera = new Engine::Camera(0.5f, 1000.0f, 45.0f, 45.0f);
	camera->translateView(glm::vec3(0, -6, 0));
	//camera->translateView(glm::vec3(30.0f, -5.0f, -50.0f));
	//camera->rotateView(glm::vec3(glm::radians(30.0f), glm::radians(60.0f), 0.0f));

	Engine::Scene * scene = new Engine::Scene();
	scene->setCamera(camera);

	Engine::SceneManager::getInstance().registerScene("scene_0", scene);
	Engine::SceneManager::getInstance().activateScene("scene_0");
}

void initTables()
{
	Engine::TableManager::getInstance().registerTable(&Engine::MeshTable::getInstance());
	Engine::TableManager::getInstance().registerTable(&Engine::TextureTable::getInstance());
	Engine::TableManager::getInstance().registerTable(&Engine::ProgramTable::getInstance());
	Engine::TableManager::getInstance().registerTable(&Engine::GPU::LightBufferManager::getInstance());
	Engine::TableManager::getInstance().registerTable(&Engine::DeferredObjectsTable::getInstance());

	// Texture table
	Engine::TextureTable::getInstance().checkForAnisotropicFilterSupport();

	// Shader table
	Engine::ProgramTable::getInstance().registerProgramFactory(Engine::PostProcessProgram::PROGRAM_NAME, new Engine::PostProcessProgramFactory());
	Engine::ProgramTable::getInstance().registerProgramFactory(Engine::DeferredShadingProgram::PROGRAM_NAME, new Engine::DeferredShadingProgramFactory());
	Engine::ProgramTable::getInstance().registerProgramFactory(Engine::SSAAProgram::PROGRAM_NAME, new Engine::SSAAProgramFactory());
	Engine::ProgramTable::getInstance().registerProgramFactory(Engine::ProceduralTerrainProgram::PROGRAM_NAME, new Engine::ProceduralTerrainProgramFactory());
	Engine::ProgramTable::getInstance().registerProgramFactory(Engine::ProceduralWaterProgram::PROGRAM_NAME, new Engine::ProceduralWaterProgramFactory());
	Engine::ProgramTable::getInstance().registerProgramFactory(Engine::SkyProgram::PROGRAM_NAME, new Engine::SkyProgramFactory());
	Engine::ProgramTable::getInstance().registerProgramFactory(Engine::BloomProgram::PROGRAM_NAME, new Engine::BloomProgramFactory());
	Engine::ProgramTable::getInstance().registerProgramFactory(Engine::TreeProgram::PROGRAM_NAME, new Engine::TreeProgramFactory());
	
	// Mesh table
	Engine::MeshTable::getInstance().addMeshToCache("cube", Engine::CreateCube());
	Engine::MeshTable::getInstance().addMeshToCache("plane", Engine::CreatePlane());
	Engine::MeshTable::getInstance().addMeshToCache("trunk", Engine::CreateTrunk());
	Engine::MeshTable::getInstance().addMeshToCache("leaf", Engine::createLeaf());
}

void initSceneObj()
{
	Engine::Scene * scene = Engine::SceneManager::getInstance().getActiveScene();

	// Add directional light
	Engine::DirectionalLight * dl = new Engine::DirectionalLight();
	dl->setColor(Engine::Settings::lightColor);
	dl->setDirection(Engine::Settings::lightDirection);
	scene->setDirectionalLight(dl);

	// Set a terrain
	scene->setTerrain(new Engine::Terrain(5.0f, 15));
	// Set a skybox
	scene->setSkybox(new Engine::SkyBox());
	
	// Configure clear color (used for far objects fog)
	scene->setClearColor(glm::vec3(0.8, 0.85, 1));
	
	scene->initialize();

	// Trigger FBO resize according to screen size
	Engine::RenderManager::getInstance().doResize(1024, 1024);
}

void initHandlers()
{
	Engine::Scene * scene = Engine::SceneManager::getInstance().getActiveScene();

	// W A S D movement
	Engine::CameraMovement * cm = new Engine::CameraMovement(scene->getCamera());
	// UI visibility toggle
	Engine::ToggleUIHandler * uiHandler = new Engine::ToggleUIHandler();
	Engine::KeyboardHandlersTable * handlers = scene->getKeyboardHandler();
	handlers->registerHandler(cm);
	handlers->registerHandler(uiHandler);
	
	Engine::CameraBezier * camBezier = new Engine::CameraBezier(scene->getCamera(), glm::vec3(100,10,100), 50.0f, 3.0f);
	scene->getAnimationHandler()->registerAnimation(camBezier);

	// Mouse pitch & yaw
	Engine::CameraRotationHandler * camMotion = new Engine::CameraRotationHandler("camera_motion", Engine::SceneManager::getInstance().getActiveScene()->getCamera());
	Engine::MouseEventManager * mouseHandler = scene->getMouseHandler();
	mouseHandler->registerMouseMotionHandler(camMotion);
}

void initRenderEngine()
{
	Engine::DeferredRenderer * dr = new Engine::DeferredRenderer();
	dr->addPostProcess(createBloomNode());

	Engine::RenderManager::getInstance().setRenderer(dr);
	Engine::RenderManager::getInstance().doResize(1024, 1024);
}

void destroy()
{
	Engine::TableManager::getInstance().cleanUp();
}

// ==========================================================================

Engine::PostProcessChainNode * createSSAANode()
{
	Engine::PostProcessChainNode * node = new Engine::PostProcessChainNode;
	//Engine::Program * gaussSource = Engine::ProgramTable::getInstance().getProgramByName(Engine::SSAAProgram::PROGRAM_NAME);
	node->postProcessProgram = Engine::ProgramTable::getInstance().getProgramByName(Engine::SSAAProgram::PROGRAM_NAME);
	//new Engine::SSAAProgram(*dynamic_cast<Engine::SSAAProgram*>(gaussSource));

	node->renderBuffer = new Engine::DeferredRenderObject(2, true);
	node->renderBuffer->addColorBuffer(0, GL_RGBA8, GL_RGBA, GL_FLOAT, 500, 500, "", GL_LINEAR);
	node->renderBuffer->addColorBuffer(1, GL_RGBA8, GL_RGBA, GL_FLOAT, 500, 500, "", GL_LINEAR);
	node->renderBuffer->addDepthBuffer24(500, 500);
	node->callBack = 0;

	Engine::Mesh * mi = Engine::MeshTable::getInstance().getMesh("plane");
	if (mi != 0)
	{
		node->postProcessProgram->configureMeshBuffers(mi);
		node->obj = new Engine::PostProcessObject(mi);
	}

	return node;
}

Engine::PostProcessChainNode * createBloomNode()
{
	Engine::PostProcessChainNode * node = new Engine::PostProcessChainNode;
	
	node->postProcessProgram = Engine::ProgramTable::getInstance().getProgramByName(Engine::BloomProgram::PROGRAM_NAME);

	node->renderBuffer = new Engine::DeferredRenderObject(3, true);
	node->renderBuffer->addColorBuffer(0, GL_RGBA8, GL_RGBA, GL_FLOAT, 500, 500, "", GL_LINEAR);
	node->renderBuffer->addColorBuffer(1, GL_RGBA8, GL_RGBA, GL_FLOAT, 500, 500, "", GL_LINEAR);
	node->renderBuffer->addColorBuffer(2, GL_RGBA8, GL_RGBA, GL_FLOAT, 500, 500, "", GL_LINEAR);
	node->renderBuffer->addDepthBuffer24(500, 500);
	node->callBack = 0;

	Engine::Mesh * mi = Engine::MeshTable::getInstance().getMesh("plane");
	if (mi != 0)
	{
		node->postProcessProgram->configureMeshBuffers(mi);
		node->obj = new Engine::PostProcessObject(mi);
	}

	return node;
}