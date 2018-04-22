#pragma once
/*
#include "Material.h"
#include "programs\StandardProgram.h"
#include "Texture.h"

#include <glm/glm.hpp>

namespace Engine
{
	namespace Material
	{
		class StandardMaterial : public AbstractMaterial
		{
		private:
			ProgramFactory<StandarProgram> factory;
		private:
			glm::vec3 color;
			glm::vec3 specularColor;
			glm::vec3 emissiveColor;

			unsigned int albedoTextureID;
			unsigned int normalTextureID;
			unsigned int specularTextureID;
			unsigned int emissiveTextureID;

			ulong flags;
		public:
			StandardMaterial();
			~StandardMaterial();

			void setColor(glm::vec3 & color);
			void setSpecularColor(glm::vec3 & color);
			void setEmissiveColor(glm::vec3 & color);
			void setAlbedoTexture(TextureInstance * texture);
			void setSpecularTexture(TextureInstance * texture);
			void setEmissiveTexture(TextureInstance * texture);
			void setNormalTexture(TextureInstance * texture);

			void buildShader();
			void populateShader();
		};
	}
}
*/