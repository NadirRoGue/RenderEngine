#include "BOX.h"
#include "auxiliar.h"

#include <windows.h>

#include <gl/glew.h>
#define SOLVE_FGLUT_WARNING
#include <gl/freeglut.h> 
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

#include <vector>

#include "PLANE.h"
#include "Scene.h"
#include "ProgramTable.h"
#include "PostProcessProgram.h"
#include "MeshLoader.h"
#include "MeshInstanceTable.h"
#include "Texture.h"
#include "Light.h"
#include "KeyboardHandler.h"
#include "MouseHandler.h"
#include "Renderer.h"
#include "PostProcessManager.h"
#include "DeferredRenderObject.h"

#include "InputImpl.h"
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
	initSceneObj();
	initRenderEngine();
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
	glutInitContextVersion(3, 3);
	glutInitContextProfile(GLUT_CORE_PROFILE);
	
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowSize(500, 500);
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
	glClearColor(1.0f, 0.0f, 1.0f, 0.0f);
	glFrontFace(GL_CCW);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glEnable(GL_CULL_FACE);
}

void destroy()
{
	Engine::MeshLoader::getInstance().destroy();
	Engine::MeshInstanceTable::getInstance().destroy();
	Engine::ProgramTable::getInstance().destroyAllPrograms();
	Engine::TextureTable::getInstance().destroy();
}

void initScene()
{
	Engine::Camera * camera = new Engine::Camera(0.1f, 50.0f, 45.0f, 45.0f);
	camera->translateView(glm::vec3(5.0f, -5.0f, -6.0f));
	camera->rotateView(glm::vec3(glm::radians(30.0f), glm::radians(60.0f), 0.0f));

	// Parameters: name, position, attenuation
	/*Engine::PointLight * pl = new Engine::PointLight("point_light_1", glm::vec3(0, 0, 6), glm::vec3(1.0f,0.5f,0.0f));
	pl->setAmbientIntensity(0.0f, 0.0f, 0.0f);
	pl->setDiffuseIntensity(1.0f, 1.0f, 1.0f);
	pl->setSpecularIntensity(1.0f, 1.0f, 1.0f);*/

	// Parameters: name, direction
	Engine::DirectionalLight * dl = new Engine::DirectionalLight("directional_light", glm::vec3(1,0.2,0));
	dl->setAmbientIntensity(0.1f, 0.1f, 0.1f);
	dl->setDiffuseIntensity(1.0f, 1.0f, 1.0f);
	dl->setSpecularIntensity(0.0f, 0.0f, 0.0f);

	/*Engine::SpotLight * sl = new Engine::SpotLight("spot_light", glm::vec3(-3, 3, 0), glm::vec3(1, 0, 0), 20.0f, 10.0f, glm::vec3(1.0f,0.0f,0.0f));
	sl->setAmbientIntensity(0.0f, 0.0f, 0.0f);
	sl->setDiffuseIntensity(1.0f, 0.0f, 0.0f);
	sl->setSpecularIntensity(0.0f, 0.0f,0.0f);*/

	Engine::Scene * scene = new Engine::Scene();
	scene->setCamera(camera);
	//scene->addPointLight(pl);
	//scene->addSpotLight(sl);
	scene->addDirectionalLight(dl);

	Engine::SceneManager::getInstance().registerScene("scene_0", scene);
	Engine::SceneManager::getInstance().activateScene("scene_0");
}

