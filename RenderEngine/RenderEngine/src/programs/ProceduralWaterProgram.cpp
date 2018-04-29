#include "programs/ProceduralWaterProgram.h"

#include "WorldConfig.h"

#include <iostream>

std::string Engine::ProceduralWaterProgram::PROGRAM_NAME = "ProceduralWaterProgram";

Engine::ProceduralWaterProgram::ProceduralWaterProgram(std::string name, unsigned long long params)
	:Engine::Program(name, params)
{
	vShaderFile = "shaders/water/water.vert";
	fShaderFile = "shaders/water/water.frag";

	tcsShaderFile = "shaders/water/water.tesctrl";
	tevalShaderFile = "shaders/water/water.teseval";
	gShaderFile = "shaders/water/water.geom";
}

Engine::ProceduralWaterProgram::ProceduralWaterProgram(const Engine::ProceduralWaterProgram & other)
	:Engine::Program(other)
{
	tcsShaderFile = other.tcsShaderFile;
	tevalShaderFile = other.tevalShaderFile;
	gShaderFile = other.gShaderFile;

	uModelView = other.uModelView;
	uModelViewProj = other.uModelViewProj;
	uNormal = other.uNormal;

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

	if (parameters & Engine::ProceduralTerrainProgram::WIRE_DRAW_MODE)
	{
		configStr += "#define WIRE_MODE";
	}

	vShader = loadShader(vShaderFile, GL_VERTEX_SHADER);
	tcsShader = loadShader(tcsShaderFile, GL_TESS_CONTROL_SHADER);
	tevalShader = loadShader(tevalShaderFile, GL_TESS_EVALUATION_SHADER);
	gShader = loadShader(gShaderFile, GL_GEOMETRY_SHADER, configStr);
	fShader = loadShader(fShaderFile, GL_FRAGMENT_SHADER, configStr);

	glProgram = glCreateProgram();

	glAttachShader(glProgram, vShader);
	glAttachShader(glProgram, tcsShader);
	glAttachShader(glProgram, tevalShader);
	glAttachShader(glProgram, gShader);
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

	unsigned int vertexPerFace = obj->getMesh()->getNumVerticesPerFace();
	glPatchParameteri(GL_PATCH_VERTICES, vertexPerFace);
}

void Engine::ProceduralWaterProgram::configureMeshBuffers(Engine::Mesh * data)
{
	glBindVertexArray(data->vao);

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
	glDetachShader(glProgram, vShader);
	glDeleteShader(vShader);

	glDetachShader(glProgram, tcsShader);
	glDeleteShader(tcsShader);

	glDetachShader(glProgram, tevalShader);
	glDeleteShader(tevalShader);

	glDetachShader(glProgram, gShader);
	glDeleteShader(gShader);

	glDetachShader(glProgram, fShader);
	glDeleteShader(fShader);

	glDeleteProgram(glProgram);
}

// ========================================================================================

Engine::Program * Engine::ProceduralWaterProgramFactory::createProgram(unsigned long long parameters)
{
	Engine::ProceduralWaterProgram * program = new Engine::ProceduralWaterProgram(Engine::ProceduralWaterProgram::PROGRAM_NAME, parameters);
	program->initialize();
	return program;
}