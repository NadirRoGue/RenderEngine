/**
* @author Nadir Román Guerrero
* @email nadir.ro.gue@gmail.com
*/

#include "PostProcessProgram.h"

#include "Renderer.h"

#include <iostream>

using namespace Engine;

PostProcessProgram::PostProcessProgram(std::string name)
	:Program(name)
{
}

PostProcessProgram::PostProcessProgram(const PostProcessProgram & other)
	:Program(other)
{
	planeVAO = other.planeVAO;
	planeVerticesVBO = other.planeVerticesVBO;

	inPos = other.inPos;

	memcpy(uRenderedTextures, other.uRenderedTextures, 9 * sizeof(unsigned int));
}

PostProcessProgram::~PostProcessProgram()
{
}

void PostProcessProgram::configureProgram()
{
	for (int i = 0; i < 9; i++)
	{
		std::string uniformName = "postProcessing_" + std::to_string(i);
		uRenderedTextures[i] = glGetUniformLocation(glProgram, uniformName.c_str());
	}

	inPos = glGetAttribLocation(glProgram, "inPos");
	inTexCoord = glGetAttribLocation(glProgram, "inTexCoord");
}

void PostProcessProgram::configureMeshBuffers(MeshInstance * mesh)
{
	glGenVertexArrays(1, &mesh->vao);
	glBindVertexArray(mesh->vao);

	glGenBuffers(1, &mesh->vboVertices);
	glBindBuffer(GL_ARRAY_BUFFER, mesh->vboVertices);
	glBufferData(GL_ARRAY_BUFFER, mesh->getMesh()->getNumVertices() * sizeof(float) * 3,
		mesh->getMesh()->getVertices(), GL_STATIC_DRAW);

	glVertexAttribPointer(inPos, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glGenBuffers(1, &mesh->vboUVs);
	glBindBuffer(GL_ARRAY_BUFFER, mesh->vboUVs);
	glBufferData(GL_ARRAY_BUFFER, mesh->getMesh()->getNumVertices() * sizeof(float) * 2, 
		mesh->getMesh()->getUVs(), GL_STATIC_DRAW);

	glVertexAttribPointer(inTexCoord, 2, GL_FLOAT, GL_FALSE, 0, 0);

	glEnableVertexAttribArray(inPos);
	glEnableVertexAttribArray(inTexCoord);
}

void PostProcessProgram::configureClearColor(const glm::vec3 & cc)
{
	// Does nothing
}

void PostProcessProgram::onRenderLight(const glm::mat4 & model, const glm::mat4 & view)
{
}

void PostProcessProgram::onRenderSpotLight(const glm::mat4 & modelPos, const glm::mat4 & modelDir, const glm::mat4 & view)
{
}

void PostProcessProgram::onRenderDirectionalLight(const glm::mat4 & model, const glm::mat4 & view)
{
}

void PostProcessProgram::configurePointLightBuffer(const PointLight *pl)
{
}

void PostProcessProgram::configureSpotLightBuffer(const SpotLight *sl)
{
}

void PostProcessProgram::configureDirectionalLightBuffer(const DirectionalLight *dl)
{
}

void PostProcessProgram::onRenderObject(const Object * obj, const glm::mat4 & view, const glm::mat4 &proj)
{

	std::map<std::string, TextureInstance *> all = obj->getAllCustomTextures();
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

void PostProcessProgram::releaseProgramBuffers(MeshInstance * mi)
{
	if (inPos != -1) glDeleteBuffers(1, &mi->vboVertices);
	glDeleteVertexArrays(1, &mi->vao);
}

// ==========================================================================

GaussianBlurProgram::GaussianBlurProgram(std::string name)
	:PostProcessProgram(name)
{
	affectedTexels = kernel = 0;
}

GaussianBlurProgram::GaussianBlurProgram(const GaussianBlurProgram & other)
	:PostProcessProgram(other)
{
	uAffetedTexels = other.uAffetedTexels;
	uMaskSize = other.uMaskSize;
	uMask = other.uMask;
	uTexelSize = other.uTexelSize;

	affectedTexels = kernel = 0;
}

GaussianBlurProgram::~GaussianBlurProgram()
{
	if (affectedTexels != 0)
	{
		delete[] affectedTexels;
	}
}

void GaussianBlurProgram::configureProgram()
{
	PostProcessProgram::configureProgram();

	uAffetedTexels = glGetUniformLocation(glProgram, "affectedTexels");
	uMaskSize = glGetUniformLocation(glProgram, "maskSize");
	uMask = glGetUniformLocation(glProgram, "kernel");
	uTexelSize = glGetUniformLocation(glProgram, "texelSize");
}

void GaussianBlurProgram::setMaskSize(unsigned int ms)
{
	maskSize = ms;
}

void GaussianBlurProgram::setKernel(float * k)
{
	if (kernel != 0)
	{
		delete[] kernel;
	}

	kernel = new float[maskSize];

	memcpy(kernel, k, sizeof(float) * maskSize);
}

void GaussianBlurProgram::setAffectedTexels(glm::vec2 * at)
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

void GaussianBlurProgram::onRenderObject(const Object * obj, const glm::mat4 & view, const glm::mat4 &proj)
{
	PostProcessProgram::onRenderObject(obj, view, proj);

	glUniform1ui(uMaskSize, maskSize);
	glUniform1fv(uMask, maskSize, &kernel[0]);
	glUniform2fv(uAffetedTexels, maskSize, &affectedTexels[0]);
	float texelSize[2];
	texelSize[0] = 1.0f / ScreenManager::SCREEN_WIDTH;
	texelSize[1] = 1.0f / ScreenManager::SCREEN_HEIGHT;
	
	glUniform2fv(uTexelSize, 1, &texelSize[0]);
}

// ==========================================================================

DepthOfFieldProgram::DepthOfFieldProgram(std::string name)
	:GaussianBlurProgram(name)
{
}

DepthOfFieldProgram::DepthOfFieldProgram(const DepthOfFieldProgram & other)
	:GaussianBlurProgram(other)
{
	uFocalDistance = other.uFocalDistance;
	uMaxDistanceFactor = other.uMaxDistanceFactor;
	uInverseProj = other.uInverseProj;
}

DepthOfFieldProgram::~DepthOfFieldProgram()
{
}

void DepthOfFieldProgram::configureProgram()
{
	GaussianBlurProgram::configureProgram();

	uFocalDistance = glGetUniformLocation(glProgram, "focalDistance");
	uMaxDistanceFactor = glGetUniformLocation(glProgram, "maxDistanceFactor");
	uInverseProj = glGetUniformLocation(glProgram, "invProj");
}

void DepthOfFieldProgram::onRenderObject(const Object * obj, const glm::mat4 & view, const glm::mat4 &proj)
{
	GaussianBlurProgram::onRenderObject(obj, view, proj);

	glm::mat4 invProj = glm::inverse(proj);

	glUniform1f(uFocalDistance, focalDistance);
	glUniform1f(uMaxDistanceFactor, maxDistanceFactor);
	glUniformMatrix4fv(uInverseProj, 1, GL_FALSE, &invProj[0][0]);
}

void DepthOfFieldProgram::setFocalDistance(float fd)
{
	focalDistance = fd;
}

void DepthOfFieldProgram::setMaxDistanceFactor(float mdf)
{
	maxDistanceFactor = mdf;
}

float DepthOfFieldProgram::getFocalDistance()
{
	return focalDistance;
}

float DepthOfFieldProgram::getMaxDistanceFactor()
{
	return maxDistanceFactor;
}

// ==========================================================================

DepthRenderProgram::DepthRenderProgram(std::string name)
	:PostProcessProgram(name)
{
}

DepthRenderProgram::DepthRenderProgram(const DepthRenderProgram & other)
	:PostProcessProgram(other)
{
	uInverseProj = other.uInverseProj;
}

DepthRenderProgram::~DepthRenderProgram()
{
}

void DepthRenderProgram::configureProgram()
{
	PostProcessProgram::configureProgram();

	uInverseProj = glGetUniformLocation(glProgram, "invProj");
}

void DepthRenderProgram::onRenderObject(const Object * obj, const glm::mat4 & view, const glm::mat4 &proj)
{
	PostProcessProgram::onRenderObject(obj, view, proj);

	glm::mat4 invProj = glm::inverse(proj);
	glUniformMatrix4fv(uInverseProj, 1, GL_FALSE, &invProj[0][0]);
}

// ==========================================================================

DeferredShadingProgram::DeferredShadingProgram(std::string name)
	:PostProcessProgram(name)
{

}

DeferredShadingProgram::DeferredShadingProgram(const DeferredShadingProgram & other)
	:PostProcessProgram(other)
{
	uPointLightPos = other.uPointLightPos;
	uIa = other.uIa;
	uId = other.uId;
	uIs = other.uIs;
	uPLattenuation = other.uPLattenuation;

	uSpotLightPos = other.uSpotLightPos;
	uSpotLightDir = other.uSpotLightDir;
	uSLIa = other.uSLIa;
	uSLId = other.uSLId;
	uSLIs = other.uSLIs;
	uSLapperture = other.uSLapperture;
	uSLm = other.uSLm;
	uSLattenuation = other.uSLattenuation;

	uDirectionalLightDir = other.uDirectionalLightDir;
	uDLIa = other.uDLIa;
	uDLId = other.uDLId;
	uDLIs = other.uDLIs;

	uBackground = other.uBackground;
}

void DeferredShadingProgram::onRenderLight(const glm::mat4 & model, const glm::mat4 & view)
{
	glm::mat4 result = view * model;
	float position[3];
	position[0] = result[3][0];
	position[1] = result[3][1];
	position[2] = result[3][2];
	
	glUniform3fv(uPointLightPos, 1, &position[0]);
}

void DeferredShadingProgram::onRenderSpotLight(const glm::mat4 & modelPos, const glm::mat4 & modelDir, const glm::mat4 & view)
{
	glm::mat4 resultPos = view * modelPos;
	float position[3];
	position[0] = resultPos[3][0];
	position[1] = resultPos[3][1];
	position[2] = resultPos[3][2];

	glUniform3fv(uSpotLightPos, 1, &position[0]);

	glm::mat4 resultDir = view * modelDir;
	float direction[3];
	direction[0] = resultDir[3][0];
	direction[1] = resultDir[3][1];
	direction[2] = resultDir[3][2];

	glUniform3fv(uSpotLightDir, 1, &direction[0]);
}

void DeferredShadingProgram::onRenderDirectionalLight(const glm::mat4 & model, const glm::mat4 & view)
{
	glm::mat4 modelCopy = model;
	modelCopy[3][3] = 0.0f;
	glm::mat4 resultPos = view * modelCopy;

	glm::vec3 direction(resultPos[3][0], resultPos[3][1], resultPos[3][2]);
	direction = glm::normalize(direction);

	float position[3];
	position[0] = direction.x;
	position[1] = direction.y;
	position[2] = direction.z;

	glUniform3fv(uDirectionalLightDir, 1, &position[0]);
}

void DeferredShadingProgram::configureProgram()
{
	PostProcessProgram::configureProgram();

	uBackground = glGetUniformLocation(glProgram, "backgroundColor");

	uIa = glGetUniformLocation(glProgram, "Ia");
	uId = glGetUniformLocation(glProgram, "Id");
	uIs = glGetUniformLocation(glProgram, "Is");
	uPointLightPos = glGetUniformLocation(glProgram, "lpos");
	uPLattenuation = glGetUniformLocation(glProgram, "PLattenuation");

	uSLIa = glGetUniformLocation(glProgram, "SLIa");
	uSLId = glGetUniformLocation(glProgram, "SLId");
	uSLIs = glGetUniformLocation(glProgram, "SLIs");
	uSpotLightPos = glGetUniformLocation(glProgram, "SLpos");
	uSpotLightDir = glGetUniformLocation(glProgram, "SLdir");
	uSLapperture = glGetUniformLocation(glProgram, "SLapperture");
	uSLm = glGetUniformLocation(glProgram, "SLm");
	uSLattenuation = glGetUniformLocation(glProgram, "SLattenuation");

	uDLIa = glGetUniformLocation(glProgram, "DLIa");
	uDLId = glGetUniformLocation(glProgram, "DLId");
	uDLIs = glGetUniformLocation(glProgram, "DLIs");
	uDirectionalLightDir = glGetUniformLocation(glProgram, "DLdir");
}

void DeferredShadingProgram::configurePointLightBuffer(const PointLight *pl)
{
	glUseProgram(glProgram);
	glUniform3fv(uIa, 1, pl->getAmbientIntensity());
	glUniform3fv(uId, 1, pl->getDiffuseIntensity());
	glUniform3fv(uIs, 1, pl->getSpecularIntensity());
	glUniform3fv(uPLattenuation, 1, pl->getAttenuationFactor());
}

void DeferredShadingProgram::configureSpotLightBuffer(const SpotLight *sl)
{
	glUseProgram(glProgram);
	glUniform3fv(uSLIa, 1, sl->getAmbientIntensity());
	glUniform3fv(uSLId, 1, sl->getDiffuseIntensity());
	glUniform3fv(uSLIa, 1, sl->getSpecularIntensity());
	glUniform1f(uSLapperture, sl->getAppertureAngle());
	glUniform1f(uSLm, sl->getM());
	glUniform3fv(uSLattenuation, 1, sl->getAttenuationFactor());
}

void DeferredShadingProgram::configureDirectionalLightBuffer(const DirectionalLight *dl)
{
	glUseProgram(glProgram);
	glUniform3fv(uDLIa, 1, dl->getAmbientIntensity());
	glUniform3fv(uDLId, 1, dl->getDiffuseIntensity());
	glUniform3fv(uDLIs, 1, dl->getSpecularIntensity());
}

void DeferredShadingProgram::configureClearColor(const glm::vec3 & cc)
{
	glUseProgram(glProgram);
	float backgroundColor[3] = { cc.x, cc.y, cc.z };
	glUniform3fv(uBackground, 1, &backgroundColor[0]);
}

// =============================================================================

EdgeBasedProgram::EdgeBasedProgram(std::string name)
	:PostProcessProgram(name)
{

}

EdgeBasedProgram::EdgeBasedProgram(const EdgeBasedProgram & other)
	: PostProcessProgram(other)
{
	uTexelSize = other.uTexelSize;
}

EdgeBasedProgram::~EdgeBasedProgram()
{

}

void EdgeBasedProgram::configureProgram()
{
	PostProcessProgram::configureProgram();

	uTexelSize = glGetUniformLocation(glProgram, "texelSize");
}

void EdgeBasedProgram::onRenderObject(const Object * obj, const glm::mat4 & view, const glm::mat4 &proj)
{
	PostProcessProgram::onRenderObject(obj, view, proj);

	float texelSize[2];
	texelSize[0] = 1.0f / ScreenManager::SCREEN_WIDTH;
	texelSize[1] = 1.0f / ScreenManager::SCREEN_HEIGHT;

	glUniform2fv(uTexelSize, 1, &texelSize[0]);
}