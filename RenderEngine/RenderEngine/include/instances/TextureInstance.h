#pragma once

#include "Texture.h"

namespace Engine
{
	class TextureInstance
	{
	private:
		AbstractTexture * texture;

		int minificationFilter;
		int magnificationFilter;
		int tComponentWrapType;
		int sComponentWrapType;
		int rComponentWrapType;

		bool applyAnisotropicFiltering;
	public:
		TextureInstance(AbstractTexture * texture);
		~TextureInstance();

		const AbstractTexture * getTexture() const;
		void generateTexture();
		void uploadTexture();

		void setMinificationFilterType(const int min);
		void setMagnificationFilterType(const int mag);
		void setTComponentWrapType(const int wrapT);
		void setSComponentWrapType(const int wrapS);
		void setRComponentWrapType(const int wrapR);
		void setAnisotropicFilterEnabled(bool val);

		const int getMificationFilterType() const;
		const int getMagnificationFilterType() const;
		const int getTWrapType() const;
		const int getSWrapType() const;
		const int getRWrapType() const;
		const bool isAnisotropicFilteringEnabled() const;

		void resize(unsigned int w, unsigned int h);

		void configureTexture() const;
	};
}