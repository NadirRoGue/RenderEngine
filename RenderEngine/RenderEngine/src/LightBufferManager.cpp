#include "LightBufferManager.h"

#include <GL/glew.h>
#include <map>
#include <vector>

#include "lights/DirectionalLight.h"
#include "lights/PointLight.h"
#include "lights/SpotLight.h"

Engine::GPU::LightBufferManager * Engine::GPU::LightBufferManager::INSTANCE = new Engine::GPU::LightBufferManager();

Engine::GPU::LightBufferManager & Engine::GPU::LightBufferManager::getInstance()
{
	return *Engine::GPU::LightBufferManager::INSTANCE;
}

// =====================================================================================

Engine::GPU::LightBufferManager::LightBufferManager()
{
}

Engine::GPU::LightBufferManager::~LightBufferManager()
{
	clean();
}

void Engine::GPU::LightBufferManager::onSceneStart()
{
	clean();

	Engine::Scene * scene = SceneManager::getInstance().getActiveScene();
	if (scene != NULL)
	{
		initializeBuffers(scene);
	}
}

void Engine::GPU::LightBufferManager::initializeBuffers(Engine::Scene * scene)
{
	// FIXME
	// ADD SUPPORT FOR SCENE DYNAMIC LIGHT ADD / REMOVAL

	Engine::DirectionalLight * dl = scene->getDirectionalLight();
	const std::map<std::string, Engine::PointLight *> & pl = scene->getPointLights();
	const std::map<std::string, Engine::SpotLight *> & sl = scene->getSpotLights();

	if (dl != NULL)
	{
		// Directional lights
		glGenBuffers(1, &gboDL);
		glBindBuffer(GL_UNIFORM_BUFFER, gboDL);
		glBufferData(GL_UNIFORM_BUFFER, sizeof(Engine::DirectionalLightData), &dl->getData(), GL_DYNAMIC_DRAW);
	}

	if (pl.size() > 0)
	{
		// Point lights
		glGenBuffers(1, &gboPL);
		glBindBuffer(GL_UNIFORM_BUFFER, gboPL);
		
		std::vector<Engine::PointLightData> plData(pl.size());
		std::map<std::string, Engine::PointLight *>::const_iterator cit = pl.cbegin();
		while (cit != pl.cend())
		{
			plData.push_back(cit->second->getData());
			cit++;
		}

		glBufferData(GL_UNIFORM_BUFFER, sizeof(Engine::PointLightData) * plData.size(), &plData[0], GL_DYNAMIC_DRAW);
	}

	if (sl.size() > 0)
	{
		// Spot lights
		glGenBuffers(1, &gboSL);
		glBindBuffer(GL_UNIFORM_BUFFER, gboSL);

		std::vector<Engine::SpotLightData> slData(sl.size());
		std::map<std::string, Engine::SpotLight *>::const_iterator cit = sl.cbegin();
		while (cit != sl.end())
		{
			slData.push_back(cit->second->getData());
			cit++;
		}

		glBufferData(GL_UNIFORM_BUFFER, sizeof(Engine::SpotLightData) * slData.size(), &slData[0], GL_DYNAMIC_DRAW);
	}
}

void Engine::GPU::LightBufferManager::clean()
{
	if (gboDL != -1)
		glDeleteBuffers(1, &gboDL);

	if (gboPL != -1)
		glDeleteBuffers(1, &gboPL);

	if (gboSL != -1)
		glDeleteBuffers(1, &gboSL);
}

void Engine::GPU::LightBufferManager::enableDirectionalLightBuffer()
{
	glBindBuffer(GL_UNIFORM_BUFFER, gboDL);
}

void Engine::GPU::LightBufferManager::enablePointLightBuffer()
{
	glBindBuffer(GL_UNIFORM_BUFFER, gboPL);
}

void Engine::GPU::LightBufferManager::enableSpotLightBuffer()
{
	glBindBuffer(GL_UNIFORM_BUFFER, gboSL);
}

void Engine::GPU::LightBufferManager::enableDirectionalLightBufferAtIndex(unsigned int index)
{
	glBindBufferBase(GL_UNIFORM_BUFFER, index, gboDL);
}

void Engine::GPU::LightBufferManager::enablePointLightBufferAtIndex(unsigned int index)
{
	glBindBufferBase(GL_UNIFORM_BUFFER, index, gboPL);
}

void Engine::GPU::LightBufferManager::enableSpotLightBufferAtIndex(unsigned int index)
{
	glBindBufferBase(GL_UNIFORM_BUFFER, index, gboSL);
}

void Engine::GPU::LightBufferManager::updateDirectionalLight(DirectionalLight * dl, bool onlyViewDependent)
{
	size_t structSize = sizeof(Engine::DirectionalLightData);
	if (onlyViewDependent)
	{
		glBufferSubData(GL_UNIFORM_BUFFER, structSize * dl->getBufferIndex(), sizeof(float) * 4, dl->getData().direction);
	}
	else
	{
		glBufferSubData(GL_UNIFORM_BUFFER, structSize * dl->getBufferIndex(), structSize, &dl->getData());
	}
}

void Engine::GPU::LightBufferManager::updatePointLight(PointLight * pl, bool onlyViewDependent)
{
	size_t structSize = sizeof(Engine::PointLight);
	if (onlyViewDependent)
	{
		glBufferSubData(GL_UNIFORM_BUFFER, structSize * pl->getBufferIndex(), sizeof(float) * 4, pl->getData().position);
	}
	else
	{
		glBufferSubData(GL_UNIFORM_BUFFER, structSize * pl->getBufferIndex(), structSize, &pl->getData());
	}
}

void Engine::GPU::LightBufferManager::updateSpotLight(SpotLight * sl, bool onlyViewDependent)
{
	size_t structSize = sizeof(Engine::SpotLightData);
	if (onlyViewDependent)
	{
		// 8: 4 pos + 4 dir
		glBufferSubData(GL_UNIFORM_BUFFER, structSize * sl->getBufferIndex(), sizeof(float) * 8, &sl->getData().direction);
	}
	else
	{
		glBufferSubData(GL_UNIFORM_BUFFER, structSize * sl->getBufferIndex(), structSize, &sl->getData());
	}
}