#pragma once

#include <map>
#include <string>

#include "Texture.h"
#include "instances/TextureInstance.h"

#include "StorageTable.h"

namespace Engine
{
	typedef struct CubemapLoadData
	{
		std::string rightFace;
		std::string leftFace;
		std::string bottomFace;
		std::string topFace;
		std::string frontFace;
		std::string backFace;
	} CubemapLoadData;

	class TextureTable : public StorageTable
	{
	private:
		static TextureTable * INSTANCE;

		std::map<std::string, AbstractTexture *> textureTable;

		bool anisotropicFilteringSupport;
		float maxAnisotropicFilterLevel;

	private:
		TextureTable();
	public:
		static TextureTable & getInstance();

		~TextureTable();

		void checkForAnisotropicFilterSupport();
		void cacheTexture(std::string fileName, std::string name);
		void cacheCubemapTexture(CubemapLoadData & cubemapData, std::string name);
		TextureInstance * instantiateTexture(std::string name);
		const bool isAnisotropicFilteringSupported() const;
		const float getMaxAnisotropicFilterLevel() const;
		void clean();
	};
}