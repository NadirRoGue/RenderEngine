#include "programs/StandardProgram.h"

#include "instances/TextureInstance.h"

Engine::StandarProgram::StandarProgram(std::string name)
	:Engine::Program(name)
{
}

Engine::StandarProgram::StandarProgram(const Engine::StandarProgram & other)
	: Engine::Program(other)
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

void Engine::StandarProgram::configureProgram()
{
	uNormalMat = glGetUniformLocation(glProgram, "normal");
	uModelViewMat = glGetUniformLocation(glProgram, "modelView");
	uModelViewProjMat = glGetUniformLocation(glProgram, "modelViewProj");

	uComputeShading = glGetUniformLocation(glProgram, "computeShading");
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

	inPos = glGetAttribLocation(glProgram, "inPos");
	inColor = glGetAttribLocation(glProgram, "inColor");
	inNormal = glGetAttribLocation(glProgram, "inNormal");
	inTexCoord = glGetAttribLocation(glProgram, "inTexCoord");
	inTangent = glGetAttribLocation(glProgram, "inTangent");
}

void Engine::StandarProgram::onRenderLight(const glm::mat4 & model, const glm::mat4 & view)
{
	glm::mat4 result = view * model;
	float position[3];
	position[0] = result[3][0];
	position[1] = result[3][1];
	position[2] = result[3][2];

	glUniform3fv(uPointLightPos, 1, &position[0]);
}

void Engine::StandarProgram::onRenderSpotLight(const glm::mat4 & modelPos, const glm::mat4 & modelDir, const glm::mat4 & view)
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

void Engine::StandarProgram::onRenderDirectionalLight(const glm::mat4 & model, const glm::mat4 & view)
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

void Engine::StandarProgram::onRenderObject(const Engine::Object * obj, const glm::mat4 & view, const glm::mat4 &proj)
{
	glm::mat4 modelView = view * obj->getModelMatrix();
	glm::mat4 modelViewProj = proj * view * obj->getModelMatrix();
	glm::mat4 normal = glm::transpose(glm::inverse(modelView));

	glUniform1i(uComputeShading, 0);

	glUniformMatrix4fv(uModelViewMat, 1, GL_FALSE, &(modelView[0][0]));
	glUniformMatrix4fv(uModelViewProjMat, 1, GL_FALSE, &(modelViewProj[0][0]));
	glUniformMatrix4fv(uNormalMat, 1, GL_FALSE, &(normal[0][0]));
}

