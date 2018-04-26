/**
* @author Nadir Román Guerrero
* @email nadir.ro.gue@gmail.com
*/

#include "PostProcessProgram.h"

#include "Renderer.h"
#include "instances/TextureInstance.h"

#include <iostream>

std::string Engine::PostProcessProgram::PROGRAM_NAME = "PostProcessProgram";

Engine::PostProcessProgram::PostProcessProgram(std::string name, unsigned long long params)
	:Engine::Program(name, params)
{
	vShaderFile = "shaders/postProcessing.v0.vert";
	fShaderFile = "shaders/postProcessing.v0.frag";
}

Engine::PostProcessProgram::PostProcessProgram(unsigned long long params)
	:Engine::PostProcessProgram::PostProcessProgram(Engine::PostProcessProgram::PROGRAM_NAME, params)
{
}

Engine::PostProcessProgram::PostProcessProgram(const Engine::PostProcessProgram & other)
	: Engine::Program(other)
{
	planeVAO = other.planeVAO;
	planeVerticesVBO = other.planeVerticesVBO;

	inPos = other.inPos;
	inTexCoord = other.inTexCoord;

	memcpy(uRenderedTextures, other.uRenderedTextures, 9 * sizeof(unsigned int));
}

Engine::PostProcessProgram::~PostProcessProgram()
{
}

void Engine::PostProcessProgram::configureProgram()
{
	for (int i = 0; i < 9; i++)
	{
		std::string uniformName = "postProcessing_" + std::to_string(i);
		uRenderedTextures[i] = glGetUniformLocation(glProgram, uniformName.c_str());
	}

	inPos = glGetAttribLocation(glProgram, "inPos");
	inTexCoord = glGetAttribLocation(glProgram, "inTexCoord");
}

void Engine::PostProcessProgram::configureMeshBuffers(Engine::Mesh * data)
{
	glBindVertexArray(data->vao);
	
	glBindBuffer(GL_ARRAY_BUFFER, data->vboVertices);
	glVertexAttribPointer(inPos, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(inPos);

	glBindBuffer(GL_ARRAY_BUFFER, data->vboUVs);
	glVertexAttribPointer(inTexCoord, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(inTexCoord);
}

void Engine::PostProcessProgram::configureClearColor(const glm::vec3 & cc)
{
	// Does nothing
}

void Engine::PostProcessProgram::onRenderLight(const glm::mat4 & model, const glm::mat4 & view)
{
}

void Engine::PostProcessProgram::onRenderSpotLight(const glm::mat4 & modelPos, const glm::mat4 & modelDir, const glm::mat4 & view)
{
}

void Engine::PostProcessProgram::onRenderDirectionalLight(const glm::mat4 & model, const glm::mat4 & view)
{
}

void Engine::PostProcessProgram::configurePointLightBuffer(const Engine::PointLight *pl)
{
}

void Engine::PostProcessProgram::configureSpotLightBuffer(const Engine::SpotLight *sl)
{
}

void Engine::PostProcessProgram::configureDirectionalLightBuffer(const Engine::DirectionalLight *dl)
{
}

void Engine::PostProcessProgram::onRenderObject(const Engine::Object * obj, const glm::mat4 & view, const glm::mat4 &proj)
{

	std::map<std::string, TextureInstance *> all = ((PostProcessObject*)obj)->getAllCustomTextures();
	std::map<std::string, TextureInstance *>::const_iterator it = all.cbegin();
	
	int start = 0;
	while (it != all.cend() && start < 9)
	{
		if (uRenderedTextures[start] != -1)
		{
			glUniform1i(uRenderedTextures[start], start);
			glActiveTexture(GL_TEXTURE0 + start);
			glBindTexture(GL_TEXTURE_2D, it->second->getTexture()->getTextureId());
		}
		start++;
		it++;
	}
}

// ==============================================================================

Engine::Program * Engine::PostProcessProgramFactory::createProgram(unsigned long long parameters)
{
	Engine::PostProcessProgram * program = new Engine::PostProcessProgram(Engine::PostProcessProgram::PROGRAM_NAME, parameters);
	program->initialize();
	return program;
}