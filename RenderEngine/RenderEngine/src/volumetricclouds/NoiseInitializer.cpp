#include "volumetricclouds/NoiseInitializer.h"

#include <gl/glew.h>

#include "datatables/ProgramTable.h"
#include "textures/Texture2D.h"
#include "textures/Texture3D.h"

const Engine::CloudSystem::NoiseInitializer * Engine::CloudSystem::NoiseInitializer::INSTANCE = new Engine::CloudSystem::NoiseInitializer();

Engine::CloudSystem::NoiseInitializer::NoiseInitializer()
{
	generator = NULL;
	PerlinWorleyFBM = NULL;
	WorleyFBM = NULL;
	CurlNoise = NULL;
	WeatherData = NULL;
}

const Engine::TextureInstance * Engine::CloudSystem::NoiseInitializer::getPerlinWorleyFBM() const
{
	return PerlinWorleyFBM;
}

const Engine::TextureInstance * Engine::CloudSystem::NoiseInitializer::getWorleyFBM() const
{
	return WorleyFBM;
}

const Engine::TextureInstance * Engine::CloudSystem::NoiseInitializer::getCurlNoise() const
{
	return CurlNoise;
}

const Engine::TextureInstance * Engine::CloudSystem::NoiseInitializer::getWeatherData() const
{
	return WeatherData;
}

void Engine::CloudSystem::NoiseInitializer::init()
{
	clean();

	initShader();
	initTextures();
	initializeFBO();
}

void Engine::CloudSystem::NoiseInitializer::initShader()
{
	if (generator == NULL)
	{
		generator = dynamic_cast<Engine::VolumetricTextureProgram*>
		(
			Engine::ProgramTable::getInstance().getProgramByName(Engine::VolumetricTextureProgram::PROGRAM_NAME)
		);
	}
}

void Engine::CloudSystem::NoiseInitializer::initTextures()
{
	Engine::Texture3D * perlinWorley = new Engine::Texture3D("perlinworleyfbm", 128, 128, 128);
	perlinWorley->setGenerateMipMaps(false);
	perlinWorley->setMemoryLayoutFormat(GL_RGBA8);
	perlinWorley->setImageFormatType(GL_RGBA);
	perlinWorley->setPixelFormatType(GL_FLOAT);

	PerlinWorleyFBM = new Engine::TextureInstance(perlinWorley);
	PerlinWorleyFBM->setAnisotropicFilterEnabled(false);
	PerlinWorleyFBM->setRComponentWrapType(GL_CLAMP_TO_EDGE);
	PerlinWorleyFBM->setSComponentWrapType(GL_CLAMP_TO_EDGE);
	PerlinWorleyFBM->setTComponentWrapType(GL_CLAMP_TO_EDGE);
	PerlinWorleyFBM->setMagnificationFilterType(GL_NEAREST);
	PerlinWorleyFBM->setMinificationFilterType(GL_NEAREST);
	PerlinWorleyFBM->generateTexture();
	PerlinWorleyFBM->configureTexture();

	PWFBMDepth = createDepthTexture(128, 128);

	Engine::Texture3D * worley = new Engine::Texture3D("worleyfbm", 32, 32, 32);
	worley->setGenerateMipMaps(false);
	worley->setMemoryLayoutFormat(GL_RGBA8);
	worley->setImageFormatType(GL_RGBA);
	worley->setPixelFormatType(GL_FLOAT);

	WorleyFBM = new Engine::TextureInstance(worley);
	WorleyFBM->setAnisotropicFilterEnabled(false);
	WorleyFBM->setRComponentWrapType(GL_CLAMP_TO_EDGE);
	WorleyFBM->setSComponentWrapType(GL_CLAMP_TO_EDGE);
	WorleyFBM->setTComponentWrapType(GL_CLAMP_TO_EDGE);
	WorleyFBM->setMagnificationFilterType(GL_NEAREST);
	WorleyFBM->setMinificationFilterType(GL_NEAREST);
	WorleyFBM->generateTexture();
	WorleyFBM->configureTexture();

	WFBMDepth = createDepthTexture(32, 32);

	Engine::Texture2D * curl = new Engine::Texture2D("curl", 0, 128, 128);
	curl->setGenerateMipMaps(false);
	curl->setMemoryLayoutFormat(GL_RGBA8);
	curl->setImageFormatType(GL_RGBA);
	curl->setPixelFormatType(GL_FLOAT);

	CurlNoise = new Engine::TextureInstance(curl);
	CurlNoise->setAnisotropicFilterEnabled(false);
	CurlNoise->setSComponentWrapType(GL_CLAMP_TO_EDGE);
	CurlNoise->setTComponentWrapType(GL_CLAMP_TO_EDGE);
	CurlNoise->setMagnificationFilterType(GL_NEAREST);
	CurlNoise->setMinificationFilterType(GL_NEAREST);
	CurlNoise->generateTexture();
	CurlNoise->configureTexture();

	CNDepth = createDepthTexture(128, 128);

	Engine::Texture2D * weather = new Engine::Texture2D("weather", 0, 1024, 1024);
	weather->setGenerateMipMaps(false);
	weather->setMemoryLayoutFormat(GL_RGBA8);
	weather->setImageFormatType(GL_RGBA);
	weather->setPixelFormatType(GL_FLOAT);

	WeatherData = new Engine::TextureInstance(weather);
	WeatherData->setAnisotropicFilterEnabled(false);
	WeatherData->setSComponentWrapType(GL_CLAMP_TO_EDGE);
	WeatherData->setTComponentWrapType(GL_CLAMP_TO_EDGE);
	WeatherData->setMagnificationFilterType(GL_NEAREST);
	WeatherData->setMinificationFilterType(GL_NEAREST);
	WeatherData->generateTexture();
	WeatherData->configureTexture();

	WDDepth = createDepthTexture(1024, 1024);
}