void Engine::StandarProgram::configureMeshBuffers(Engine::MeshInstance * mesh)
{
	Engine::Mesh * data = mesh->getMesh();

	glBindVertexArray(data->vao);

	if (inPos != -1)
	{
		glBindBuffer(GL_ARRAY_BUFFER, data->vboVertices);
		glVertexAttribPointer(inPos, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(inPos);
	}
	if (inColor != -1)
	{
		glBindBuffer(GL_ARRAY_BUFFER, data->vboColors);
		glVertexAttribPointer(inColor, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(inColor);
	}
	if (inNormal != -1)
	{
		glBindBuffer(GL_ARRAY_BUFFER, data->vboNormals);
		glVertexAttribPointer(inNormal, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(inNormal);
	}
	if (inTexCoord != -1)
	{
		glBindBuffer(GL_ARRAY_BUFFER, data->vboUVs);
		glVertexAttribPointer(inTexCoord, 2, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(inTexCoord);
	}
	if (inTangent != -1)
	{
		glBindBuffer(GL_ARRAY_BUFFER, data->vboTangents);
		glVertexAttribPointer(inTangent, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(inTangent);
	}
}

void Engine::StandarProgram::configurePointLightBuffer(const Engine::PointLight *pl)
{
	glUseProgram(glProgram);
	glUniform3fv(uIa, 1, pl->getAmbientIntensity());
	glUniform3fv(uId, 1, pl->getDiffuseIntensity());
	glUniform3fv(uIs, 1, pl->getSpecularIntensity());
	glUniform3fv(uPLattenuation, 1, pl->getAttenuationFactor());
}

void Engine::StandarProgram::configureSpotLightBuffer(const Engine::SpotLight *sl)
{
	glUseProgram(glProgram);
	glUniform3fv(uSLIa, 1, sl->getAmbientIntensity());
	glUniform3fv(uSLId, 1, sl->getDiffuseIntensity());
	glUniform3fv(uSLIa, 1, sl->getSpecularIntensity());
	glUniform1f(uSLapperture, sl->getAppertureAngle());
	glUniform1f(uSLm, sl->getM());
	glUniform3fv(uSLattenuation, 1, sl->getAttenuationFactor());
}

void Engine::StandarProgram::configureDirectionalLightBuffer(const Engine::DirectionalLight *dl)
{
	glUseProgram(glProgram);
	glUniform3fv(uDLIa, 1, dl->getAmbientIntensity());
	glUniform3fv(uDLId, 1, dl->getDiffuseIntensity());
	glUniform3fv(uDLIs, 1, dl->getSpecularIntensity());
}

/*
void Engine::StandarProgram::releaseProgramBuffers(Engine::MeshInstance * mi)
{
if (inPos != -1) glDeleteBuffers(1, &mi->vboVertices);
if (inColor != -1) glDeleteBuffers(1, &mi->vboColors);
if (inNormal != -1) glDeleteBuffers(1, &mi->vboNormals);
if (inTexCoord != -1) glDeleteBuffers(1, &mi->vboUVs);
glDeleteBuffers(1, &mi->vboFaces);
glDeleteVertexArrays(1, &mi->vao);
}
*/
void Engine::StandarProgram::configureClearColor(const glm::vec3 & cc)
{
	glUseProgram(glProgram);
	float backgroundColor[3] = { cc.x, cc.y, cc.z };
	glUniform3fv(uBackground, 1, &backgroundColor[0]);
}

// ============================================================================

Engine::TextureProgram::TextureProgram(std::string name)
	:Engine::StandarProgram(name)
{
}

Engine::TextureProgram::TextureProgram(const Engine::TextureProgram & other)
	: Engine::StandarProgram(other)
{

}

void Engine::TextureProgram::onRenderObject(const Engine::Object * obj, const glm::mat4 & view, const glm::mat4 & proj)
{
	Engine::StandarProgram::onRenderObject(obj, view, proj);

	const Engine::TextureInstance * albedo = obj->getAlbedoTexture();
	const Engine::TextureInstance * normal = obj->getNormalMapTexture();
	const Engine::TextureInstance * specular = obj->getSpecularMapTexture();
	const Engine::TextureInstance * emissive = obj->getEmissiveTexture();

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, albedo->getTexture()->getTextureId());
	albedo->configureTexture();
	glUniform1i(uAlbedoTex, 0);

	glActiveTexture(GL_TEXTURE0 + 1);
	glBindTexture(GL_TEXTURE_2D, emissive->getTexture()->getTextureId());
	emissive->configureTexture();
	glUniform1i(uEmissiveTex, 1);

	glActiveTexture(GL_TEXTURE0 + 2);
	glBindTexture(GL_TEXTURE_2D, specular->getTexture()->getTextureId());
	specular->configureTexture();
	glUniform1i(uSpecTex, 2);

	glActiveTexture(GL_TEXTURE0 + 3);
	glBindTexture(GL_TEXTURE_2D, normal->getTexture()->getTextureId());
	normal->configureTexture();
	glUniform1i(uNormalTex, 3);
}

void Engine::TextureProgram::configureMeshBuffers(Engine::MeshInstance * mesh)
{
	Engine::StandarProgram::configureMeshBuffers(mesh);

	uAlbedoTex = glGetUniformLocation(glProgram, "colorTex");
	uEmissiveTex = glGetUniformLocation(glProgram, "emiTex");
	uSpecTex = glGetUniformLocation(glProgram, "specTex");
	uNormalTex = glGetUniformLocation(glProgram, "normalTex");
}