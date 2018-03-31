/**
 * @author Nadir Román Guerrero
 * @email nadir.ro.gue@gmail.com
 */

#include "Program.h"

#include "auxiliar.h"
#include "Mesh.h"

#include "Renderer.h"

#include <iostream>

Engine::Program::Program(std::string name)
{
	this->name = std::string(name);
}

Engine::Program::Program(const Engine::Program & other)
{
	glProgram = other.glProgram;
	vShader = other.vShader;
	fShader = other.fShader;
}

/*
void Engine::Program::initialize(std::string vertexShader, std::string fragmentShader)
{
	// Creamos el shader de vértices
	vShader = loadShader(vertexShader, GL_VERTEX_SHADER);
	// Creamos el shader de fragmentos
	fShader = loadShader(fragmentShader, GL_FRAGMENT_SHADER);

	// Creamos un programa para enlazar los shader anteriores
	glProgram = glCreateProgram();

	// Enlacamos los shader
	glAttachShader(glProgram, vShader);
	glAttachShader(glProgram, fShader);

	// Linkamos el programa
	glLinkProgram(glProgram);

	// Comprobamos errores de linkado (E.G., una varaible in que usamos en el shader 
	// de fragmentos pero que no existe en el de vértices)
	int linked;
	glGetProgramiv(glProgram, GL_LINK_STATUS, &linked);
	if (!linked)
	{
		//Calculamos una cadena de error
		GLint logLen;
		glGetProgramiv(glProgram, GL_INFO_LOG_LENGTH, &logLen);
		char *logString = new char[logLen];
		glGetProgramInfoLog(glProgram, logLen, NULL, logString);
		std::cout << "Error: " << logString << std::endl;
		delete[] logString;
		//glDeleteProgram(program);
		//program = 0;
		exit(-1);
	}

	configureProgram();
}
*/
Engine::Program::~Program()
{
}

unsigned int Engine::Program::getProgramId() const
{
	return glProgram;
}

unsigned int Engine::Program::loadShader(std::string fileName, GLenum type, std::string config)
{
	unsigned int fileLen;
	char * source = loadStringFromFile(fileName.c_str(), fileLen);

	char * finalSource = new char[strlen(source) + config.length];
	strcpy(finalSource, config.c_str());
	strcat(finalSource, source);
	fileLen += config.length;

	delete[] source;

	GLuint shader = glCreateShader(type);
	glShaderSource(shader, 1, (const GLchar **)&finalSource, (const GLint *)&fileLen);
	glCompileShader(shader);
	
	delete[] finalSource;

	GLint compiled;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
	if (!compiled)
	{
		GLint logLen;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLen);
		char *logString = new char[logLen];
		glGetShaderInfoLog(shader, logLen, NULL, logString);
		std::cout << "Error: " << logString << std::endl;
		delete[] logString;
		glDeleteShader(shader);
		exit(-1);
	}

	return shader;
}

void Engine::Program::configureDirectionalLightBuffer(const Engine::DirectionalLight *dl)
{
}

void Engine::Program::configurePointLightBuffer(const Engine::PointLight *pl)
{
}

void Engine::Program::configureSpotLightBuffer(const Engine::SpotLight *sl)
{
}

std::string Engine::Program::getName() const
{
	return name;
}

void Engine::Program::onRenderLight(const glm::mat4 & model, const glm::mat4 & view)
{

}

void Engine::Program::onRenderSpotLight(const glm::mat4 & modelPos, const glm::mat4 & modelDir, const glm::mat4 & view)
{

}

void Engine::Program::onRenderDirectionalLight(const glm::mat4 & model, const glm::mat4 & view)
{

}

void Engine::Program::onRenderObject(const Engine::Object * obj, const glm::mat4 & view, const glm::mat4 &proj)
{

}

// ==============================================================================
// ==============================================================================

Engine::StandarProgram::StandarProgram(std::string name, unsigned long flags)
	:Engine::Program(name)
{
	this->flags = flags;
}

