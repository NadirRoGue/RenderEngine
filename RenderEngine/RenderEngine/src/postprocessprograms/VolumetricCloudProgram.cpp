#include "postprocessprograms/VolumetricCloudProgram.h"

#include "volumetricclouds/NoiseInitializer.h"
#include "Scene.h"
#include "WorldConfig.h"
#include "TimeAccesor.h"
#include "renderers/DeferredRenderer.h"

const std::string Engine::VolumetricCloudProgram::PROGRAM_NAME = "VolumetricCloudProgram";

Engine::VolumetricCloudProgram::VolumetricCloudProgram(std::string name, unsigned long long params)
	:Engine::PostProcessProgram(name, params)
{
	fShaderFile = "shaders/clouds/volumetricclouds.frag";
}

Engine::VolumetricCloudProgram::VolumetricCloudProgram(const Engine::VolumetricCloudProgram & other)
	: Engine::PostProcessProgram(other)
{
	uInvView = other.uInvView;

	uResolution = other.uResolution;
	uFov = other.uFov;

	uPerlinWorley = other.uPerlinWorley;
	uWorley = other.uWorley;
	uWeather = other.uWeather;

	uCamPos = other.uCamPos;
	uLightDir = other.uLightDir;
	uLightColor = other.uLightColor;
	uCloudColor = other.uCloudColor;
	uZenitColor = other.uZenitColor;
	uHorizonColor = other.uHorizonColor;
	uTime = other.uTime;

	uCloudSpeed = other.uCloudSpeed;
	uCloudType = other.uCloudType;
	uCoverageMultiplier = other.uCoverageMultiplier;

	uCurrentDepth = other.uCurrentDepth;
}

void Engine::VolumetricCloudProgram::configureProgram()
{
	Engine::PostProcessProgram::configureProgram();

	uInvView = glGetUniformLocation(glProgram, "invView");

	uResolution = glGetUniformLocation(glProgram, "screenResolution");
	uFov = glGetUniformLocation(glProgram, "FOV");

	uPerlinWorley = glGetUniformLocation(glProgram, "perlinworley");
	uWorley = glGetUniformLocation(glProgram, "worley");
	uWeather = glGetUniformLocation(glProgram, "weather");

	uCamPos = glGetUniformLocation(glProgram, "camPos");
	uLightDir = glGetUniformLocation(glProgram, "lightDir");
	uLightColor = glGetUniformLocation(glProgram, "lightColor");
	uCloudColor = glGetUniformLocation(glProgram, "cloudColor");
	uZenitColor = glGetUniformLocation(glProgram, "zenitColor");
	uHorizonColor = glGetUniformLocation(glProgram, "horizonColor");
	uTime = glGetUniformLocation(glProgram, "time");

	uCloudSpeed = glGetUniformLocation(glProgram, "cloudSpeed");
	uCloudType = glGetUniformLocation(glProgram, "cloudType");
	uCoverageMultiplier = glGetUniformLocation(glProgram, "coverageMultiplier");

	uCurrentDepth = glGetUniformLocation(glProgram, "currentPixelDepth");
}

void Engine::VolumetricCloudProgram::onRenderObject(Engine::Object * obj, const glm::mat4 & view, const glm::mat4 & proj)
{
	glm::mat4 invView = glm::inverse(view);
	glUniformMatrix4fv(uInvView, 1, GL_FALSE, &(invView[0][0]));

	Engine::Scene * scene = Engine::SceneManager::getInstance().getActiveScene();
	Engine::Camera * cam = scene->getCamera();
	glm::vec3 camPos = -cam->getPosition();
	glUniform3fv(uCamPos, 1, &camPos[0]);

	glm::vec3 normLightDir = glm::normalize(Engine::Settings::lightDirection);
	glUniform3fv(uLightDir, 1, &normLightDir[0]);
	glUniform3fv(uLightColor, 1, &Engine::Settings::lightColor[0]);

	glUniform3fv(uZenitColor, 1, &Engine::Settings::skyZenitColor[0]);
	glUniform3fv(uHorizonColor, 1, &Engine::Settings::skyHorizonColor[0]);

	glUniform3fv(uCloudColor, 1, &Engine::Settings::cloudColor[0]);
	glUniform1f(uTime, Engine::Time::timeSinceBegining);
	glUniform1f(uCloudSpeed, Engine::Settings::cloudSpeed);
	glUniform1f(uCloudType, Engine::Settings::cloudType);
	glUniform1f(uCoverageMultiplier, Engine::Settings::coverageMultiplier);

	float res[2] = { (float)Engine::ScreenManager::SCREEN_WIDTH, (float)Engine::ScreenManager::SCREEN_HEIGHT };
	glUniform2fv(uResolution, 1, res);
	glUniform1f(uFov, cam->getFOV());

	const Engine::TextureInstance * pw = Engine::CloudSystem::NoiseInitializer::getInstance().getPerlinWorleyFBM();
	const Engine::TextureInstance * w = Engine::CloudSystem::NoiseInitializer::getInstance().getWorleyFBM();
	const Engine::TextureInstance * wth = Engine::CloudSystem::NoiseInitializer::getInstance().getWeatherData();

	glUniform1i(uPerlinWorley, 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_3D, pw->getTexture()->getTextureId());

	glUniform1i(uWorley, 1);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_3D, w->getTexture()->getTextureId());

	glUniform1i(uWeather, 2);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, wth->getTexture()->getTextureId());

	Engine::DeferredRenderer * dr = static_cast<Engine::DeferredRenderer*>(Engine::RenderManager::getInstance().getRenderer());
	glUniform1i(uCurrentDepth, 3);
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, dr->getGBufferDepth()->getTexture()->getTextureId());
}

// ===============================================================================================

Engine::Program * Engine::VolumetricCloudProgramFactory::createProgram(unsigned long long params)
{
	Engine::VolumetricCloudProgram * prog = new Engine::VolumetricCloudProgram(Engine::VolumetricCloudProgram::PROGRAM_NAME, params);
	prog->initialize();
	return prog;
}