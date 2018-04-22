/**
 * @author Nadir Román Guerrero
 * @email nadir.ro.gue@gmail.com
 */

#include "Program.h"

#include "auxiliar.h"
#include "Mesh.h"
#include "Texture.h"
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

Engine::Program::~Program()
{
}

unsigned int Engine::Program::getProgramId() const
{
	return glProgram;
}

unsigned int Engine::Program::loadShader(std::string fileName, GLenum type)
{
	unsigned int fileLen;
	// Cargamos en memoria el código del shader
	char *source = loadStringFromFile(fileName.c_str(), fileLen);

	//////////////////////////////////////////////
	//Creación y compilación del Shader
	GLuint shader;
	// Creamos un objecto de tipo shader en el contexto (obtenemos su identificador)
	shader = glCreateShader(type);
	// Establecemos el código del shader
	glShaderSource(shader, 1, (const GLchar **)&source, (const GLint *)&fileLen);
	// Compilamos el shader
	glCompileShader(shader);
	// Liberamos la memoria de la cadena del código
	delete[] source;

	//Comprobamos que se compiló bien
	GLint compiled;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
	if (!compiled)
	{
		//Calculamos una cadena de error
		GLint logLen;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLen);
		char *logString = new char[logLen];
		glGetShaderInfoLog(shader, logLen, NULL, logString);
		std::cout << "Error on " << fileName << ": " << logString << std::endl;
		delete[] logString;
		glDeleteShader(shader); // No necesario, pues después salimos de la aplicación
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

void Engine::Program::destroy()
{
	glDetachShader(glProgram, vShader);
	glDeleteShader(vShader);

	glDetachShader(glProgram, fShader);
	glDeleteShader(fShader);

	glDeleteProgram(glProgram);
}

// ==============================================================================
// ==============================================================================

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
	// Obtenemos el identificador para las variables uniformes
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

	glUniform1i(uComputeShading, RenderManager::getInstance().isForwardRendering() ? 1 : 0);

	glUniformMatrix4fv(uModelViewMat, 1, GL_FALSE, &(modelView[0][0]));
	glUniformMatrix4fv(uModelViewProjMat, 1, GL_FALSE, &(modelViewProj[0][0]));
	glUniformMatrix4fv(uNormalMat, 1, GL_FALSE, &(normal[0][0]));
}

void Engine::StandarProgram::configureMeshBuffers(Engine::MeshInstance * mesh)
{
	// Generamos el VAO
	//glGenVertexArrays(1, &mesh->vao);
	// Activamos el VAO para poder modificarlo
	// Tras activarlo, cada llamada a la configuración de un vao se hará
	// sobre el VAO activo
	Engine::Mesh * data = mesh->getMesh();

	glBindVertexArray(data->vao);

	// Generamos los distintos VBOs
	if (inPos != -1)
	{
		// Creo
		//glGenBuffers(1, &mesh->vboVertices);
		// Activo
		glBindBuffer(GL_ARRAY_BUFFER, data->vboVertices);
		// Subimos los datos a la tarjeta gráfica
		//glBufferData(GL_ARRAY_BUFFER, numVertex * sizeof(float) * 3, mesh->getMesh()->getVertices(), GL_STATIC_DRAW);
		// Configuramos el VAO (le decimos que este buffer va en la posición inPos en le VAO activo)
		glVertexAttribPointer(inPos, 3, GL_FLOAT, GL_FALSE, 0, 0);
		// Activamos el atributo inPos para que lo utilice
		glEnableVertexAttribArray(inPos);
	}
	if (inColor != -1)
	{
		//glGenBuffers(1, &mesh->vboColors);
		glBindBuffer(GL_ARRAY_BUFFER, data->vboColors);
		//glBufferData(GL_ARRAY_BUFFER, numVertex * sizeof(float) * 3, mesh->getMesh()->getColor(), GL_STATIC_DRAW);
		glVertexAttribPointer(inColor, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(inColor);
	}
	if (inNormal != -1)
	{
		//glGenBuffers(1, &mesh->vboNormals);
		glBindBuffer(GL_ARRAY_BUFFER, data->vboNormals);
		//glBufferData(GL_ARRAY_BUFFER, numVertex * sizeof(float) * 3, mesh->getMesh()->getNormals(), GL_STATIC_DRAW);
		glVertexAttribPointer(inNormal, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(inNormal);
	}
	if (inTexCoord != -1)
	{
		//glGenBuffers(1, &mesh->vboUVs);
		glBindBuffer(GL_ARRAY_BUFFER, data->vboUVs);
		//glBufferData(GL_ARRAY_BUFFER, numVertex * sizeof(float) * 2, mesh->getMesh()->getUVs(), GL_STATIC_DRAW);
		glVertexAttribPointer(inTexCoord, 2, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(inTexCoord);
	}
	if (inTangent != -1)
	{
		//glGenBuffers(1, &mesh->vboTangents);
		glBindBuffer(GL_ARRAY_BUFFER, data->vboTangents);
		//glBufferData(GL_ARRAY_BUFFER, numVertex * sizeof(float) * 3, mesh->getMesh()->getTangetns(), GL_STATIC_DRAW);
		glVertexAttribPointer(inTangent, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(inTangent);
	}

	//glGenBuffers(1, &mesh->vboFaces);
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->vboFaces);
	//glBufferData(GL_ELEMENT_ARRAY_BUFFER, numFaces * sizeof(unsigned int) * 3, mesh->getMesh()->getFaces(), GL_STATIC_DRAW);
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

void Engine::StandarProgram::releaseProgramBuffers(Engine::MeshInstance * mi)
{
	/*
	if (inPos != -1) glDeleteBuffers(1, &mi->vboVertices);
	if (inColor != -1) glDeleteBuffers(1, &mi->vboColors);
	if (inNormal != -1) glDeleteBuffers(1, &mi->vboNormals);
	if (inTexCoord != -1) glDeleteBuffers(1, &mi->vboUVs);
	glDeleteBuffers(1, &mi->vboFaces);
	glDeleteVertexArrays(1, &mi->vao);
	*/
}

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

// =================================================================================

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
	//glGenVertexArrays(1, &mesh->vao);
	Engine::Mesh * data = mesh->getMesh();
	glBindVertexArray(data->vao);

	unsigned int numFaces = mesh->getMesh()->getNumFaces();
	unsigned int numVertex = mesh->getMesh()->getNumVertices();

	if (uInPos != -1)
	{
		//glGenBuffers(1, &mesh->vboVertices);
		glBindBuffer(GL_ARRAY_BUFFER, data->vboVertices);
		//glBufferData(GL_ARRAY_BUFFER, numVertex * sizeof(float) * 3, mesh->getMesh()->getVertices(), GL_STATIC_DRAW);
		glVertexAttribPointer(uInPos, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(uInPos);
	}

	if (uInUV != -1)
	{
		//glGenBuffers(1, &mesh->vboUVs);
		glBindBuffer(GL_ARRAY_BUFFER, data->vboUVs);
		//glBufferData(GL_ARRAY_BUFFER, numVertex * sizeof(float) * 2, mesh->getMesh()->getUVs(), GL_STATIC_DRAW);
		glVertexAttribPointer(uInUV, 2, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(uInUV);
	}

	//glGenBuffers(1, &mesh->vboFaces);
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->vboFaces);
	//glBufferData(GL_ELEMENT_ARRAY_BUFFER, numFaces * sizeof(unsigned int) * mesh->getMesh()->getNumVerticesPerFace(), mesh->getMesh()->getFaces(), GL_STATIC_DRAW);
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