void initShaderTable()
{
	Engine::TextureTable::getInstance().checkForAnisotropicFilterSupport();
	Engine::TextureTable::getInstance().cacheTexture("img/color2.png");
	Engine::TextureTable::getInstance().cacheTexture("img/emissive.png");
	Engine::TextureTable::getInstance().cacheTexture("img/normal.png");
	Engine::TextureTable::getInstance().cacheTexture("img/specMap.png");

	Engine::TextureTable::getInstance().cacheTexture("img/batman_d.png");
	Engine::TextureTable::getInstance().cacheTexture("img/batman_s.png");
	Engine::TextureTable::getInstance().cacheTexture("img/batman_n.png");
	Engine::TextureTable::getInstance().cacheTexture("img/batman_e.png");

	Engine::TextureTable::getInstance().cacheTexture("img/Rock_10_d.png");
	Engine::TextureTable::getInstance().cacheTexture("img/Rock_10_e.png");
	Engine::TextureTable::getInstance().cacheTexture("img/Rock_10_s.png");
	Engine::TextureTable::getInstance().cacheTexture("img/Rock_10_n.png");

	Engine::TextureTable::getInstance().cacheTexture("img/noise.png");

	Engine::ProgramTable::getInstance().createProgram(new Engine::StandarProgram("full_color_material"),
		"shaders/shader.full_color.vert", "shaders/shader.full_color.frag");

	Engine::ProgramTable::getInstance().createProgram(new Engine::TextureProgram("full_texture_material"),
		"shaders/shader.full_texture.vert", "shaders/shader.full_texture.frag");

	Engine::ProgramTable::getInstance().createProgram(new Engine::PostProcessProgram("post_processing_program"),
		"shaders/postProcessing.v0.vert", "shaders/postProcessing.v0.frag");

	Engine::ProgramTable::getInstance().createProgram(new Engine::GaussianBlurProgram("gaussian_blur_post_processing_program"),
		"shaders/postProcessing.v0.vert", "shaders/postProcessing.GaussianBlur.frag");

	Engine::ProgramTable::getInstance().createProgram(new Engine::DepthOfFieldProgram("depth_of_field_post_processing_program"),
		"shaders/postProcessing.v0.vert", "shaders/postProcessing.DepthOfFieldZbuffer.frag");

	Engine::ProgramTable::getInstance().createProgram(new Engine::DepthRenderProgram("depth_render_post_processing_program"),
		"shaders/postProcessing.v0.vert", "shaders/postProcessing.DepthZBuffer.frag");

	Engine::ProgramTable::getInstance().createProgram(new Engine::PostProcessProgram("color_render_post_processing_program"),
		"shaders/postProcessing.v0.vert", "shaders/postProcessing.ColorRender.frag");

	Engine::ProgramTable::getInstance().createProgram(new Engine::PostProcessProgram("normal_render_post_processing_program"),
		"shaders/postProcessing.v0.vert", "shaders/postProcessing.NormalRender.frag");

	Engine::ProgramTable::getInstance().createProgram(new Engine::PostProcessProgram("specular_render_post_processing_program"),
		"shaders/postProcessing.v0.vert", "shaders/postProcessing.SpecularRender.frag");

	Engine::ProgramTable::getInstance().createProgram(new Engine::DeferredShadingProgram("deferred_shading"),
		"shaders/postProcessing.v0.vert", "shaders/DeferredShading.frag");

	Engine::ProgramTable::getInstance().createProgram(new Engine::EdgeBasedProgram("toon_shading_program"),
		"shaders/postProcessing.v0.vert", "shaders/postProcessing.ToonShading.frag");

	Engine::ProgramTable::getInstance().createProgram(new Engine::EdgeBasedProgram("screen_space_anti_aliasing"),
		"shaders/postProcessing.v0.vert", "shaders/postProcessing.SSAA.frag");

	Engine::ProgramTable::getInstance().createProgram(new Engine::ProceduralTerrainProgram("ProceduralTerrainProgram"), "", "");
}

void initMeshesAssets()
{
	Engine::Mesh cubeMesh = Engine::Mesh((unsigned int)cubeNTriangleIndex, (unsigned int)cubeNVertex,
		cubeTriangleIndex, cubeVertexPos, cubeVertexColor, cubeVertexNormal, cubeVertexTexCoord, cubeVertexTangent);
	Engine::MeshLoader::getInstance().addMeshToCache("cube", cubeMesh);

	Engine::Mesh plane = Engine::Mesh(0, (unsigned int)planeNVertex, 0, planeVertexPos, 0, 0, planeUVs, 0);
	Engine::MeshLoader::getInstance().addMeshToCache("plane", plane);

	Engine::Mesh leftPlane = Engine::Mesh(0, (unsigned int)planeNVertex, 0, leftSmallPlaneVertex, 0, 0, planeUVs, 0);
	Engine::MeshLoader::getInstance().addMeshToCache("left_plane", leftPlane);

	Engine::Mesh rightPlane = Engine::Mesh(0, (unsigned int)planeNVertex, 0, rightSmallPlaneVertex, 0, 0, planeUVs, 0);
	Engine::MeshLoader::getInstance().addMeshToCache("right_plane", rightPlane);

	Engine::MeshInstanceTable::getInstance().instantiateMesh("cube", "full_color_material");
	Engine::MeshInstanceTable::getInstance().instantiateMesh("cube", "full_texture_material");

	Engine::MeshInstanceTable::getInstance().instantiateMesh("models/batman.obj", "full_texture_material");
	Engine::MeshInstanceTable::getInstance().instantiateMesh("models/Rock_10.obj", "full_texture_material");

	Engine::MeshInstanceTable::getInstance().instantiateMesh("plane", "post_processing_program");
	Engine::MeshInstanceTable::getInstance().instantiateMesh("plane", "gaussian_blur_post_processing_program");
	Engine::MeshInstanceTable::getInstance().instantiateMesh("plane", "depth_of_field_post_processing_program");

	Engine::MeshInstanceTable::getInstance().instantiateMesh("plane", "depth_render_post_processing_program");
	Engine::MeshInstanceTable::getInstance().instantiateMesh("plane", "color_render_post_processing_program");
	Engine::MeshInstanceTable::getInstance().instantiateMesh("plane", "normal_render_post_processing_program");
	Engine::MeshInstanceTable::getInstance().instantiateMesh("plane", "specular_render_post_processing_program");
	Engine::MeshInstanceTable::getInstance().instantiateMesh("plane", "deferred_shading");
	Engine::MeshInstanceTable::getInstance().instantiateMesh("plane", "toon_shading_program");
	Engine::MeshInstanceTable::getInstance().instantiateMesh("plane", "screen_space_anti_aliasing");
	
	// Side by side render
	Engine::MeshInstanceTable::getInstance().instantiateMesh("left_plane", "post_processing_program");
	Engine::MeshInstanceTable::getInstance().instantiateMesh("right_plane", "post_processing_program");

	Engine::MeshInstanceTable::getInstance().instantiateMesh("models/quad.obj", "ProceduralTerrainProgram");
	Engine::MeshInstanceTable::getInstance().instantiateMesh("models/plane.obj", "full_texture_material");
}

