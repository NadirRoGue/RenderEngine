#include "programs/ProceduralWaterProgram.h"

#include "WorldConfig.h"
#include "renderers/DeferredRenderer.h"

#include <iostream>

std::string Engine::ProceduralWaterProgram::PROGRAM_NAME = "ProceduralWaterProgram";

const unsigned long long Engine::ProceduralWaterProgram::WIRE_DRAW_MODE = 0x01;
const unsigned long long Engine::ProceduralWaterProgram::SHADOW_MAP = 0x02;

Engine::ProceduralWaterProgram::ProceduralWaterProgram(std::string name, unsigned long long params)
	:Engine::Program(name, params)
{
	vShaderFile = "shaders/water/water.vert";
	gShaderFile = "shaders/water/water.geom";
	fShaderFile = "shaders/water/water.frag";
}

Engine::ProceduralWaterProgram::ProceduralWaterProgram(const Engine::ProceduralWaterProgram & other)
	:Engine::Program(other)
{
	gShader = other.gShader;

	uModelView = other.uModelView;
	uModelViewProj = other.uModelViewProj;
	uNormal = other.uNormal;

	uLightDepthMatrix = other.uLightDepthMatrix;
	uLightDepthMatrix1 = other.uLightDepthMatrix1;
	uDepthTexture = other.uDepthTexture;
	uDepthTexture1 = other.uDepthTexture1;
	uLightDirection = other.uLightDirection;

	uInInfo = other.uInInfo;
	uScreenSize = other.uScreenSize;

	uTime = other.uTime;
	uWaterColor = other.uWaterColor;
	uWaterSpeed = other.uWaterSpeed;

	uInPos = other.uInPos;
	uInUV = other.uInUV;

	uGridPos = other.uGridPos;
}

void Engine::ProceduralWaterProgram::initialize()
{
	std::string configStr = "";

	if (parameters & Engine::ProceduralWaterProgram::WIRE_DRAW_MODE)
	{
		configStr += "#define WIRE_MODE";
	}

	if (parameters & Engine::ProceduralWaterProgram::SHADOW_MAP)
	{
		configStr += "#define SHADOW_MAP";
	}

	vShader = loadShader(vShaderFile, GL_VERTEX_SHADER, configStr);

	if (parameters & Engine::ProceduralWaterProgram::WIRE_DRAW_MODE)
	{
		gShader = loadShader(gShaderFile, GL_GEOMETRY_SHADER, configStr);
	}

	fShader = loadShader(fShaderFile, GL_FRAGMENT_SHADER, configStr);

	glProgram = glCreateProgram();

	glAttachShader(glProgram, vShader);
	if (parameters & Engine::ProceduralWaterProgram::WIRE_DRAW_MODE)
	{
		glAttachShader(glProgram, gShader);
	}

	glAttachShader(glProgram, fShader);

	glLinkProgram(glProgram);

	int linked;
	glGetProgramiv(glProgram, GL_LINK_STATUS, &linked);
	if (!linked)
	{
		GLint logLen;
		glGetProgramiv(glProgram, GL_INFO_LOG_LENGTH, &logLen);
		char *logString = new char[logLen];
		glGetProgramInfoLog(glProgram, logLen, NULL, logString);
		std::cout << "Error: " << logString << std::endl;
		delete[] logString;
		exit(-1);
	}

	configureProgram();
}

void Engine::ProceduralWaterProgram::configureProgram()
{
	uModelView = glGetUniformLocation(glProgram, "modelView");
	uModelViewProj = glGetUniformLocation(glProgram, "modelViewProj");
	uNormal = glGetUniformLocation(glProgram, "normal");
	uGridPos = glGetUniformLocation(glProgram, "gridPos");

	uLightDepthMatrix = glGetUniformLocation(glProgram, "lightDepthMat");
	uLightDepthMatrix1 = glGetUniformLocation(glProgram, "lightDepthMat1");
	uDepthTexture = glGetUniformLocation(glProgram, "depthTexture");
	uDepthTexture1 = glGetUniformLocation(glProgram, "depthTexture1");
	uLightDirection = glGetUniformLocation(glProgram, "lightDir");

	uInInfo = glGetUniformLocation(glProgram, "inInfo");
	uScreenSize = glGetUniformLocation(glProgram, "screenSize");
	
	uWaterColor = glGetUniformLocation(glProgram, "watercolor");
	uWaterSpeed = glGetUniformLocation(glProgram, "waterspeed");
	uTime = glGetUniformLocation(glProgram, "time");

	uInPos = glGetAttribLocation(glProgram, "inPos");
	uInUV = glGetAttribLocation(glProgram, "inUV");
}

