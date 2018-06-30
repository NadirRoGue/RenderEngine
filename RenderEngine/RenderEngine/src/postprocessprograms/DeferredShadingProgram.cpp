#include "postprocessprograms/DeferredShadingProgram.h"

#include "Scene.h"
#include "LightBufferManager.h"
#include "WorldConfig.h"

std::string Engine::DeferredShadingProgram::PROGRAM_NAME = "DeferredShadingProgram";

Engine::DeferredShadingProgram::DeferredShadingProgram(std::string name, unsigned long long params)
	:Engine::PostProcessProgram(name, params)
{
	fShaderFile = "shaders/postprocess/DeferredShading.frag";
}

Engine::DeferredShadingProgram::DeferredShadingProgram(const Engine::DeferredShadingProgram & other)
	: Engine::PostProcessProgram(other)
{
	uDLBuffer = other.uDLBuffer;
	uPLBuffer = other.uPLBuffer;
	uSLBuffer = other.uSLBuffer;

	uSkyHorizonColor = other.uSkyHorizonColor;
	uSkyZenitColor = other.uSkyZenitColor;

	uColorFactor = other.uColorFactor;
}

void Engine::DeferredShadingProgram::processDirectionalLights(Engine::DirectionalLight * dl, const glm::mat4 & view)
{
	glm::mat4 resultPos = view * dl->getModelMatrix();

	glm::vec3 direction(resultPos[3][0], resultPos[3][1], resultPos[3][2]);
	direction = glm::normalize(direction);

	memcpy(dl->getData().direction, &direction[0], sizeof(float) * 3);

	Engine::GPU::LightBufferManager::getInstance().enableDirectionalLightBufferAtIndex(uDLBuffer);
	Engine::GPU::LightBufferManager::getInstance().updateDirectionalLight(dl, dl->requiresUpdate());
	dl->clearUpdateFlag();
}

void Engine::DeferredShadingProgram::onRenderObject(const Engine::Object * obj, Engine::Camera * camera)
{
	Engine::PostProcessProgram::onRenderObject(obj, camera);

	Engine::Scene * scene = Engine::SceneManager::getInstance().getActiveScene();
	if (scene != 0)
	{
		Engine::DirectionalLight * dl = scene->getDirectionalLight();
		processDirectionalLights(dl, camera->getViewMatrix());
	}

	glUniform3fv(uSkyZenitColor, 1, &Engine::Settings::skyZenitColor[0]);
	glUniform3fv(uSkyHorizonColor, 1, &Engine::Settings::skyHorizonColor[0]);

	glUniform1f(uColorFactor, Engine::Settings::lightFactor);
}

void Engine::DeferredShadingProgram::configureProgram()
{
	Engine::PostProcessProgram::configureProgram();

	uSkyHorizonColor = glGetUniformLocation(glProgram, "horizonColor");
	uSkyZenitColor = glGetUniformLocation(glProgram, "zenitColor");

	uDLBuffer = glGetUniformBlockIndex(glProgram, "DLBuffer");
	uPLBuffer = glGetUniformBlockIndex(glProgram, "PLBuffer");
	uSLBuffer = glGetUniformBlockIndex(glProgram, "SLBuffer");

	uColorFactor = glGetUniformLocation(glProgram, "colorFactor");
}

// =====================================================

Engine::Program * Engine::DeferredShadingProgramFactory::createProgram(unsigned long long parameters)
{
	Engine::DeferredShadingProgram * program = new Engine::DeferredShadingProgram(Engine::DeferredShadingProgram::PROGRAM_NAME, parameters);
	program->initialize();
	return program;
}