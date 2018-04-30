#include "postprocessprograms/DeferredShadingProgram.h"

#include "Scene.h"

std::string Engine::DeferredShadingProgram::PROGRAM_NAME = "DeferredShadingProgram";

Engine::DeferredShadingProgram::DeferredShadingProgram(std::string name, unsigned long long params)
	:Engine::PostProcessProgram(name, params)
{
	fShaderFile = "shaders/postprocess/DeferredShading.frag";
}

Engine::DeferredShadingProgram::DeferredShadingProgram(const Engine::DeferredShadingProgram & other)
	: Engine::PostProcessProgram(other)
{
	uDirectionalLightDir = other.uDirectionalLightDir;
	uDLIa = other.uDLIa;
	uDLId = other.uDLId;
	uDLIs = other.uDLIs;

	uDLBuffer = other.uDLBuffer;
	uPLBuffer = other.uPLBuffer;
	uSLBuffer = other.uSLBuffer;

	uBackground = other.uBackground;
}

void Engine::DeferredShadingProgram::initializeLightBuffers()
{

	lightBufferInitialized = true;
}

void Engine::DeferredShadingProgram::onRenderObject(const Engine::Object * obj, const glm::mat4 & view, const glm::mat4 &proj)
{
	// FIXME - UGLY INITIALIZATION
	if (!lightBufferInitialized)
		initializeLightBuffers();

	Engine::PostProcessProgram::onRenderObject(obj, view, proj);

	Engine::Scene * scene = Engine::SceneManager::getInstance().getActiveScene();
	if (scene != 0)
	{
		Engine::DirectionalLight * dl = scene->getDirectionalLight();

		static float IA[3] = { 0.15f, 0.15f, 0.15f };
		static float ID[3] = { 1.f, 1.f, 1.f };

		glUniform3fv(uDLIa, 1, IA);
		glUniform3fv(uDLId, 1, ID);
		glUniform3fv(uDLIs, 1, ID);

		glm::mat4 modelCopy = dl->getModelMatrix();
		modelCopy[3][3] = 0.0f;
		glm::mat4 resultPos = view * modelCopy;

		glm::vec3 direction(resultPos[3][0], resultPos[3][1], resultPos[3][2]);
		direction = glm::normalize(direction);

		float position[3];
		position[0] = direction.x;
		position[1] = direction.y;
		position[2] = direction.z;

		glUniform3fv(uDirectionalLightDir, 1, &position[0]);

		glUniform3fv(uBackground, 1, &scene->getClearColor()[0]);
	}
}

/*
void Engine::DeferredShadingProgram::onRenderLight(const glm::mat4 & model, const glm::mat4 & view)
{
	glm::mat4 result = view * model;
	float position[3];
	position[0] = result[3][0];
	position[1] = result[3][1];
	position[2] = result[3][2];

	glUniform3fv(uPointLightPos, 1, &position[0]);
}

void Engine::DeferredShadingProgram::onRenderSpotLight(const glm::mat4 & modelPos, const glm::mat4 & modelDir, const glm::mat4 & view)
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

void Engine::DeferredShadingProgram::onRenderDirectionalLight(const glm::mat4 & model, const glm::mat4 & view)
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
*/
void Engine::DeferredShadingProgram::configureProgram()
{
	Engine::PostProcessProgram::configureProgram();

	uBackground = glGetUniformLocation(glProgram, "backgroundColor");

	uDLBuffer = glGetUniformBlockIndex(glProgram, "DLBuffer");
	uPLBuffer = glGetUniformBlockIndex(glProgram, "PLBuffer");
	uSLBuffer = glGetUniformBlockIndex(glProgram, "SLBuffer");

	uDLIa = glGetUniformLocation(glProgram, "DLIa");
	uDLId = glGetUniformLocation(glProgram, "DLId");
	uDLIs = glGetUniformLocation(glProgram, "DLIs");
	uDirectionalLightDir = glGetUniformLocation(glProgram, "DLdir");
}

/*
void Engine::DeferredShadingProgram::configurePointLightBuffer(const Engine::PointLight *pl)
{
	glUseProgram(glProgram);
	glUniform3fv(uIa, 1, pl->getAmbientIntensity());
	glUniform3fv(uId, 1, pl->getDiffuseIntensity());
	glUniform3fv(uIs, 1, pl->getSpecularIntensity());
	glUniform3fv(uPLattenuation, 1, pl->getAttenuationFactor());
}

void Engine::DeferredShadingProgram::configureSpotLightBuffer(const Engine::SpotLight *sl)
{
	glUseProgram(glProgram);
	glUniform3fv(uSLIa, 1, sl->getAmbientIntensity());
	glUniform3fv(uSLId, 1, sl->getDiffuseIntensity());
	glUniform3fv(uSLIa, 1, sl->getSpecularIntensity());
	glUniform1f(uSLapperture, sl->getAppertureAngle());
	glUniform1f(uSLm, sl->getM());
	glUniform3fv(uSLattenuation, 1, sl->getAttenuationFactor());
}

void Engine::DeferredShadingProgram::configureDirectionalLightBuffer(const Engine::DirectionalLight *dl)
{
	glUseProgram(glProgram);
	glUniform3fv(uDLIa, 1, dl->getAmbientIntensity());
	glUniform3fv(uDLId, 1, dl->getDiffuseIntensity());
	glUniform3fv(uDLIs, 1, dl->getSpecularIntensity());
}

void Engine::DeferredShadingProgram::configureClearColor(const glm::vec3 & cc)
{
	glUseProgram(glProgram);
	float backgroundColor[3] = { cc.x, cc.y, cc.z };
	glUniform3fv(uBackground, 1, &backgroundColor[0]);
}
*/
// =====================================================

Engine::Program * Engine::DeferredShadingProgramFactory::createProgram(unsigned long long parameters)
{
	Engine::DeferredShadingProgram * program = new Engine::DeferredShadingProgram(Engine::DeferredShadingProgram::PROGRAM_NAME, parameters);
	program->initialize();
	return program;
}