void Engine::ProceduralWaterProgram::setTimeUniform(float sinTime)
{
	glUniform1f(uTime, sinTime);
}

void Engine::ProceduralWaterProgram::setUniformGridPosition(unsigned int i, unsigned int j)
{
	glUniform2i(uGridPos, i, j);
}

void Engine::ProceduralWaterProgram::setUniformLightDepthMatrix(const glm::mat4 & ldm)
{
	glUniformMatrix4fv(uLightDepthMatrix, 1, GL_FALSE, &(ldm[0][0]));
}

void Engine::ProceduralWaterProgram::setUniformLightDepthMatrix1(const glm::mat4 & ldm)
{
	glUniformMatrix4fv(uLightDepthMatrix1, 1, GL_FALSE, &(ldm[0][0]));
}

void Engine::ProceduralWaterProgram::setUniformDepthTexture(const Engine::TextureInstance * depthTexture)
{
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, depthTexture->getTexture()->getTextureId());
	glUniform1i(uDepthTexture, 0);
}

void Engine::ProceduralWaterProgram::setUniformDepthTexture1(const Engine::TextureInstance * dt)
{
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, dt->getTexture()->getTextureId());
	glUniform1i(uDepthTexture1, 1);
}

void Engine::ProceduralWaterProgram::setUniformLightDirection(const glm::vec3 & lightDir)
{
	glUniform3fv(uLightDirection, 1, &lightDir[0]);
}

void Engine::ProceduralWaterProgram::setUniformGBufferInfo()
{
	Engine::DeferredRenderer * dr = static_cast<Engine::DeferredRenderer*>(Engine::RenderManager::getInstance().getRenderer());
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, dr->getGBufferInfo()->getTexture()->getTextureId());
	glUniform1i(uInInfo, 2);
	glUniform2f(uScreenSize, float(Engine::ScreenManager::SCREEN_WIDTH), float(Engine::ScreenManager::SCREEN_HEIGHT));
}

void Engine::ProceduralWaterProgram::onRenderObject(const Object * obj, const glm::mat4 & view, const glm::mat4 &proj)
{
	glm::mat4 modelView = view * obj->getModelMatrix();
	glm::mat4 modelViewProj = proj * view * obj->getModelMatrix();
	glm::mat4 normal = glm::transpose(glm::inverse(modelView));

	glUniformMatrix4fv(uModelView, 1, GL_FALSE, &(modelView[0][0]));
	glUniformMatrix4fv(uModelViewProj, 1, GL_FALSE, &(modelViewProj[0][0]));
	glUniformMatrix4fv(uNormal, 1, GL_FALSE, &(normal[0][0]));

	glUniform1f(uWaterSpeed, Engine::Settings::waterSpeed);
	glUniform3fv(uWaterColor, 1, &Engine::Settings::waterColor[0]);
}

void Engine::ProceduralWaterProgram::configureMeshBuffers(Engine::Mesh * data)
{
	data->use();

	if (uInPos != -1)
	{
		glBindBuffer(GL_ARRAY_BUFFER, data->vboVertices);
		glVertexAttribPointer(uInPos, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(uInPos);
	}

	if (uInUV != -1)
	{
		glBindBuffer(GL_ARRAY_BUFFER, data->vboUVs);
		glVertexAttribPointer(uInUV, 2, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(uInUV);
	}
}

void Engine::ProceduralWaterProgram::destroy()
{
	if (vShader != -1)
	{
		glDetachShader(glProgram, vShader);
		glDeleteShader(vShader);
	}

	if (gShader != -1)
	{
		glDetachShader(glProgram, gShader);
		glDeleteShader(gShader);
	}

	if (fShader != -1)
	{
		glDetachShader(glProgram, fShader);
		glDeleteShader(fShader);
	}

	glDeleteProgram(glProgram);
}

// ========================================================================================

Engine::Program * Engine::ProceduralWaterProgramFactory::createProgram(unsigned long long parameters)
{
	Engine::ProceduralWaterProgram * program = new Engine::ProceduralWaterProgram(Engine::ProceduralWaterProgram::PROGRAM_NAME, parameters);
	program->initialize();
	return program;
}