Engine::StandarProgram::StandarProgram(const Engine::StandarProgram & other)
	: Engine::Program(other)
{
	flags = other.flags;

	tcsShader = other.tcsShader;
	tevalShader = other.tevalShader;
	gShader = other.gShader;

	inPos = other.inPos;
	inNormal = other.inNormal;
	inTexCoord = other.inTexCoord;
	inColor = other.inColor;
	inTangent = other.inTangent;
	
	/*
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
	*/
}

Engine::StandarProgram::~StandarProgram()
{

}

std::string Engine::StandarProgram::createConfigString()
{
	std::string configStr = "";

	if (flags & Engine::StandarProgram::ALBEDO_BIT)
	{
		configStr += "#define ALBEDO_MAPPING\n";
	}

	if (flags & Engine::StandarProgram::NORMAL_MAP_BIT)
	{
		configStr += "#define NORMAL_MAPPING\n";
	}

	if (flags & Engine::StandarProgram::SPECULAR_MAP_BIT)
	{
		configStr += "#define SPECULAR_MAPPING\n";
	}

	if (flags & Engine::StandarProgram::EMISSIVE_MAP_BIT)
	{
		configStr += "#define EMISSIVE_MAPPING\n";
	}

	if (flags & Engine::StandarProgram::OCCLUSION_MAP_BIT)
	{
		configStr += "#define OCCLUSION_MAPPING\n";
	}

	if (flags & Engine::StandarProgram::DISPLACEMENT_MAP_BIT)
	{
		configStr += "#define DISPLACEMENT_MAPPING\n";
	}

	if (flags & Engine::StandarProgram::AUTOLOD_BIT)
	{
		configStr += "#define AUTOLOD\n";
	}

	return configStr;
}

void Engine::StandarProgram::initialize()
{
	glProgram = glCreateProgram();

	std::string config = createConfigString();

	vShader = loadShader("shaders/standard.vert", GL_VERTEX_SHADER, config);
	glAttachShader(glProgram, vShader);

	if (flags & Engine::StandarProgram::AUTOLOD_BIT)
	{
		tcsShader = loadShader("shaders/standard.tessctrl", GL_TESS_CONTROL_SHADER, config);
		glAttachShader(glProgram, tcsShader);

		tevalShader = loadShader("shaders/standard.tesseval", GL_TESS_EVALUATION_SHADER, config);
		glAttachShader(glProgram, tevalShader);
	}

	if (flags & Engine::StandarProgram::DISPLACEMENT_MAP_BIT)
	{
		gShader = loadShader("shaders/standard.geom", GL_GEOMETRY_SHADER, config);
		glAttachShader(glProgram, gShader);
	}
	
	fShader = loadShader("shaders/standard.frag", GL_FRAGMENT_SHADER, config);
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
		glDeleteProgram(glProgram);
		glProgram = 0;
		exit(-1);
	}

	configureProgram();
}

void Engine::StandarProgram::configureProgram()
{
	// Obtenemos el identificador para las variables uniformes
	uNormalMat = glGetUniformLocation(glProgram, "normal");
	uModelViewMat = glGetUniformLocation(glProgram, "modelView");
	uModelViewProjMat = glGetUniformLocation(glProgram, "modelViewProj");

	/*
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
	*/

	inPos = glGetAttribLocation(glProgram, "inPos");
	inColor = glGetAttribLocation(glProgram, "inColor");
	inNormal = glGetAttribLocation(glProgram, "inNormal");
	inTexCoord = glGetAttribLocation(glProgram, "inUV");
	inTangent = glGetAttribLocation(glProgram, "inTangent");
}

