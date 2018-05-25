#include "postprocessprograms/DeferredShadingProgram.h"

#include "Scene.h"
#include "LightBufferManager.h"

std::string Engine::DeferredShadingProgram::PROGRAM_NAME = "DeferredShadingProgram";

Engine::DeferredShadingProgram::DeferredShadingProgram(std::string name, unsigned long long params)
	:Engine::PostProcessProgram(name, params)
{
	fShaderFile = "shaders/postprocess/DeferredShading.frag";
}

Engine::DeferredShadingProgram::DeferredShadingProgram(const Engine::DeferredShadingProgram & other)
	: Engine::PostProcessProgram(other)
{
	uDirectionalLightDir = other.uDirectionalLightDir;
	uDLIa = other.uDLIa;
	uDLId = other.uDLId;
	uDLIs = other.uDLIs;

	uDLBuffer = other.uDLBuffer;
	uPLBuffer = other.uPLBuffer;
	uSLBuffer = other.uSLBuffer;

	uBackground = other.uBackground;

	uProjMat = other.uProjMat;
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

void Engine::DeferredShadingProgram::onRenderObject(const Engine::Object * obj, const glm::mat4 & view, const glm::mat4 &proj)
{
	Engine::PostProcessProgram::onRenderObject(obj, view, proj);

	Engine::Scene * scene = Engine::SceneManager::getInstance().getActiveScene();
	if (scene != 0)
	{
		Engine::DirectionalLight * dl = scene->getDirectionalLight();
		processDirectionalLights(dl, view);
	}

	glUniformMatrix4fv(uProjMat, 1, GL_FALSE, &(proj[0][0]));

	glUniform3fv(uBackground, 1, &scene->getClearColor()[0]);
}

void Engine::DeferredShadingProgram::configureProgram()
{
	Engine::PostProcessProgram::configureProgram();

	uBackground = glGetUniformLocation(glProgram, "backgroundColor");

	uDLBuffer = glGetUniformBlockIndex(glProgram, "DLBuffer");
	uPLBuffer = glGetUniformBlockIndex(glProgram, "PLBuffer");
	uSLBuffer = glGetUniformBlockIndex(glProgram, "SLBuffer");

	uProjMat = glGetUniformLocation(glProgram, "projMat");

	uDLIa = glGetUniformLocation(glProgram, "DLIa");
	uDLId = glGetUniformLocation(glProgram, "DLId");
	uDLIs = glGetUniformLocation(glProgram, "DLIs");
	uDirectionalLightDir = glGetUniformLocation(glProgram, "DLdir");
}


// =====================================================

Engine::Program * Engine::DeferredShadingProgramFactory::createProgram(unsigned long long parameters)
{
	Engine::DeferredShadingProgram * program = new Engine::DeferredShadingProgram(Engine::DeferredShadingProgram::PROGRAM_NAME, parameters);
	program->initialize();
	return program;
}