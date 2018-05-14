#pragma once

#include "Texture.h"

namespace Engine
{
	class Texture3D : public AbstractTexture
	{
	private:
		unsigned char * data;
		unsigned int width;
		unsigned int height;
		unsigned int depth;
	public:
		Texture3D(std::string name, unsigned int w, unsigned int h, unsigned int d);
		~Texture3D();

		const unsigned char * getTexturePixels() const;
		const unsigned int getWidth() const;
		const unsigned int getHeight() const;
		const unsigned int getLayers() const;

		void setSize(unsigned int w, unsigned int h, unsigned int d);

		void uploadTexture();
		GLenum getTextureType();
	};
}