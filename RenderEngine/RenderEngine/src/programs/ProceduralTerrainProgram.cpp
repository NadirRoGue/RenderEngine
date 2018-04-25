#include "programs/ProceduralTerrainProgram.h"

#include <iostream>

Engine::ProceduralTerrainProgram::ProceduralTerrainProgram(std::string name) :Program(name)
{
}

Engine::ProceduralTerrainProgram::ProceduralTerrainProgram(const ProceduralTerrainProgram & other)
	: Program(other)
{
	uModelView = other.uModelView;
	uModelViewProj = other.uModelViewProj;
	uNormal = other.uNormal;

	uInPos = other.uInPos;
	uInUV = other.uInUV;

	uGridPos = other.uGridPos;
}

void Engine::ProceduralTerrainProgram::initialize(std::string vertexShader, std::string fragmentShader)
{
	vShader = loadShader("shaders/terrain/terrain.vert", GL_VERTEX_SHADER);
	tcsShader = loadShader("shaders/terrain/terrain.tesctrl", GL_TESS_CONTROL_SHADER);
	tevalShader = loadShader("shaders/terrain/terrain.teseval", GL_TESS_EVALUATION_SHADER);
	gShader = loadShader("shaders/terrain/terrain.geom", GL_GEOMETRY_SHADER);
	fShader = loadShader("shaders/terrain/terrain.frag", GL_FRAGMENT_SHADER);

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

void Engine::ProceduralTerrainProgram::configureProgram()
{
	uModelView = glGetUniformLocation(glProgram, "modelView");
	uModelViewProj = glGetUniformLocation(glProgram, "modelViewProj");
	uNormal = glGetUniformLocation(glProgram, "normal");
	uGridPos = glGetUniformLocation(glProgram, "gridPos");

	uInPos = glGetAttribLocation(glProgram, "inPos");
	uInUV = glGetAttribLocation(glProgram, "inUV");
}

void Engine::ProceduralTerrainProgram::configureMeshBuffers(Engine::MeshInstance * mesh)
{
	Engine::Mesh * data = mesh->getMesh();
	glBindVertexArray(data->vao);

	unsigned int numFaces = mesh->getMesh()->getNumFaces();
	unsigned int numVertex = mesh->getMesh()->getNumVertices();

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

void Engine::ProceduralTerrainProgram::onRenderObject(const Engine::Object * obj, const glm::mat4 & view, const glm::mat4 &proj)
{
	glm::mat4 modelView = view * obj->getModelMatrix();
	glm::mat4 modelViewProj = proj * view * obj->getModelMatrix();
	glm::mat4 normal = glm::transpose(glm::inverse(modelView));

	glUniformMatrix4fv(uModelView, 1, GL_FALSE, &(modelView[0][0]));
	glUniformMatrix4fv(uModelViewProj, 1, GL_FALSE, &(modelViewProj[0][0]));
	glUniformMatrix4fv(uNormal, 1, GL_FALSE, &(normal[0][0]));

	unsigned int vertexPerFace = obj->getMesh()->getNumVerticesPerFace();
	glPatchParameteri(GL_PATCH_VERTICES, vertexPerFace);
}

void Engine::ProceduralTerrainProgram::setUniformGridPosition(unsigned int i, unsigned int j)
{
	glUniform2i(uGridPos, i, j);
}

void Engine::ProceduralTerrainProgram::destroy()
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