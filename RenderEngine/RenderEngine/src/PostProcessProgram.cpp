/**
* @author Nadir Román Guerrero
* @email nadir.ro.gue@gmail.com
*/

#include "PostProcessProgram.h"

#include "Renderer.h"
#include "instances/TextureInstance.h"

#include <iostream>

Engine::PostProcessProgram::PostProcessProgram(std::string name)
	:Engine::Program(name)
{
}

Engine::PostProcessProgram::PostProcessProgram(const Engine::PostProcessProgram & other)
	: Engine::Program(other)
{
	planeVAO = other.planeVAO;
	planeVerticesVBO = other.planeVerticesVBO;

	inPos = other.inPos;

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

void Engine::PostProcessProgram::configureMeshBuffers(Engine::MeshInstance * mesh)
{
	Engine::Mesh * data = mesh->getMesh();
	//glGenVertexArrays(1, &data->vao);
	glBindVertexArray(data->vao);

	//glGenBuffers(1, &mesh->vboVertices);
	glBindBuffer(GL_ARRAY_BUFFER, data->vboVertices);
	//glBufferData(GL_ARRAY_BUFFER, mesh->getMesh()->getNumVertices() * sizeof(float) * 3, mesh->getMesh()->getVertices(), GL_STATIC_DRAW);
	glVertexAttribPointer(inPos, 3, GL_FLOAT, GL_FALSE, 0, 0);

	//glGenBuffers(1, &mesh->vboUVs);
	glBindBuffer(GL_ARRAY_BUFFER, data->vboUVs);
	//glBufferData(GL_ARRAY_BUFFER, mesh->getMesh()->getNumVertices() * sizeof(float) * 2,  mesh->getMesh()->getUVs(), GL_STATIC_DRAW);

	glVertexAttribPointer(inTexCoord, 2, GL_FLOAT, GL_FALSE, 0, 0);

	glEnableVertexAttribArray(inPos);
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