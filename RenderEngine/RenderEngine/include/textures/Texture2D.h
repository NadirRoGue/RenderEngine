#pragma once

#include "Texture.h"

namespace Engine
{
	class Texture2D : public AbstractTexture
	{
	private:
		unsigned char * data;
		unsigned int width;
		unsigned int height;
	public:
		Texture2D(std::string name, unsigned char *data, unsigned int width, unsigned int height);
		~Texture2D();

		const unsigned char * getTexturePixels() const;
		const unsigned int getWidth() const;
		const unsigned int getHeight() const;

		void setSize(unsigned int w, unsigned int h);

		void uploadTexture();
		GLenum getTextureType();
	};
}