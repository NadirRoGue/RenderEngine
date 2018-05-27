#include "postprocessprograms/SSGodRayProgram.h"

#include "WorldConfig.h"
#include "Scene.h"

#include <iostream>

const std::string Engine::SSGodRayProgram::PROGRAM_NAME = "SSGodRayProgram";

Engine::SSGodRayProgram::SSGodRayProgram(std::string name, unsigned long long params)
	:Engine::PostProcessProgram(name, params)
{
	fShaderFile = "shaders/postprocess/SSGodRays.frag";
}

Engine::SSGodRayProgram::SSGodRayProgram(const Engine::SSGodRayProgram & other)
	: Engine::PostProcessProgram(other)
{
	uLightScreenPos = other.uLightScreenPos;
	uWeight = other.uWeight;
	uDecay = other.uDecay;
	uDensity = other.uDensity;
	uExposure = other.uExposure;
	uOnlyPass = other.uOnlyPass;
}

void Engine::SSGodRayProgram::configureProgram()
{
	Engine::PostProcessProgram::configureProgram();

	uLightScreenPos = glGetUniformLocation(glProgram, "lightScreenPos");
	uWeight = glGetUniformLocation(glProgram, "weight");
	uExposure = glGetUniformLocation(glProgram, "exposure");
	uDensity = glGetUniformLocation(glProgram, "density");
	uDecay = glGetUniformLocation(glProgram, "decay");
	uOnlyPass = glGetUniformLocation(glProgram, "onlyPass");
}

void Engine::SSGodRayProgram::onRenderObject(const Engine::Object * obj, const glm::mat4 & view, const glm::mat4 & proj)
{
	Engine::PostProcessProgram::onRenderObject(obj, view, proj);

	// Project light position on screen
	Engine::Scene * scene = Engine::SceneManager::getInstance().getActiveScene();
	Engine::Camera * cam = scene->getCamera();
	glm::vec4 lightDir = glm::vec4(-cam->getPosition() + Engine::Settings::lightDirection * 20.0f, 1.0);
	lightDir = proj * view * lightDir;
	lightDir /= lightDir.w;
	lightDir = lightDir * 0.5f + 0.5f;

	// Its a screen space effect based on sun position on screen. When its offscreen, it produces artifacts
	// make sure we only apply when its ok
	bool onlyPass = lightDir.x < 0.f || lightDir.x > 1.f || lightDir.y < 0.f || lightDir.y > 1.f || lightDir.z > 1.f;
	glUniform1i(uOnlyPass, onlyPass);

	//lightDir.y = -glm::abs(lightDir.y);
	glUniform2fv(uLightScreenPos, 1, &lightDir[0]);

	// Send tweakable data
	glUniform1f(uWeight, Engine::Settings::godRaysWeight);
	glUniform1f(uExposure, Engine::Settings::godRaysExposure);
	glUniform1f(uDensity, Engine::Settings::godRaysDensity);
	glUniform1f(uDecay, Engine::Settings::godRaysDecay);
}

// ==========================================================================

Engine::Program * Engine::SSGodRayProgramFactory::createProgram(unsigned long long params)
{
	Engine::SSGodRayProgram * prog = new Engine::SSGodRayProgram(Engine::SSGodRayProgram::PROGRAM_NAME, params);
	prog->initialize();
	return prog;
}