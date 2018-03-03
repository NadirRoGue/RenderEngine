/*
* @author Nadir Román Guerrero
* @email nadir.ro.gue@gmail.com
*/

#pragma once

#include <string>
#include <map>

#include <GL/glew.h>

#include "auxiliar.h"

namespace Engine
{
	class Texture
	{
	private:
		std::string fileName;

		unsigned int textureId;
		
		unsigned char * data;
		unsigned int width;
		unsigned int height;

		int internalFormat;	// How do we want to store the texture in the gpu memory?
		GLenum formatType;	// Whats the format of the image we got form disk (what does each element we read mean)?
		GLenum pixelType;	// Hows each element of the texture codified? (3 floats, 1 unsigned int, 1 unsigned char,...)

		bool generateMipMaps;

	public:
		Texture(std::string file, unsigned char *data, unsigned int width, unsigned int height);
		~Texture();

		const std::string & getName() const;
		const unsigned char * getTexturePixels() const;
		const unsigned int getWidth() const;
		const unsigned int getHeight() const;

		const unsigned int getTextureId() const;

		void setMemoryLayoutFormat(const int format);
		void setImageFormatType(const GLenum type);
		void setPixelFormatType(const GLenum type);
		void setGenerateMipMaps(bool val);
		
		const int getMemoryLayoutFormat() const;
		const GLenum getImageFormat() const;
		const GLenum getPixelFormat() const;

		void setSize(unsigned int w, unsigned int h);

		void generateTexture();
		void uploadTexture();
	};

	 // ===================================================================

	class TextureInstance
	{
	private:
		Texture * texture;

		float minificationFilter;
		float magnificationFilter;
		float tComponentWrapType;
		float sComponentWrapType;

		bool applyAnisotropicFiltering;

		unsigned int textureIndex;

	public:
		TextureInstance(Texture * texture);
		~TextureInstance();

		const Texture * getTexture() const;
		void generateTexture();
		void uploadTexture();

		void setMinificationFilterType(const float min);
		void setMagnificationFilterType(const float mag);
		void setTComponentWrapType(const float wrapT);
		void setSComponentWrapType(const float wrapS);
		void setTextureIndex(unsigned int index);
		void setAnisotropicFilterEnabled(bool val);

		const float getMificationFilterType() const;
		const float getMagnificationFilterType() const;
		const float getTWrapType() const;
		const float getSWrapType() const;
		const unsigned int getTextureIndex() const;
		const bool isAnisotropicFilteringEnabled() const;

		void reseize(unsigned int w, unsigned int h);

		void configureTexture() const;
	};

	// =============================================================

	class TextureTable
	{
	private:
		static TextureTable * INSTANCE;

		std::map<std::string, Texture *> textureTable;

		bool anisotropicFilteringSupport;
		float maxAnisotropicFilterLevel;
		
	private:
		TextureTable();
	public:
		static TextureTable & getInstance();

		~TextureTable();

		void checkForAnisotropicFilterSupport();
		void cacheTexture(std::string fileName);
		TextureInstance * instantiateTexture(std::string fileName);
		const bool isAnisotropicFilteringSupported() const;
		const float getMaxAnisotropicFilterLevel() const;
		void destroy();
	};
}