void Engine::StandarProgram::configureMeshBuffers(Engine::MeshInstance * mesh)
{
	glGenVertexArrays(1, &mesh->vao);
	glBindVertexArray(mesh->vao);

	unsigned int numFaces = mesh->getMesh()->getNumFaces();
	unsigned int numVertex = mesh->getMesh()->getNumVertices();

	if (inPos != -1)
	{
		glGenBuffers(1, &mesh->vboVertices);
		glBindBuffer(GL_ARRAY_BUFFER, mesh->vboVertices);
		glBufferData(GL_ARRAY_BUFFER, numVertex * sizeof(float) * 3, mesh->getMesh()->getVertices(), GL_STATIC_DRAW);
		glVertexAttribPointer(inPos, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(inPos);
	}
	if (inColor != -1)
	{
		glGenBuffers(1, &mesh->vboColors);
		glBindBuffer(GL_ARRAY_BUFFER, mesh->vboColors);
		glBufferData(GL_ARRAY_BUFFER, numVertex * sizeof(float) * 3, mesh->getMesh()->getColor(), GL_STATIC_DRAW);
		glVertexAttribPointer(inColor, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(inColor);
	}
	if (inNormal != -1)
	{
		glGenBuffers(1, &mesh->vboNormals);
		glBindBuffer(GL_ARRAY_BUFFER, mesh->vboNormals);
		glBufferData(GL_ARRAY_BUFFER, numVertex * sizeof(float) * 3, mesh->getMesh()->getNormals(), GL_STATIC_DRAW);
		glVertexAttribPointer(inNormal, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(inNormal);
	}
	if (inTexCoord != -1)
	{
		glGenBuffers(1, &mesh->vboUVs);
		glBindBuffer(GL_ARRAY_BUFFER, mesh->vboUVs);
		glBufferData(GL_ARRAY_BUFFER, numVertex * sizeof(float) * 2, mesh->getMesh()->getUVs(), GL_STATIC_DRAW);
		glVertexAttribPointer(inTexCoord, 2, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(inTexCoord);
	}
	if (inTangent != -1)
	{
		glGenBuffers(1, &mesh->vboTangents);
		glBindBuffer(GL_ARRAY_BUFFER, mesh->vboTangents);
		glBufferData(GL_ARRAY_BUFFER, numVertex * sizeof(float) * 3, mesh->getMesh()->getTangetns(), GL_STATIC_DRAW);
		glVertexAttribPointer(inTangent, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(inTangent);
	}

	glGenBuffers(1, &mesh->vboFaces);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->vboFaces);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, numFaces * sizeof(unsigned int) * 3, mesh->getMesh()->getFaces(), GL_STATIC_DRAW);
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

void Engine::StandarProgram::configureClearColor(const glm::vec3 & cc)
{
	glUseProgram(glProgram);
	float backgroundColor[3] = { cc.x, cc.y, cc.z };
	glUniform3fv(uBackground, 1, &backgroundColor[0]);
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

	glUniform1i(uComputeShading, RenderManager::getInstance().isForwardRendering() ? 1 : 0);

	glUniformMatrix4fv(uModelViewMat, 1, GL_FALSE, &(modelView[0][0]));
	glUniformMatrix4fv(uModelViewProjMat, 1, GL_FALSE, &(modelViewProj[0][0]));
	glUniformMatrix4fv(uNormalMat, 1, GL_FALSE, &(normal[0][0]));
}

void Engine::StandarProgram::releaseProgramBuffers(Engine::MeshInstance * mi)
{
	if (inPos != -1) glDeleteBuffers(1, &mi->vboVertices);
	if (inColor != -1) glDeleteBuffers(1, &mi->vboColors);
	if (inNormal != -1) glDeleteBuffers(1, &mi->vboNormals);
	if (inTexCoord != -1) glDeleteBuffers(1, &mi->vboUVs);
	glDeleteBuffers(1, &mi->vboFaces);
	glDeleteVertexArrays(1, &mi->vao);
}

// ============================================================================

Engine::TextureProgram::TextureProgram(std::string name)
	:Engine::StandarProgram(name)
{
}

Engine::TextureProgram::TextureProgram(const Engine::TextureProgram & other)
	:Engine::StandarProgram(other)
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
	glBindTexture(GL_TEXTURE_2D,normal->getTexture()->getTextureId());
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