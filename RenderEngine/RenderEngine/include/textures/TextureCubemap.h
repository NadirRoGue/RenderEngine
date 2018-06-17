/**
* @author Nadir Román Guerrero
* @email nadir.ro.gue@gmail.com
*/
#pragma once

#include "Texture.h"

namespace Engine
{
	/**
	 * Class in charge of represent, give access, and manipulate Cube map texture types
	 */
	class TextureCubemap : public AbstractTexture
	{
	private:
		// Per-face data
		unsigned char * data[6];
		// Common to all faces width
		unsigned int tileWidth;
		// Common to all faces height
		unsigned int tileHeight;
	public:
		TextureCubemap(std::string name, unsigned int tileWidth, unsigned int tileHeight);
		~TextureCubemap();
		void setTileData(unsigned int index, unsigned char * data);

		const unsigned char * getPixelsFromTile(unsigned int index) const;
		const unsigned int getTileWidth() const;
		const unsigned int getTileHeight() const;

		void setSize(unsigned int w, unsigned int h, unsigned int d = 1);
		void uploadTexture();
		GLenum getTextureType();
	};
}