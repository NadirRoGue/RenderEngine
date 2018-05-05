/*
* @author Nadir Román Guerrero
* @email nadir.ro.gue@gmail.com
*/

#pragma once
#define GLM_FORCE_RADIANS

#include <glm/glm.hpp>
#include <map>
#include <gl\glew.h>

#include "IRenderable.h"
#include "Mesh.h"
#include "instances/TextureInstance.h"
#include "Texture.h"

namespace Engine
{
	class Object : public IRenderable
	{
	public:
		static std::string ALBEDO_TEX;
		static std::string NORMAL_TEX;
		static std::string SPECULAR_TEX;
		static std::string EMISSIVE_TEX;
	private:
		Mesh * mesh;

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

		std::string shaderName;
	public:
		Object(Mesh * mi);
		~Object();

		const glm::mat4 & getModelMatrix() const;
		const Mesh * getMesh() const;
		Mesh * getManipMesh();

		void rotate(float angle, glm::vec3 r);
		void translate(glm::vec3 t);
		void scale(glm::vec3 s);
		void setTranslation(glm::vec3 t);
		void setScale(glm::vec3 s);
		void setModelMatrix(const glm::mat4 & matrix);

		void setShader(std::string shaderName);
		std::string getShaderName();

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

		void notifyRenderModeUpdate(RenderMode mode);
		
	private:
		void updateModelMatrix();
	};

	class PostProcessObject: public Object
	{
	private:
		std::map<std::string, TextureInstance *> inputBuffers;
	public:
		PostProcessObject(Mesh * mi);

		void addTexture(std::string name, TextureInstance * instance);
		TextureInstance * getTexture(std::string name);
		const std::map<std::string, TextureInstance *> & getAllCustomTextures() const;
	};
}