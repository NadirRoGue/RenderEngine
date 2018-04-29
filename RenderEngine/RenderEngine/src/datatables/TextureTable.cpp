#include "datatables/TextureTable.h"

#include <iostream>

#include "textures/Texture2D.h"
#include "textures/TextureCubemap.h"

#include <FreeImage.h>
#define _CRT_SECURE_DEPRECATE_MEMORY
#include <memory.h>

unsigned char *loadTexture(const char* fileName, unsigned int &w, unsigned int &h)
{
	FreeImage_Initialise(TRUE);

	FREE_IMAGE_FORMAT format = FreeImage_GetFileType(fileName, 0);
	if (format == FIF_UNKNOWN)
		format = FreeImage_GetFIFFromFilename(fileName);
	if ((format == FIF_UNKNOWN) || !FreeImage_FIFSupportsReading(format))
		return NULL;

	FIBITMAP* img = FreeImage_Load(format, fileName);
	if (img == NULL)
		return NULL;

	FIBITMAP* tempImg = img;
	img = FreeImage_ConvertTo32Bits(img);
	FreeImage_Unload(tempImg);

	w = FreeImage_GetWidth(img);
	h = FreeImage_GetHeight(img);

	//BGRA a RGBA
	unsigned char * map = new unsigned char[4 * w*h];
	char *buff = (char*)FreeImage_GetBits(img);

	for (unsigned int j = 0; j<w*h; j++) {
		map[j * 4 + 0] = buff[j * 4 + 2];
		map[j * 4 + 1] = buff[j * 4 + 1];
		map[j * 4 + 2] = buff[j * 4 + 0];
		map[j * 4 + 3] = buff[j * 4 + 3];
	}

	FreeImage_Unload(img);
	FreeImage_DeInitialise();

	return map;
}

// ================================================================================

unsigned char * readTextureFromFile(std::string file, unsigned int & width, unsigned int & height, std::string debugFailMsg)
{
	unsigned char *data = loadTexture(file.c_str(), width, height);
	if (data == 0)
	{
		std::cout << "TextureTable: error reading " << file << " " << debugFailMsg << std::endl;
		return NULL;
	}

	return data;
}

// ================================================================================

Engine::TextureTable * Engine::TextureTable::INSTANCE = new Engine::TextureTable();

Engine::TextureTable & Engine::TextureTable::getInstance()
{
	return *INSTANCE;
}

Engine::TextureTable::TextureTable()
{
}

Engine::TextureTable::~TextureTable()
{
}

void Engine::TextureTable::checkForAnisotropicFilterSupport()
{
	maxAnisotropicFilterLevel = 0.0f;
	anisotropicFilteringSupport = glewIsSupported("GL_EXT_texture_filter_anisotropic");
	if (anisotropicFilteringSupport)
	{
		glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &maxAnisotropicFilterLevel);
		std::cout << "TextureTable: Maximun anisotropic level supported: " << maxAnisotropicFilterLevel << std::endl;
	}
	else
	{
		std::cout << "TextureTable: Texture anisotropic filtering is not supported" << std::endl;
	}
}

void Engine::TextureTable::cacheTexture(std::string filename, std::string name)
{
	std::map<std::string, Engine::AbstractTexture *>::iterator it = textureTable.find(name);
	if (it != textureTable.end())
	{
		std::cout << "TextureTable: attempt to load duplicate texture " << name << std::endl;
		return;
	}

	unsigned int width, height;
	unsigned char *data = readTextureFromFile(filename, width, height, "");

	if (!data)
	{
		return;
	}

	Engine::Texture2D * texture = new Engine::Texture2D(data, width, height);
	texture->generateTexture();
	texture->uploadTexture();
	textureTable[name] = texture;

	delete[] data;
}

void Engine::TextureTable::cacheCubemapTexture(CubemapLoadData & cubemapData, std::string name)
{
	std::map<std::string, Engine::AbstractTexture *>::iterator it = textureTable.find(name);
	if (it != textureTable.end())
	{
		std::cout << "TextureTable: attempt to load duplicate texture " << name << std::endl;
		return;
	}

	unsigned int width, height;

	unsigned char *rightData = readTextureFromFile(cubemapData.rightFace, width, height, "(Cubemap right face)");
	if (rightData == NULL) return;
	unsigned char *leftData = readTextureFromFile(cubemapData.leftFace, width, height, "(Cubemap left face)");
	if (leftData == NULL) return;
	unsigned char *topData = readTextureFromFile(cubemapData.topFace, width, height, "(Cubemap top face)");
	if (topData == NULL) return;
	unsigned char *bottomData = readTextureFromFile(cubemapData.bottomFace, width, height, "(Cubemap bottom face)");
	if (bottomData == NULL) return;
	unsigned char *frontData = readTextureFromFile(cubemapData.frontFace, width, height, "(Cubemap front face)");
	if (frontData == NULL) return;
	unsigned char *backData = readTextureFromFile(cubemapData.backFace, width, height, "(Cubemap back face)");
	if (backData == NULL) return;

	Engine::TextureCubemap * texture = new Engine::TextureCubemap(width, height);
	texture->setTileData(0, rightData);
	texture->setTileData(1, leftData);
	texture->setTileData(2, bottomData);
	texture->setTileData(3, topData);
	texture->setTileData(4, frontData);
	texture->setTileData(5, backData);
	texture->generateTexture();
	texture->uploadTexture();
	textureTable[name] = texture;

	delete[] rightData;
	delete[] leftData;
	delete[] topData;
	delete[] bottomData;
	delete[] frontData;
	delete[] backData;
}

Engine::TextureInstance * Engine::TextureTable::instantiateTexture(std::string name)
{
	std::map<std::string, Engine::AbstractTexture *>::iterator it = textureTable.find(name);

	if (it == textureTable.end())
	{
		std::cout << "TextureTable: Could not instantiate texture " << name << " - does not exist on cache" << std::endl;
		return NULL;
	}

	return new Engine::TextureInstance(it->second);
}

const bool Engine::TextureTable::isAnisotropicFilteringSupported() const
{
	return anisotropicFilteringSupport;
}

const float Engine::TextureTable::getMaxAnisotropicFilterLevel() const
{
	return maxAnisotropicFilterLevel;
}

void Engine::TextureTable::clean()
{
	std::map<std::string, Engine::AbstractTexture *>::iterator it = textureTable.begin();
	while (it != textureTable.end())
	{
		unsigned int id = it->second->getTextureId();
		glDeleteTextures(1, &id);
		it++;
	}

	textureTable.clear();
	std::cout << "Texture table cleared" << std::endl;
}