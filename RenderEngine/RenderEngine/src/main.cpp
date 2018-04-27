#include <windows.h>

#include <gl/glew.h>
#define SOLVE_FGLUT_WARNING
#include <gl/freeglut.h> 
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

#include <vector>

#include "StorageTable.h"
#include "Scene.h"
#include "datatables/ProgramTable.h"
#include "PostProcessProgram.h"
#include "datatables/MeshTable.h"
#include "datatables/TextureTable.h"
#include "postprocessprograms/DeferredShadingProgram.h"
#include "postprocessprograms/SSAAProgram.h"
#include "defaultobjects/Cube.h"
#include "defaultobjects/Plane.h"
#include "Texture.h"
#include "Light.h"
#include "KeyboardHandler.h"
#include "MouseHandler.h"
#include "Renderer.h"
#include "PostProcessManager.h"
#include "DeferredRenderObject.h"
#include "Terrain.h"

#include "programs/ProceduralWaterProgram.h"

#include "inputhandlers/keyboardhandlers/CameraMovementHandler.h"
#include "inputhandlers/mousehandlers/CameraRotationHandler.h"
#include "AnimImpl.h"


//////////////////////////////////////////////////////////////
// Funciones auxiliares
//////////////////////////////////////////////////////////////

//Declaración de CB
void renderFunc();
void resizeFunc(int width, int height);
void idleFunc();
void keyboardFunc(unsigned char key, int x, int y);
void mouseFunc(int button, int state, int x, int y);
void motionFunc(int x, int y);

//Funciones de inicialización y destrucción
void initContext(int argc, char** argv);
void initOGL();

void initScene();
void initShaderTable();
void initMeshesAssets();
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
	std::locale::global(std::locale("spanish"));// acentos ;)

	initContext(argc, argv);
	initOGL();

	initScene();
	initShaderTable();
	initMeshesAssets();
	initRenderEngine();
	initSceneObj();
	initHandlers();
	
	glutMainLoop();

	destroy();

	return 0;
}
	
// ======================================================================
// ======================================================================

void initContext(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitContextVersion(4, 1);
	glutInitContextProfile(GLUT_CORE_PROFILE);
	
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowSize(1024, 1024);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("Prácticas GLSL");

	glewExperimental = GL_TRUE;
	GLenum err = glewInit();
	if (GLEW_OK != err)
	{
		std::cout << "Error: " << glewGetErrorString(err) << std::endl;
		exit(-1);
	}
	
	const GLubyte *oglVersion = glGetString(GL_VERSION);
	std::cout << "This system supports OpenGL Version: " << oglVersion << std::endl;

	glutReshapeFunc(resizeFunc);
	glutDisplayFunc(renderFunc);
	glutIdleFunc(idleFunc);
	glutKeyboardFunc(keyboardFunc);
	glutMouseFunc(mouseFunc);
	glutMotionFunc(motionFunc);
}

void initOGL()
{
	glEnable(GL_DEPTH_TEST);
	glClearColor(1.0f, 1.0f, 1.0f, 0.0f);
	glFrontFace(GL_CCW);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glEnable(GL_CULL_FACE);
}

void destroy()
{
	Engine::TableManager::getInstance().cleanUp();
}

void initScene()
{
	//Engine::TableManager::getInstance().registerTable(&Engine::MeshInstanceTable::getInstance());
	Engine::TableManager::getInstance().registerTable(&Engine::MeshTable::getInstance());
	Engine::TableManager::getInstance().registerTable(&Engine::TextureTable::getInstance());
	Engine::TableManager::getInstance().registerTable(&Engine::ProgramTable::getInstance());


	Engine::Camera * camera = new Engine::Camera(0.1f, 1000.0f, 45.0f, 45.0f);
	camera->translateView(glm::vec3(0.0f, -5.0f, 0.0f));
	camera->rotateView(glm::vec3(glm::radians(30.0f), glm::radians(60.0f), 0.0f));

	// Parameters: name, direction
	Engine::DirectionalLight * dl = new Engine::DirectionalLight("directional_light", glm::vec3(1,1,0));
	dl->setAmbientIntensity(0.2f, 0.2f, 0.2f);
	dl->setDiffuseIntensity(1.0f, 1.0f, 1.0f);
	dl->setSpecularIntensity(0.0f, 0.0f, 0.0f);

	Engine::Scene * scene = new Engine::Scene();
	scene->setCamera(camera);
	scene->setDirectionalLight(dl);

	Engine::SceneManager::getInstance().registerScene("scene_0", scene);
	Engine::SceneManager::getInstance().activateScene("scene_0");
}

