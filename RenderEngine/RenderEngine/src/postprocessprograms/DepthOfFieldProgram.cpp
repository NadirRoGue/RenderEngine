#include "postprocessprograms/DepthOfFieldProgram.h"

#include "renderers/DeferredRenderer.h"
#include "WorldConfig.h"

std::string Engine::DepthOfFieldProgram::PROGRAM_NAME = "DepthOfFieldProgram";

Engine::DepthOfFieldProgram::DepthOfFieldProgram(std::string name, unsigned long long params)
	:Engine::PostProcessProgram(name, params)
{
	fShaderFile = "shaders/postprocess/DepthOfField.frag";
}

Engine::DepthOfFieldProgram::DepthOfFieldProgram(const Engine::DepthOfFieldProgram & other)
	: Engine::PostProcessProgram(other)
{
	uFocalDistance = other.uFocalDistance;
	uMaxDistanceFactor = other.uMaxDistanceFactor;
	uInverseProj = other.uInverseProj;
	uTexelSize = other.uTexelSize;
	uDepthBuffer = other.uDepthBuffer;
}

Engine::DepthOfFieldProgram::~DepthOfFieldProgram()
{
}

void Engine::DepthOfFieldProgram::configureProgram()
{
	Engine::PostProcessProgram::configureProgram();

	uFocalDistance = glGetUniformLocation(glProgram, "focalDistance");
	uMaxDistanceFactor = glGetUniformLocation(glProgram, "maxDistanceFactor");
	uInverseProj = glGetUniformLocation(glProgram, "invProj");
	uTexelSize = glGetUniformLocation(glProgram, "texelSize");
	uDepthBuffer = glGetUniformLocation(glProgram, "depthBuffer");
}

void Engine::DepthOfFieldProgram::onRenderObject(const Engine::Object * obj, Engine::Camera * camera)
{
	Engine::PostProcessProgram::onRenderObject(obj, camera);

	glm::mat4 invProj = glm::inverse(camera->getProjectionMatrix());

	glUniform1f(uFocalDistance, Engine::Settings::dofFocalDist);
	glUniform1f(uMaxDistanceFactor, Engine::Settings::dofMaxDist);
	glUniformMatrix4fv(uInverseProj, 1, GL_FALSE, &invProj[0][0]);
	glUniform2f(uTexelSize, 1.0f / (float)Engine::ScreenManager::SCREEN_WIDTH, 1.0f / (float)Engine::ScreenManager::SCREEN_HEIGHT);

	Engine::DeferredRenderer * dr = static_cast<Engine::DeferredRenderer*>(Engine::RenderManager::getInstance().getRenderer());
	
	glUniform1i(uDepthBuffer, 1);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, dr->getGBufferDepth()->getTexture()->getTextureId());
}

// ==================================================================

Engine::Program * Engine::DepthOfFieldProgramFactory::createProgram(unsigned long long parameters)
{
	Engine::DepthOfFieldProgram * program = new Engine::DepthOfFieldProgram(Engine::DepthOfFieldProgram::PROGRAM_NAME, parameters);
	program->initialize();
	return program;
}