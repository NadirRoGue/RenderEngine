#include "programs/ProceduralWaterProgram.h"

#include <chrono>
#include <iostream>

std::string Engine::ProceduralWaterProgram::PROGRAM_NAME = "ProceduralWaterProgram";

Engine::ProceduralWaterProgram::ProceduralWaterProgram(std::string name, unsigned long long params)
	:Engine::ProceduralTerrainProgram(name, params)
{
	vShaderFile = "shaders/water/water.vert";
	fShaderFile = "shaders/water/water.frag";

	tcsShaderFile = "shaders/water/water.tesctrl";
	tevalShaderFile = "shaders/water/water.teseval";
	gShaderFile = "shaders/water/water.geom";
}

Engine::ProceduralWaterProgram::ProceduralWaterProgram(const Engine::ProceduralWaterProgram & other)
	:Engine::ProceduralTerrainProgram(other)
{
}

void Engine::ProceduralWaterProgram::configureProgram()
{
	Engine::ProceduralTerrainProgram::configureProgram();

	uTime = glGetUniformLocation(glProgram, "time");
}

void Engine::ProceduralWaterProgram::setTimeUniform(float sinTime)
{
	glUniform1f(uTime, sinTime);
}

// ========================================================================================

Engine::Program * Engine::ProceduralWaterProgramFactory::createProgram(unsigned long long parameters)
{
	Engine::ProceduralWaterProgram * program = new Engine::ProceduralWaterProgram(Engine::ProceduralWaterProgram::PROGRAM_NAME, parameters);
	program->initialize();
	return program;
}