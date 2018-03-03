/*
* @author Nadir Román Guerrero
* @email nadir.ro.gue@gmail.com
*/

#pragma once

#include <glm/glm.hpp>
#include <map>

#include "MeshInstance.h"
#include "Texture.h"

namespace Engine
{
	class Object
	{
	public:
		static std::string ALBEDO_TEX;
		static std::string NORMAL_TEX;
		static std::string SPECULAR_TEX;
		static std::string EMISSIVE_TEX;
	private:
		MeshInstance * meshInstance;

		glm::mat4 modelMatrix;

		glm::vec3 rotation;
		float angleR;
		glm::vec3 translation;

		glm::vec3 scaleVector;

		TextureInstance * albedo;
		TextureInstance * emissive;
		TextureInstance * normal;
		TextureInstance * specular;
		std::map<std::string, TextureInstance *> textures;
	public:
		Object(MeshInstance * mi);
		~Object();

		const glm::mat4 & getModelMatrix() const;
		MeshInstance * getMeshInstance();
		Mesh * getMesh();

		void rotate(float angle, glm::vec3 r);
		void translate(glm::vec3 t);
		void scale(glm::vec3 s);
		void setModelMatrix(const glm::mat4 & matrix);

		void addTexture(std::string name, TextureInstance * instance);
		TextureInstance * getTexture(std::string name);
		const std::map<std::string, TextureInstance *> & getAllCustomTextures() const;

		void setAlbedoTexture(TextureInstance * t);
		void setNormalMapTexture(TextureInstance * t);
		void setSpecularMapTexture(TextureInstance * t);
		void setEmissiveTexture(TextureInstance * t);

		const TextureInstance * getAlbedoTexture() const;
		const TextureInstance * getNormalMapTexture() const;
		const TextureInstance * getSpecularMapTexture() const;
		const TextureInstance * getEmissiveTexture() const;
		
	private:
		void updateModelMatrix();
	};
}