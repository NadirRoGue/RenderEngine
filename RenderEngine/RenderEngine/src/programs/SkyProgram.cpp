#include "programs/SkyProgram.h"

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
	uCubeMap = other.uCubeMap;
	uLightDir = other.uLightDir;
	inPos = other.inPos;
}

void Engine::SkyProgram::configureProgram()
{
	uProjMatrix = glGetUniformLocation(glProgram, "proj");
	uCubeMap = glGetUniformLocation(glProgram, "skybox");
	uLightDir = glGetUniformLocation(glProgram, "lightDir");
	inPos = glGetAttribLocation(glProgram, "inPos");
}

void Engine::SkyProgram::configureMeshBuffers(Engine::Mesh * m)
{
	glBindVertexArray(m->vao);

	if (inPos != -1)
	{
		glBindBuffer(GL_ARRAY_BUFFER, m->vboVertices);
		glVertexAttribPointer(inPos, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(inPos);
	}
}

void Engine::SkyProgram::onRenderObject(const Engine::Object * obj, const glm::mat4 & view, const glm::mat4 &proj)
{
	glm::mat4 modelViewProj = proj * view * obj->getModelMatrix();
	glUniformMatrix4fv(uProjMatrix, 1, GL_FALSE, &(modelViewProj[0][0]));
}

void Engine::SkyProgram::setCubemapUniform(Engine::TextureInstance * t)
{
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, t->getTexture()->getTextureId());
	glUniform1i(uCubeMap, 0);

}

void Engine::SkyProgram::setLightDirUniform(glm::vec3 & lightDir)
{
	glUniform3fv(uLightDir, 1, &lightDir[0]);
}

// =====================================================================

Engine::Program * Engine::SkyProgramFactory::createProgram(unsigned long long parameters)
{
	Engine::SkyProgram * program = new Engine::SkyProgram(Engine::SkyProgram::PROGRAM_NAME, parameters);
	program->initialize();
	return program;
}