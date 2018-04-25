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

void Engine::PostProcessProgram::releaseProgramBuffers(Engine::MeshInstance * mi)
{
	/*
	if (inPos != -1) glDeleteBuffers(1, &mi->vboVertices);
	glDeleteVertexArrays(1, &mi->vao);
	*/
}

// ==========================================================================

Engine::GaussianBlurProgram::GaussianBlurProgram(std::string name)
	:Engine::PostProcessProgram(name)
{
	affectedTexels = kernel = 0;
}

Engine::GaussianBlurProgram::GaussianBlurProgram(const Engine::GaussianBlurProgram & other)
	:PostProcessProgram(other)
{
	uAffetedTexels = other.uAffetedTexels;
	uMaskSize = other.uMaskSize;
	uMask = other.uMask;
	uTexelSize = other.uTexelSize;

	affectedTexels = kernel = 0;
}

Engine::GaussianBlurProgram::~GaussianBlurProgram()
{
	if (affectedTexels != 0)
	{
		delete[] affectedTexels;
	}
}

void Engine::GaussianBlurProgram::configureProgram()
{
	PostProcessProgram::configureProgram();

	uAffetedTexels = glGetUniformLocation(glProgram, "affectedTexels");
	uMaskSize = glGetUniformLocation(glProgram, "maskSize");
	uMask = glGetUniformLocation(glProgram, "kernel");
	uTexelSize = glGetUniformLocation(glProgram, "texelSize");
}

void Engine::GaussianBlurProgram::setMaskSize(unsigned int ms)
{
	maskSize = ms;
}

void Engine::GaussianBlurProgram::setKernel(float * k)
{
	if (kernel != 0)
	{
		delete[] kernel;
	}

	kernel = new float[maskSize];

	memcpy(kernel, k, sizeof(float) * maskSize);
}

void Engine::GaussianBlurProgram::setAffectedTexels(glm::vec2 * at)
{
	if (affectedTexels != 0)
	{
		delete[] affectedTexels;
	}

	affectedTexels = new float[maskSize * 2];

	for (unsigned int i = 0; i < maskSize; i++)
	{
		glm::vec2 v = at[i];
		const unsigned int start = i * 2;
		affectedTexels[start] = v.x;
		affectedTexels[start + 1] = v.y;
	}
}

void Engine::GaussianBlurProgram::onRenderObject(const Engine::Object * obj, const glm::mat4 & view, const glm::mat4 &proj)
{
	Engine::PostProcessProgram::onRenderObject(obj, view, proj);

	glUniform1ui(uMaskSize, maskSize);
	glUniform1fv(uMask, maskSize, &kernel[0]);
	glUniform2fv(uAffetedTexels, maskSize, &affectedTexels[0]);
	float texelSize[2];
	texelSize[0] = 1.0f / ScreenManager::SCREEN_WIDTH;
	texelSize[1] = 1.0f / ScreenManager::SCREEN_HEIGHT;
	
	glUniform2fv(uTexelSize, 1, &texelSize[0]);
}

// ==========================================================================

Engine::DepthOfFieldProgram::DepthOfFieldProgram(std::string name)
	:Engine::GaussianBlurProgram(name)
{
}

Engine::DepthOfFieldProgram::DepthOfFieldProgram(const Engine::DepthOfFieldProgram & other)
	: Engine::GaussianBlurProgram(other)
{
	uFocalDistance = other.uFocalDistance;
	uMaxDistanceFactor = other.uMaxDistanceFactor;
	uInverseProj = other.uInverseProj;
}

Engine::DepthOfFieldProgram::~DepthOfFieldProgram()
{
}

void Engine::DepthOfFieldProgram::configureProgram()
{
	Engine::GaussianBlurProgram::configureProgram();

	uFocalDistance = glGetUniformLocation(glProgram, "focalDistance");
	uMaxDistanceFactor = glGetUniformLocation(glProgram, "maxDistanceFactor");
	uInverseProj = glGetUniformLocation(glProgram, "invProj");
}

void Engine::DepthOfFieldProgram::onRenderObject(const Engine::Object * obj, const glm::mat4 & view, const glm::mat4 &proj)
{
	Engine::GaussianBlurProgram::onRenderObject(obj, view, proj);

	glm::mat4 invProj = glm::inverse(proj);

	glUniform1f(uFocalDistance, focalDistance);
	glUniform1f(uMaxDistanceFactor, maxDistanceFactor);
	glUniformMatrix4fv(uInverseProj, 1, GL_FALSE, &invProj[0][0]);
}

void Engine::DepthOfFieldProgram::setFocalDistance(float fd)
{
	focalDistance = fd;
}

void Engine::DepthOfFieldProgram::setMaxDistanceFactor(float mdf)
{
	maxDistanceFactor = mdf;
}

float Engine::DepthOfFieldProgram::getFocalDistance()
{
	return focalDistance;
}

float Engine::DepthOfFieldProgram::getMaxDistanceFactor()
{
	return maxDistanceFactor;
}

// ==========================================================================

Engine::PerlinGeneratorProgram::PerlinGeneratorProgram(std::string name)
	:PostProcessProgram(name)
{
	scale = 5.0f;
	frequency = 1.0f;
	amplitude = 0.5f;
	octaves = 8;
}

Engine::PerlinGeneratorProgram::PerlinGeneratorProgram(const PerlinGeneratorProgram & other)
	: PostProcessProgram(other)
{
	perlinAmplitude = other.perlinAmplitude;
	perlinFrequency = other.perlinFrequency;
	perlinScale = other.perlinScale;
	perlinOctaves = other.perlinOctaves;

	scale = other.scale;
	frequency = other.frequency;
	amplitude = other.amplitude;
	octaves = other.octaves;
}

Engine::PerlinGeneratorProgram::~PerlinGeneratorProgram()
{

}

void Engine::PerlinGeneratorProgram::configureProgram()
{
	Engine::PostProcessProgram::configureProgram();
	perlinAmplitude = glGetUniformLocation(glProgram, "amplitude");
	perlinFrequency = glGetUniformLocation(glProgram, "frecuency");
	perlinScale = glGetUniformLocation(glProgram, "scale");
	perlinOctaves = glGetUniformLocation(glProgram, "octaves");
}

void Engine::PerlinGeneratorProgram::onRenderObject(const Object * obj, const glm::mat4 & view, const glm::mat4 &proj)
{
	Engine::PostProcessProgram::onRenderObject(obj, view, proj);
	glUniform1f(perlinAmplitude, amplitude);
	glUniform1f(perlinFrequency, frequency);
	glUniform1f(perlinOctaves, octaves);
	glUniform1f(perlinScale, scale);
}