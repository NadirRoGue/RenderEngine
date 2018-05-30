#include "computeprograms/WeatherTextureProgram.h"

Engine::WeatherTextureProgram::WeatherTextureProgram()
	:Engine::ComputeProgram("shaders/clouds/generation/weather.comp")
{

}

Engine::WeatherTextureProgram::WeatherTextureProgram(const Engine::WeatherTextureProgram & other)
	: Engine::ComputeProgram(other)
{
	uWeatherTex = other.uWeatherTex;
}

void Engine::WeatherTextureProgram::configureProgram()
{
	uWeatherTex = glGetUniformLocation(glProgram, "outWeatherTex");
}

void Engine::WeatherTextureProgram::bindOutput(const Engine::TextureInstance * ti)
{
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, ti->getTexture()->getTextureId());
	glBindImageTexture(0, ti->getTexture()->getTextureId(), 0, GL_TRUE, 0, GL_READ_WRITE, GL_RGBA8);
	glUniform1i(uWeatherTex, 0);
}