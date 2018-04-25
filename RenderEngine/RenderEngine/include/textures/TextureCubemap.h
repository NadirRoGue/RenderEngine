#pragma once

#include "Texture.h"

namespace Engine
{
	class TextureCubemap : public AbstractTexture
	{
	private:
		unsigned char * data[6];
		unsigned int tileWidth;
		unsigned int tileHeight;
	public:
		TextureCubemap(unsigned int tileWidth, unsigned int tileHeight);
		~TextureCubemap();
		void setTileData(unsigned int index, unsigned char * data);

		const unsigned char * getPixelsFromTile(unsigned int index) const;
		const unsigned int getTileWidth() const;
		const unsigned int getTileHeight() const;

		void setSize(unsigned int w, unsigned int h);
		void uploadTexture();
		GLenum getTextureType();
	};
}