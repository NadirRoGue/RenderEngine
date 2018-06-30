#include "programs/CloudShadowProgram.h"

#include "volumetricclouds/NoiseInitializer.h"
#include "WorldConfig.h"

const std::string Engine::CloudShadowProgram::PROGRAM_NAME = "CloudShadowProgram";

Engine::CloudShadowProgram::CloudShadowProgram(std::string name, unsigned long long params)
	:Engine::Program(name, params)
{
	vShaderFile = "shaders/clouds/cloudshadow.vert";
	fShaderFile = "shaders/clouds/cloudshadow.frag";
}

Engine::CloudShadowProgram::CloudShadowProgram(const Engine::CloudShadowProgram & other)
	:Engine::Program(other)
{
	uInPos = other.uInPos;
	uLightProjMat = other.uLightProjMat;
	uWeather = other.uWeather;
}

void Engine::CloudShadowProgram::configureProgram()
{
	uInPos = glGetAttribLocation(glProgram, "inPos");
	
	uLightProjMat = glGetUniformLocation(glProgram, "lightProjMat");
	uWeather = glGetUniformLocation(glProgram, "weather");
	uCoverageMult = glGetUniformLocation(glProgram, "coverageMultiplier");
}

void Engine::CloudShadowProgram::configureMeshBuffers(Engine::Mesh * mesh)
{
	mesh->use();

	if (uInPos != -1)
	{
		glBindBuffer(GL_ARRAY_BUFFER, mesh->vboVertices);
		glVertexAttribPointer(uInPos, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(uInPos);
	}

	if (uInUV != -1)
	{
		//glBindBuffer(GL_ARRAY_BUFFER, mesh->vboUVs);
		//glVertexAttribPoint
	}
}

void Engine::CloudShadowProgram::onRenderObject(const Engine::Object * obj, Engine::Camera * camera)
{
	//Engine::Program::onRenderObject(obj, camera);
	const Engine::TextureInstance * weather = Engine::CloudSystem::NoiseInitializer::getInstance().getWeatherData();

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, weather->getTexture()->getTextureId());
	glUniform1i(uWeather, 0);

	glUniform1f(uCoverageMult, Engine::Settings::coverageMultiplier);
}

void Engine::CloudShadowProgram::setUniformLightProjMatrix(const glm::mat4 & proj)
{
	glUniformMatrix4fv(uLightProjMat, 1, GL_FALSE, &(proj[0][0]));
}

 // ========================================================

Engine::Program * Engine::CloudShadowProgramFactory::createProgram(unsigned long long parameters)
{
	Engine::CloudShadowProgram * p = new Engine::CloudShadowProgram(Engine::CloudShadowProgram::PROGRAM_NAME, parameters);
	p->initialize();
	return p;
}