/**
* @author Nadir Román Guerrero
* @email nadir.ro.gue@gmail.com
*/

#pragma once

#include "Texture.h"

namespace Engine
{
	/**
	 * Class which acts as proxy between the different type of textures
	 * and the rest of the engine, allowing to use an unique data type
	 * to manage and access all of them
	 */
	class TextureInstance
	{
	private:
		// Abstract texture type we are instancing
		AbstractTexture * texture;

		// Texture configuration
		// minification texture filtering
		int minificationFilter;
		// magnification texture filtering
		int magnificationFilter;
		// 3D wrap type configuration
		int tComponentWrapType;
		int sComponentWrapType;
		int rComponentWrapType;

		// Flags used to enable anisotropic filter for this instance or not
		bool applyAnisotropicFiltering;
	public:
		TextureInstance(AbstractTexture * texture);
		~TextureInstance();

		// Returns a pointer to the texture we are instancing
		const AbstractTexture * getTexture() const;
		
		// Generates a texture ID
		void generateTexture();

		// Resize and uploas (if any data) the texture to the GPU
		void uploadTexture();

		// Apply the configuration to the texture
		void configureTexture() const;

		// Generates mip levels for this texture (must be configure prior to configureTexture())
		void generateMipMaps();

		// Configures the minification filter (must be configure prior to configureTexture())
		void setMinificationFilterType(const int min);
		// Configures the magnification filter (must be configure prior to configureTexture())
		void setMagnificationFilterType(const int mag);
		// Configures the texture wrap method for the t component (must be configure prior to configureTexture())
		void setTComponentWrapType(const int wrapT);
		// Configures the texture wrap method for the s component (must be configure prior to configureTexture())
		void setSComponentWrapType(const int wrapS);
		// Configures the texture wrap method for the r (3d textures) component (must be configure prior to configureTexture())
		void setRComponentWrapType(const int wrapR);
		// Sets wether this texture should use anisotropic filter (must be configure prior to configureTexture())
		void setAnisotropicFilterEnabled(bool val);

		const int getMificationFilterType() const;
		const int getMagnificationFilterType() const;
		const int getTWrapType() const;
		const int getSWrapType() const;
		const int getRWrapType() const;
		const bool isAnisotropicFilteringEnabled() const;

		// Resizes the texture we are instancing (will affect all other instances pointing to the same texture)
		void resize(unsigned int w, unsigned int h);
	};
}