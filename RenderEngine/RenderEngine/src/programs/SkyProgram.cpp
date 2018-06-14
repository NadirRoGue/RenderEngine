#include "programs/SkyProgram.h"

#include "WorldConfig.h"
#include "Scene.h"

std::string Engine::SkyProgram::PROGRAM_NAME = "SkyProgram";

Engine::SkyProgram::SkyProgram(std::string name, unsigned long long params)
	:Program(name, params)
{
	vShaderFile = "shaders/sky/sky.vert";
	fShaderFile = "shaders/sky/sky.frag";
}

Engine::SkyProgram::SkyProgram(const Engine::SkyProgram & other)
	: Program(other)
{
	uProjMatrix = other.uProjMatrix;
	uLightDir = other.uLightDir;
	uLightColor = other.uLightColor;
	uSkyHorizonColor = other.uSkyHorizonColor;
	uSkyZenitColor = other.uSkyZenitColor;
	uColorFactor = other.uColorFactor;
	inPos = other.inPos;
}

void Engine::SkyProgram::configureProgram()
{
	uProjMatrix = glGetUniformLocation(glProgram, "proj");
	uLightDir = glGetUniformLocation(glProgram, "lightDir");
	uLightColor = glGetUniformLocation(glProgram, "lightColor");
	uColorFactor = glGetUniformLocation(glProgram, "colorFactor");

	uSkyZenitColor = glGetUniformLocation(glProgram, "zenitColor");
	uSkyHorizonColor = glGetUniformLocation(glProgram, "horizonColor");

	inPos = glGetAttribLocation(glProgram, "inPos");
}

void Engine::SkyProgram::configureMeshBuffers(Engine::Mesh * m)
{
	m->use();

	if (inPos != -1)
	{
		glBindBuffer(GL_ARRAY_BUFFER, m->vboVertices);
		glVertexAttribPointer(inPos, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(inPos);
	}
}

void Engine::SkyProgram::onRenderObject(const Engine::Object * obj, Engine::Camera * camera)
{
	glm::mat4 modelViewProj = camera->getProjectionMatrix() * camera->getViewMatrix() * obj->getModelMatrix();
	glUniformMatrix4fv(uProjMatrix, 1, GL_FALSE, &(modelViewProj[0][0]));

	glm::vec3 direction = -glm::normalize(Engine::Settings::lightDirection);
	glUniform3fv(uLightDir, 1, &direction[0]);

	glUniform3fv(uLightColor, 1, &Engine::Settings::realLightColor[0]);
	glUniform1f(uColorFactor, Engine::Settings::lightFactor);

	glUniform3fv(uSkyZenitColor, 1, &Engine::Settings::skyZenitColor[0]);
	glUniform3fv(uSkyHorizonColor, 1, &Engine::Settings::skyHorizonColor[0]);
}

// =====================================================================

Engine::Program * Engine::SkyProgramFactory::createProgram(unsigned long long parameters)
{
	Engine::SkyProgram * program = new Engine::SkyProgram(Engine::SkyProgram::PROGRAM_NAME, parameters);
	program->initialize();
	return program;
}