Engine::TextureInstance * Engine::CloudSystem::NoiseInitializer::createDepthTexture(unsigned int w, unsigned int h)
{
	Engine::Texture2D * texture = new Engine::Texture2D("", 0, w, h);
	texture->setGenerateMipMaps(false);
	texture->setMemoryLayoutFormat(GL_DEPTH_COMPONENT24);
	texture->setImageFormatType(GL_DEPTH_COMPONENT);
	texture->setPixelFormatType(GL_FLOAT);

	Engine::TextureInstance * textureInstance = new Engine::TextureInstance(texture);
	textureInstance->setAnisotropicFilterEnabled(false);
	textureInstance->setMagnificationFilterType(GL_NEAREST);
	textureInstance->setMinificationFilterType(GL_NEAREST);
	textureInstance->setSComponentWrapType(GL_CLAMP_TO_EDGE);
	textureInstance->setTComponentWrapType(GL_CLAMP_TO_EDGE);

	textureInstance->generateTexture();
	textureInstance->configureTexture();

	return textureInstance;
}

void Engine::CloudSystem::NoiseInitializer::initializeFBO()
{
	GLenum drawBuffers[1] = { GL_COLOR_ATTACHMENT0 };

	glGenFramebuffers(1, &uPWFbo);
	glBindFramebuffer(GL_FRAMEBUFFER, uPWFbo);
	glFramebufferTexture3D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_3D, PerlinWorleyFBM->getTexture()->getTextureId(), 0, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, PWFBMDepth->getTexture()->getTextureId(), 0);
	glDrawBuffers(1, drawBuffers);

	glGenFramebuffers(1, &uWFbo);
	glBindFramebuffer(GL_FRAMEBUFFER, uWFbo);
	glFramebufferTexture3D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_3D, WorleyFBM->getTexture()->getTextureId(), 0, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, WFBMDepth->getTexture()->getTextureId(), 0);
	glDrawBuffers(1, drawBuffers);

	glGenFramebuffers(1, &uCFbo);
	glBindFramebuffer(GL_FRAMEBUFFER, uCFbo);
	glFramebufferTexture3D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_3D, CurlNoise->getTexture()->getTextureId(), 0, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, CNDepth->getTexture()->getTextureId(), 0);
	glDrawBuffers(1, drawBuffers);

	glGenFramebuffers(1, &uWFbo);
	glBindFramebuffer(GL_FRAMEBUFFER, uWFbo);
	glFramebufferTexture3D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_3D, WeatherData->getTexture()->getTextureId(), 0, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, WDDepth->getTexture()->getTextureId(), 0);
	glDrawBuffers(1, drawBuffers);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Engine::CloudSystem::NoiseInitializer::clean()
{
	
}

void Engine::CloudSystem::NoiseInitializer::render() const
{

}