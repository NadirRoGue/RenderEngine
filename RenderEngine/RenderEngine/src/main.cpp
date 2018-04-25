#include <windows.h>

#include <gl/glew.h>
#define SOLVE_FGLUT_WARNING
#include <gl/freeglut.h> 
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

#include <vector>

#include "Scene.h"
#include "datatables/ProgramTable.h"
#include "PostProcessProgram.h"
#include "datatables/MeshTable.h"
#include "datatables/MeshInstanceTable.h"
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
	Engine::MeshTable::getInstance().destroy();
	Engine::MeshInstanceTable::getInstance().destroy();
	Engine::ProgramTable::getInstance().destroyAllPrograms();
	Engine::TextureTable::getInstance().destroy();
}

void initScene()
{
	Engine::Camera * camera = new Engine::Camera(0.1f, 1000.0f, 45.0f, 45.0f);
	camera->translateView(glm::vec3(0.0f, -5.0f, 0.0f));
	camera->rotateView(glm::vec3(glm::radians(30.0f), glm::radians(60.0f), 0.0f));

	// Parameters: name, direction
	Engine::DirectionalLight * dl = new Engine::DirectionalLight("directional_light", glm::vec3(1,1,0));
	dl->setAmbientIntensity(0.1f, 0.1f, 0.1f);
	dl->setDiffuseIntensity(1.0f, 1.0f, 1.0f);
	dl->setSpecularIntensity(0.0f, 0.0f, 0.0f);

	Engine::Scene * scene = new Engine::Scene();
	scene->setCamera(camera);
	scene->addDirectionalLight(dl);

	Engine::SceneManager::getInstance().registerScene("scene_0", scene);
	Engine::SceneManager::getInstance().activateScene("scene_0");
}

void initShaderTable()
{
	
	Engine::TextureTable::getInstance().checkForAnisotropicFilterSupport();

	Engine::ProgramTable::getInstance().createProgram(new Engine::PostProcessProgram("post_processing_program"),
		"shaders/postProcessing.v0.vert", "shaders/postProcessing.v0.frag");

	Engine::ProgramTable::getInstance().createProgram(new Engine::DeferredShadingProgram("deferred_shading"),
		"shaders/postProcessing.v0.vert", "shaders/DeferredShading.frag");

	Engine::ProgramTable::getInstance().createProgram(new Engine::SSAAProgram("screen_space_anti_aliasing"),
		"shaders/postProcessing.v0.vert", "shaders/postProcessing.SSAA.frag");
		
	Engine::ProgramTable::getInstance().createProgram(new Engine::ProceduralTerrainProgram("ProceduralTerrainProgram"), "", "");

	Engine::ProgramTable::getInstance().createProgram(new Engine::PerlinGeneratorProgram("PerlinGeneratorProgram"),
		"shaders/postProcessing.v0.vert", "shaders/terrain/perlin.frag");

	Engine::ProgramTable::getInstance().createProgram(new Engine::SkyProgram("SkyProgram"),
		"shaders/sky/sky.vert", "shaders/sky/sky.frag");
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
	//data.rightFace = "img/skyboxes/right.jpg";
	//data.leftFace = "img/skyboxes/left.jpg";
	//data.topFace = "img/skyboxes/top.jpg";
	//data.bottomFace = "img/skyboxes/bottom.jpg";
	//data.frontFace = "img/skyboxes/front.jpg";
	//data.backFace = "img/skyboxes/back.jpg";

	Engine::TextureTable::getInstance().cacheCubemapTexture(data, "DaylightCubemap");

	Engine::MeshTable::getInstance().addMeshToCache("cube", Engine::CreateCube());
	Engine::MeshTable::getInstance().addMeshToCache("plane", Engine::CreatePlane());
	Engine::MeshTable::getInstance().addMeshToCache("left_plane", Engine::CreateLeftPlane());
	Engine::MeshTable::getInstance().addMeshToCache("right_plane", Engine::CreateRightPlane());

	//Engine::MeshInstanceTable::getInstance().instantiateMesh("cube", "full_color_material");
	//Engine::MeshInstanceTable::getInstance().instantiateMesh("cube", "full_texture_material");

	//Engine::MeshInstanceTable::getInstance().instantiateMesh("models/Rock_10.obj", "full_texture_material");

	Engine::MeshInstanceTable::getInstance().instantiateMesh("plane", "post_processing_program");
	Engine::MeshInstanceTable::getInstance().instantiateMesh("plane", "deferred_shading");
	Engine::MeshInstanceTable::getInstance().instantiateMesh("plane", "screen_space_anti_aliasing");
		
	//Engine::MeshInstanceTable::getInstance().instantiateMesh("plane", "PerlinGeneratorProgram");
	//Engine::MeshInstanceTable::getInstance().instantiateMesh("models/quad.obj", "ProceduralTerrainProgram");
	//Engine::MeshInstanceTable::getInstance().instantiateMesh("models/plane.obj", "ProceduralTerrainProgram");
}

void initSceneObj()
{
	/*
	Engine::PostProcessChainNode * perlinGenerator = new Engine::PostProcessChainNode;
	Engine::MeshInstance * pp = Engine::MeshInstanceTable::getInstance().getMeshInstance("plane", "PerlinGeneratorProgram");
	Engine::DeferredRenderObject * buffer = new Engine::DeferredRenderObject(1, true);
	buffer->addColorBuffer(0, GL_RGBA8, GL_RGBA, GL_FLOAT, 1024, 1024, GL_NEAREST);
	buffer->addDepthBuffer24(1024, 1024);

	buffer->initialize();
	buffer->resizeFBO(1024, 1024);

	Engine::Program * perlinProgram = Engine::ProgramTable::getInstance().getProgramByName("PerlinGeneratorProgram");
	Engine::PerlinGeneratorProgram * finalProgram = new Engine::PerlinGeneratorProgram(*dynamic_cast<Engine::PerlinGeneratorProgram*>(perlinProgram));

	perlinGenerator->obj = new Engine::PostProcessObject(pp);
	perlinGenerator->renderBuffer = buffer;
	perlinGenerator->postProcessProgram = finalProgram;

	Engine::RenderManager::getInstance().getCurrentDeferredRenderer()->setPreProcess(perlinGenerator);
	*/
	Engine::Scene * scene = Engine::SceneManager::getInstance().getActiveScene();
	scene->setTerrain(new Engine::Terrain(5.0f, 20));
	scene->setSkybox(new Engine::SkyBox(Engine::TextureTable::getInstance().instantiateTexture("DaylightCubemap")));
	/*
	Engine::MeshInstance * mi = Engine::MeshInstanceTable::getInstance().getMeshInstance("models/plane.obj", "ProceduralTerrainProgram");
	if (mi != NULL)
	{
		Engine::Object * obj = new Engine::Object(mi);
		obj->setRenderMode(GL_PATCHES);
		//buffer->populateDeferredObject(obj);
		scene->addObject(obj);
	}*/
	
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