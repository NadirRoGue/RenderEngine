/*
* @author Nadir Román Guerrero
* @email nadir.ro.gue@gmail.com
*/

#pragma once

#include <string>
#include <map>

#include <GL/glew.h>

namespace Engine
{
	class AbstractTexture
	{
	protected:
		unsigned int textureId;

		int internalFormat;	// How do we want to store the texture in the gpu memory?
		GLenum formatType;	// Whats the format of the image we got form disk (what does each element we read mean)?
		GLenum pixelType;	// Hows each element of the texture codified? (3 floats, 1 unsigned int, 1 unsigned char,...)
		bool generateMipMaps;
	public:
		const unsigned int getTextureId() const;

		void setMemoryLayoutFormat(const int format);
		void setImageFormatType(const GLenum type);
		void setPixelFormatType(const GLenum type);
		void setGenerateMipMaps(bool val);

		const int getMemoryLayoutFormat() const;
		const GLenum getImageFormat() const;
		const GLenum getPixelFormat() const;

		void generateTexture();
		virtual void uploadTexture() = 0;
		virtual void setSize(unsigned int w, unsigned int h) = 0;
		virtual GLenum getTextureType() = 0;
	};
}