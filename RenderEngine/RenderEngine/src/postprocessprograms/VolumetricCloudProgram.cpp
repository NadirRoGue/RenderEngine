#include "postprocessprograms/VolumetricCloudProgram.h"

#include "volumetricclouds/NoiseInitializer.h"

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
	uInvProj = other.uInvProj;
	uViewProj = other.uViewProj;

	uPerlinWorley = other.uPerlinWorley;
	uWorley = other.uWorley;
	uWeather = other.uWeather;
}

void Engine::VolumetricCloudProgram::configureProgram()
{
	Engine::PostProcessProgram::configureProgram();

	uInvView = glGetUniformLocation(glProgram, "invView");
	uInvProj = glGetUniformLocation(glProgram, "invProj");
	uViewProj = glGetUniformLocation(glProgram, "viewProj");

	uPerlinWorley = glGetUniformLocation(glProgram, "perlinworley");
	uWorley = glGetUniformLocation(glProgram, "worley");
	uWeather = glGetUniformLocation(glProgram, "weather");
}

void Engine::VolumetricCloudProgram::onRenderObject(Engine::Object * obj, const glm::mat4 & view, const glm::mat4 & proj)
{
	//Engine::PostProcessProgram::onRenderObject(obj, view, proj);
	glm::mat4 invView = glm::inverse(view);
	glm::mat4 invProj = glm::inverse(proj);
	glm::mat4 viewProj = proj * view;

	glUniformMatrix4fv(uInvView, 1, GL_FALSE, &(invView[0][0]));
	glUniformMatrix4fv(uInvProj, 1, GL_FALSE, &(invProj[0][0]));
	glUniformMatrix4fv(uViewProj, 1, GL_FALSE, &(viewProj[0][0]));

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
}

// ===============================================================================================

Engine::Program * Engine::VolumetricCloudProgramFactory::createProgram(unsigned long long params)
{
	Engine::VolumetricCloudProgram * prog = new Engine::VolumetricCloudProgram(Engine::VolumetricCloudProgram::PROGRAM_NAME, params);
	prog->initialize();
	return prog;
}