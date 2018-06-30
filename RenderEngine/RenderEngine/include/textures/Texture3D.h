/**
* @author Nadir Román Guerrero
* @email nadir.ro.gue@gmail.com
*/
#pragma once

#include "Texture.h"

namespace Engine
{
	/**
	 * Class in charge to represent, give access, and manipulate a 3D texture
	 */
	class Texture3D : public AbstractTexture
	{
	private:
		// Voxel data
		unsigned char * data;
		// Width
		unsigned int width;
		// Height
		unsigned int height;
		// Depth
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