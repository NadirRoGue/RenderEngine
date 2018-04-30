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

Engine::DeferredRenderObject * createForwardBufferWithDepth();
Engine::PostProcessChainNode * createDeferredShadingNode();
Engine::PostProcessChainNode * createSSAANode();
Engine::PostProcessChainNode * createEndNode();

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
	Engine::Camera * camera = new Engine::Camera(0.1f, 1000.0f, 45.0f, 45.0f);
	camera->translateView(glm::vec3(30.0f, -5.0f, -50.0f));
	camera->rotateView(glm::vec3(glm::radians(30.0f), glm::radians(60.0f), 0.0f));

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
	Engine::ProgramTable::getInstance().registerProgramFactory(Engine::SSAAProgram::PROGRAM_NAME, new Engine::SSAAProgramFactory());
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

	Engine::DirectionalLight * dl = new Engine::DirectionalLight();
	dl->setColor(1.0f, 1.0f, 1.0f);
	dl->setDirection(glm::vec3(1, 1, 0));
	scene->setDirectionalLight(dl);

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
	Engine::DeferredRenderer * dr = new Engine::DeferredRenderer();
	dr->setForwardPassBuffers(createForwardBufferWithDepth());
	dr->addPostProcess(createDeferredShadingNode());
	//dr->addPostProcess(createSSAANode());
	dr->setFinalPostProcess(createEndNode());
	Engine::RenderManager::getInstance().setRenderer(dr);
	Engine::RenderManager::getInstance().doResize(1024, 1024);
}

void destroy()
{
	Engine::TableManager::getInstance().cleanUp();
}

// ==========================================================================

Engine::DeferredRenderObject * createForwardBufferWithDepth()
{
	// G-Buffers
	Engine::DeferredRenderObject * buffer = new Engine::DeferredRenderObject(5, true);
	buffer->addColorBuffer(0, GL_RGBA8, GL_RGBA, GL_FLOAT, 500, 500, GL_LINEAR);			// Color
	buffer->addColorBuffer(1, GL_RGB32F, GL_RGBA, GL_UNSIGNED_BYTE, 500, 500, GL_LINEAR);	// Normal
	buffer->addColorBuffer(2, GL_RGBA8, GL_RGBA, GL_FLOAT, 500, 500, GL_LINEAR);			// Specular
	buffer->addColorBuffer(3, GL_RGBA8, GL_RGBA, GL_FLOAT, 500, 500, GL_LINEAR);			// Emission
	buffer->addColorBuffer(4, GL_RGB32F, GL_RGBA, GL_UNSIGNED_BYTE, 500, 500, GL_LINEAR);	// Position
	buffer->addDepthBuffer24(500, 500);														// Depth
	return buffer;
}

Engine::PostProcessChainNode * createDeferredShadingNode()
{
	Engine::PostProcessChainNode * node = new Engine::PostProcessChainNode;
	Engine::Program * sourcePostProcess = Engine::ProgramTable::getInstance().getProgramByName(Engine::DeferredShadingProgram::PROGRAM_NAME);

	Engine::DeferredRenderObject * buffer = new Engine::DeferredRenderObject(2, true);
	buffer->addColorBuffer(0, GL_RGBA8, GL_RGBA, GL_FLOAT, 500, 500, GL_LINEAR);
	buffer->addColorBuffer(1, GL_RGBA8, GL_RGBA, GL_FLOAT, 500, 500, GL_LINEAR);
	buffer->addDepthBuffer24(500, 500);

	node->postProcessProgram = new Engine::DeferredShadingProgram(*dynamic_cast<Engine::DeferredShadingProgram*>(sourcePostProcess));
	node->renderBuffer = buffer;
	node->callBack = 0;

	Engine::Mesh * mi = Engine::MeshTable::getInstance().getMesh("plane");
	if (mi != 0)
	{
		node->postProcessProgram->configureMeshBuffers(mi);
		node->obj = new Engine::PostProcessObject(mi);
	}

	return node;
}

Engine::PostProcessChainNode * createSSAANode()
{
	Engine::PostProcessChainNode * node = new Engine::PostProcessChainNode;
	Engine::Program * gaussSource = Engine::ProgramTable::getInstance().getProgramByName(Engine::SSAAProgram::PROGRAM_NAME);
	node->postProcessProgram = new Engine::SSAAProgram(*dynamic_cast<Engine::SSAAProgram*>(gaussSource));

	node->renderBuffer = new Engine::DeferredRenderObject(2, false);
	node->renderBuffer->addColorBuffer(0, GL_RGBA8, GL_RGBA, GL_FLOAT, 500, 500, GL_LINEAR);
	node->renderBuffer->addColorBuffer(1, GL_RGBA8, GL_RGBA, GL_FLOAT, 500, 500, GL_LINEAR);
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

Engine::PostProcessChainNode * createEndNode()
{
	Engine::PostProcessChainNode * node = new Engine::PostProcessChainNode;
	Engine::Program * sourcePostProcess = Engine::ProgramTable::getInstance().getProgramByName(Engine::PostProcessProgram::PROGRAM_NAME);
	node->postProcessProgram = new Engine::PostProcessProgram(*dynamic_cast<Engine::PostProcessProgram*>(sourcePostProcess));
	node->renderBuffer = 0;

	Engine::Mesh * mi = Engine::MeshTable::getInstance().getMesh("plane");
	if (mi != NULL)
	{
		node->postProcessProgram->configureMeshBuffers(mi);
		node->obj = new Engine::PostProcessObject(mi);
	}

	node->callBack = 0;

	return node;
}