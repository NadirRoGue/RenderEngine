/*
* @author Nadir Román Guerrero
* @email nadir.ro.gue@gmail.com
*/

#pragma once

#include <list>
#include <glm/glm.hpp>
#include <GL/glew.h>
#include <memory>

#include "Defines.h"
#include "instances/MeshInstance.h"
#include "lights/PointLight.h"
#include "lights/SpotLight.h"
#include "lights/DirectionalLight.h"
#include "Object.h"

namespace Engine
{
	class Program
	{
	protected:
		unsigned int glProgram;
		unsigned int vShader;
		unsigned int fShader;

	private:
		std::string name;

	public:
		Program(std::string name);
		Program(const Program & other);
		~Program();

		std::string getName() const;
		unsigned int getProgramId() const;
		virtual void initialize(std::string vShaderFile, std::string fShaderFile);

		virtual void configureProgram() = 0;
		virtual void configureMeshBuffers(MeshInstance * mesh) = 0;

		virtual void configurePointLightBuffer(const PointLight *pl);
		virtual void configureSpotLightBuffer(const SpotLight *pl);
		virtual void configureDirectionalLightBuffer(const DirectionalLight *dl);
		virtual void configureClearColor(const glm::vec3 & cc) {};

		virtual void onRenderLight(const glm::mat4 & model, const glm::mat4 & view) {};
		virtual void onRenderSpotLight(const glm::mat4 & modelPos, const glm::mat4 & modelDir, const glm::mat4 & view) {};
		virtual void onRenderDirectionalLight(const glm::mat4 & model, const glm::mat4 & view) {};
		virtual void onRenderObject(const Object * obj, const glm::mat4 & view, const glm::mat4 &proj) = 0;

		virtual void destroy();
	protected:
		virtual unsigned int loadShader(std::string fileName, GLenum type);
	};

	/*
	template<class T, typename std::enable_if<std::is_base_of<Program, T>::value>::type* = nullptr>
	class ProgramFactory
	{
	private:
		std::map<ulong, std::unique_ptr<T>> cache;

	public:
		ProgramFactory()
		{
		}

		~ProgramFactory()
		{
		}

		T * create(ulong params)
		{
			std::map<ulong, T*>::iterator it = cache.find(params);
			if (it != cache.end())
			{
				return it->second.get();
			}
			else
			{
				return (cache[params] = make_unique<T>(params)).get();
			}
		}

		void destroy()
		{
			std::map<ulong, T *>::iterator it = cache.begin();
			while (it != cache.end())
			{
				it->second->destroy();
				it++;
			}

			cache.clear();
		}
	};
	*/
}