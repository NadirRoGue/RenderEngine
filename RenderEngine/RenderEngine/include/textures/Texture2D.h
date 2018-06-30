/**
* @author Nadir Román Guerrero
* @email nadir.ro.gue@gmail.com
*/
#pragma once

#include "Texture.h"

namespace Engine
{
	/**
	 * Class used to represent, give access, and manipulate 2D Textures
	 */
	class Texture2D : public AbstractTexture
	{
	private:
		// Texture pixel data 
		unsigned char * data;
		// Texture width in pixels
		unsigned int width;
		// Texture height in pixels
		unsigned int height;
	public:
		Texture2D(std::string name, unsigned char *data, unsigned int width, unsigned int height);
		~Texture2D();

		const unsigned char * getTexturePixels() const;
		const unsigned int getWidth() const;
		const unsigned int getHeight() const;

		void setSize(unsigned int w, unsigned int h, unsigned int d = 1);

		void uploadTexture();
		GLenum getTextureType();
	};
}