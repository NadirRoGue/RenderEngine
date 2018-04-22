/*
* @author Nadir Román Guerrero
* @email nadir.ro.gue@gmail.com
*/

#pragma once

#include <glm/glm.hpp>
#include <map>
#include <gl\glew.h>

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

		float angleR;
		glm::vec3 rotation;
		glm::vec3 translation;
		glm::vec3 scaleVector;

		GLenum renderMode;

		TextureInstance * albedo;
		TextureInstance * emissive;
		TextureInstance * normal;
		TextureInstance * specular;
	public:
		Object(MeshInstance * mi);
		~Object();

		const glm::mat4 & getModelMatrix() const;
		MeshInstance * getMeshInstance();
		Mesh * getMesh() const;

		void rotate(float angle, glm::vec3 r);
		void translate(glm::vec3 t);
		void scale(glm::vec3 s);
		void setTranslation(glm::vec3 t);
		void setScale(glm::vec3 s);
		void setModelMatrix(const glm::mat4 & matrix);

		void setRenderMode(GLenum renderMode);
		GLenum getRenderMode();

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

	class PostProcessObject: public Object
	{
	private:
		std::map<std::string, TextureInstance *> inputBuffers;
	public:
		PostProcessObject(MeshInstance * mi);

		void addTexture(std::string name, TextureInstance * instance);
		TextureInstance * getTexture(std::string name);
		const std::map<std::string, TextureInstance *> & getAllCustomTextures() const;
	};
}