void initSceneObj()
{
	Engine::Scene * scene = Engine::SceneManager::getInstance().getActiveScene();
	
	Engine::MeshInstance * mi = Engine::MeshInstanceTable::getInstance().getMeshInstance("models/quad.obj", "ProceduralTerrainProgram");
	if (mi != NULL)
	{
		Engine::Object * obj = new Engine::Object(mi);
		scene->addObject(obj);
	}
	
	/*Engine::MeshInstance * colorPlane = Engine::MeshInstanceTable::getInstance().getMeshInstance("models/plane.obj", "full_texture_material");
	if (colorPlane != 0)
	{
		Engine::Object * batmanObj = new Engine::Object(colorPlane);
		batmanObj->setAlbedoTexture(Engine::TextureTable::getInstance().instantiateTexture("img/color2.png"));
		batmanObj->setNormalMapTexture(Engine::TextureTable::getInstance().instantiateTexture("img/normal.png"));
		batmanObj->setSpecularMapTexture(Engine::TextureTable::getInstance().instantiateTexture("img/specMap.png"));
		batmanObj->setEmissiveTexture(Engine::TextureTable::getInstance().instantiateTexture("img/emissive.png"));
		scene->addObject(batmanObj);
	}
	
	Engine::MeshInstance * RockInst = Engine::MeshInstanceTable::getInstance().getMeshInstance("models/Rock_10.obj", "full_texture_material");
	if (RockInst != 0)
	{
		Engine::Object * rockObj = new Engine::Object(RockInst);
		rockObj->scale(glm::vec3(0.5f, 0.5f, 0.5f));
		rockObj->translate(glm::vec3(-3, 0, 3));
		rockObj->setAlbedoTexture(Engine::TextureTable::getInstance().instantiateTexture("img/Rock_10_d.png"));
		rockObj->setNormalMapTexture(Engine::TextureTable::getInstance().instantiateTexture("img/Rock_10_n.png"));
		rockObj->setSpecularMapTexture(Engine::TextureTable::getInstance().instantiateTexture("img/Rock_10_s.png"));
		rockObj->setEmissiveTexture(Engine::TextureTable::getInstance().instantiateTexture("img/Rock_10_e.png"));

		scene->addObject(rockObj);
	}
	*/
	scene->setClearColor(glm::vec3(1, 0, 0));
}

void initHandlers()
{
	Engine::PointLight * pl = Engine::SceneManager::getInstance().getActiveScene()->getLightByName("point_light_1");
	Engine::TestImplementation::LightMovement * lm = new Engine::TestImplementation::LightMovement(pl);
	Engine::TestImplementation::LightIntensityMod * lim = new Engine::TestImplementation::LightIntensityMod(pl);
	Engine::TestImplementation::CameraMovement * cm = new Engine::TestImplementation::CameraMovement(Engine::SceneManager::getInstance().getActiveScene()->getCamera());

	Engine::KeyboardHandlersTable * handlers = Engine::SceneManager::getInstance().getActiveScene()->getKeyboardHandler();
	handlers->registerHandler(lm);
	handlers->registerHandler(lim);
	handlers->registerHandler(cm);
	handlers->registerHandler(new Engine::TestImplementation::RendererSwitcher());

	Engine::TestImplementation::CameraMotion * camMotion = new Engine::TestImplementation::CameraMotion("camera_motion", Engine::SceneManager::getInstance().getActiveScene()->getCamera());
	
	Engine::MouseEventManager * mouseHandler = Engine::SceneManager::getInstance().getActiveScene()->getMouseHandler();
	mouseHandler->registerMouseMotionHandler(camMotion);
}

void initRenderEngine()
{
	Engine::DeferredRenderer * dr = Engine::TestImplementation::createDeferredRendererWithDoF();

	Engine::RenderManager::getInstance().setForwardRenderer(new Engine::ForwardRenderer());
	Engine::RenderManager::getInstance().setDeferredRenderer(dr);

	Engine::RenderManager::getInstance().deferredRender();

	Engine::RenderManager::getInstance().doResize(500, 500);
}

void renderFunc()
{
	Engine::RenderManager::getInstance().doRender();
	// Intercambiamos buffer front and back
	glutSwapBuffers();
}

void resizeFunc(int width, int height)
{
	Engine::SceneManager::getInstance().getActiveScene()->onViewportResize(width, height);
	Engine::RenderManager::getInstance().doResize(width, height);
	glViewport(0, 0, width, height);
	// No necesario porque glViewport ya genera los eventos de pintar
	//glutPostRedisplay();
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