/**
* @author Nadir Román Guerrero
* @email nadir.ro.gue@gmail.com
*/

#pragma once

#include <map>
#include <string>

#include "Texture.h"
#include "instances/TextureInstance.h"

#include "StorageTable.h"

namespace Engine
{
	// GL_TEXTURE_CUBEMAP specific input structure
	typedef struct CubemapLoadData
	{
		// right face texture filename
		std::string rightFace;
		// left face texture filename
		std::string leftFace;
		// bottom face texture filename
		std::string bottomFace;
		// top face texture filename
		std::string topFace;
		// front face texture filename
		std::string frontFace;
		// back face texture filename
		std::string backFace;
	} CubemapLoadData;

	/**
	 * Class in charge to instantiate and give access to textures
	 */
	class TextureTable : public StorageTable
	{
	private:
		static TextureTable * INSTANCE;

		// Loaded textures
		std::map<std::string, AbstractTexture *> textureTable;

		// Flags which holds whether there is anisotropic filter support for texture
		bool anisotropicFilteringSupport;
		// Holds the max anisotropic filter level supported
		float maxAnisotropicFilterLevel;

	private:
		TextureTable();
	public:
		static TextureTable & getInstance();

		~TextureTable();

		// Queries the GPU Drive to check for anisotropic support
		void checkForAnisotropicFilterSupport();

		// Attempts to load a texture from a file name and stores it with the given name
		void cacheTexture(std::string fileName, std::string name);

		// Attempts to load a cubemap from the given CubeMapLoadData input, and stores it with the given name
		void cacheCubemapTexture(CubemapLoadData & cubemapData, std::string name);

		// Instantiates an existing texture
		TextureInstance * instantiateTexture(std::string name);

		// Returns wethere anisotropic filter is supported
		const bool isAnisotropicFilteringSupported() const;

		// Returns the max level of anisotropic filtering
		const float getMaxAnisotropicFilterLevel() const;

		// Releases the textures from GPU and CPU
		void clean();
	};
}