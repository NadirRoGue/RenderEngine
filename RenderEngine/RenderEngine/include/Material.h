#pragma once

#include "Program.h"
#include "Texture.h"

#include <glm/glm.hpp>
#include <map>

namespace Engine
{
	namespace Material
	{
		class AbstractMaterial
		{
		friend class MaterialTable;
		protected:
			std::string name;
			unsigned long flags;

			virtual Program * compile() = 0;
		public:
			AbstractMaterial();
			~AbstractMaterial();

			const std::string & getMaterialName() const;
			unsigned long getFlags() const;

			virtual void populateProgram(Program * prog) = 0;
		};

		// =====================================================

		class StandardMaterial : public AbstractMaterial
		{
		private:
			static glm::vec3 DEFAULT_COLOR;

			TextureInstance * albedo;
			TextureInstance * normalMap;
			TextureInstance * specularMap;
			TextureInstance * occlusionMap;
			TextureInstance * displacementMap;

			float specularCoefficent;
			bool unlit;
			bool autoLOD;

		protected:
			Program * compile();
		public:
			StandardMaterial();
			~StandardMaterial();

			void setAlbedo(TextureInstance * albedo);
			void setNormalMap(TextureInstance * normalMap);
			void setSpecularMap(TextureInstance * specularMap);
			void setOcclusionMap(TextureInstance * occlusionMap);
			void setDisplacementMap(TextureInstance * displacementMap);
			void setSpecularCoefficent(float coefficent);
			void setUnlit(bool unlit);
			void setAutoLOD(bool autolod);

			void populateProgram(Program * prog);
		};

		// ==================================================================

		class MaterialTable
		{
		private:
			static MaterialTable * INSTANCE;

			std::map < std::string, std::map<unsigned long, Program *>> table;

		private:
			MaterialTable();

		public:
			static MaterialTable & getInstance();

			~MaterialTable();

			void addMaterial(AbstractMaterial * mat);
			Program * getProgramFromMaterial(AbstractMaterial * mat);
		};
	}
}