void initShaderTable()
{
	
	Engine::TextureTable::getInstance().checkForAnisotropicFilterSupport();

	Engine::ProgramTable::getInstance().registerProgramFactory(Engine::PostProcessProgram::PROGRAM_NAME, new Engine::PostProcessProgramFactory());
	Engine::ProgramTable::getInstance().registerProgramFactory(Engine::DeferredShadingProgram::PROGRAM_NAME, new Engine::DeferredShadingProgramFactory());
	Engine::ProgramTable::getInstance().registerProgramFactory(Engine::ProceduralTerrainProgram::PROGRAM_NAME, new Engine::ProceduralTerrainProgramFactory());
	Engine::ProgramTable::getInstance().registerProgramFactory(Engine::ProceduralWaterProgram::PROGRAM_NAME, new Engine::ProceduralWaterProgramFactory());
	Engine::ProgramTable::getInstance().registerProgramFactory(Engine::SkyProgram::PROGRAM_NAME, new Engine::SkyProgramFactory());
}

void initMeshesAssets()
{
	Engine::CubemapLoadData data;
	data.rightFace = "img/skyboxes/Daylight_Box_Right.png";
	data.leftFace = "img/skyboxes/Daylight_Box_Left.png";
	data.topFace = "img/skyboxes/Daylight_Box_Top.png";
	data.bottomFace = "img/skyboxes/Daylight_Box_Bottom.png";
	data.frontFace = "img/skyboxes/Daylight_Box_Front.png";
	data.backFace = "img/skyboxes/Daylight_Box_Back.png";

	Engine::TextureTable::getInstance().cacheCubemapTexture(data, "DaylightCubemap");

	Engine::MeshTable::getInstance().addMeshToCache("cube", Engine::CreateCube());
	Engine::MeshTable::getInstance().addMeshToCache("plane", Engine::CreatePlane());

	//Engine::MeshInstanceTable::getInstance().instantiateMesh("plane", Engine::PostProcessProgram::PROGRAM_NAME);
	//Engine::MeshInstanceTable::getInstance().instantiateMesh("plane", Engine::DeferredShadingProgram::PROGRAM_NAME);
	//Engine::MeshInstanceTable::getInstance().instantiateMesh("plane", "screen_space_anti_aliasing");
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
	Engine::CameraMovement * cm = new Engine::CameraMovement(Engine::SceneManager::getInstance().getActiveScene()->getCamera());

	Engine::KeyboardHandlersTable * handlers = Engine::SceneManager::getInstance().getActiveScene()->getKeyboardHandler();
	handlers->registerHandler(cm);

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

void renderFunc()
{
	Engine::RenderManager::getInstance().doRender();
	glutSwapBuffers();
}

void resizeFunc(int width, int height)
{
	Engine::SceneManager::getInstance().getActiveScene()->onViewportResize(width, height);
	Engine::RenderManager::getInstance().doResize(width, height);
	glViewport(0, 0, width, height);
}

void idleFunc()
{
	Engine::SceneManager::getInstance().getActiveScene()->getAnimationHandler()->tick();
	glutPostRedisplay();
}

void keyboardFunc(unsigned char key, int x, int y)
{
	Engine::KeyboardHandlersTable * table = Engine::SceneManager::getInstance().getActiveScene()->getKeyboardHandler();
	if (table != NULL)
	{
		table->handleKeyPress(key, x, y);
	}
}

void mouseFunc(int button, int state, int x, int y)
{
	// Left button = 0
	// Pressed = 0
	// Released = 0
	Engine::MouseEventManager * manager = Engine::SceneManager::getInstance().getActiveScene()->getMouseHandler();
	if (manager != NULL)
	{
		manager->handleMouseClick(button, state, x, y);
	}
}

void motionFunc(int x, int y)
{
	// xMin, yMin = 0,0 (top-left corner)
	// xMax, yMax = width,height (bottom-right corner)
	Engine::MouseEventManager * manager = Engine::SceneManager::getInstance().getActiveScene()->getMouseHandler();
	if (manager != NULL)
	{
		manager->handleMouseMotion(x, y);
	}
}