#include "renderers/ForwardRenderer.h"

#include "Scene.h"

Engine::ForwardRenderer::ForwardRenderer()
	:Engine::Renderer()
{
}

Engine::ForwardRenderer::~ForwardRenderer()
{
}

void Engine::ForwardRenderer::doRender()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	Engine::Scene * scene = Engine::SceneManager::getInstance().getActiveScene();

	if (scene == 0)
		return;

	if (scene->getTerrain() != NULL)
	{
		scene->getTerrain()->render(activeCam);
	}

	const std::map<std::string, Engine::ProgramRenderables *> renders = scene->getObjects();

	const std::map<std::string, Engine::PointLight *> pointLights = scene->getPointLights();
	const std::map<std::string, Engine::SpotLight *> spotLights = scene->getSpotLights();
	Engine::DirectionalLight * dl = scene->getDirectionalLight();

	std::map<std::string, Engine::ProgramRenderables *>::const_iterator renderableIt;
	for (renderableIt = renders.cbegin(); renderableIt != renders.cend(); renderableIt++)
	{
		// Stablish program to use
		// changing program is expensive -> https://www.opengl.org/discussion_boards/showthread.php/185615-cheep-expensive-calls
		glUseProgram(renderableIt->second->program->getProgramId());

		/*
		if (Engine::RenderManager::getInstance().isForwardRendering())
		{
			std::map<std::string, Engine::PointLight *>::const_iterator it = pointLights.cbegin();
			while (it != pointLights.end())
			{
				renderableIt->second->program->onRenderLight(it->second->getModelMatrix(), activeCam->getViewMatrix());
				it++;
			}

			std::map<std::string, Engine::SpotLight *>::const_iterator slIt = spotLights.cbegin();
			while (slIt != spotLights.end())
			{
				renderableIt->second->program->onRenderSpotLight(slIt->second->getModelMatrix(), slIt->second->getDirModelMatrix(), activeCam->getViewMatrix());
				slIt++;
			}

			std::map<std::string, Engine::DirectionalLight *>::const_iterator dlIt = directionalLights.cbegin();
			while (dlIt != directionalLights.end())
			{
				renderableIt->second->program->onRenderDirectionalLight(dlIt->second->getModelMatrix(), activeCam->getViewMatrix());
				dlIt++;
			}
		}
		*/

		renderProgram(activeCam, renderableIt->second);
	}
}

void Engine::ForwardRenderer::renderProgram(Engine::Camera * camera, Engine::ProgramRenderables * renderables)
{
	glm::mat4 & viewMatrix = camera->getViewMatrix();
	glm::mat4 & projMatrix = camera->getProjectionMatrix();

	Engine::Program * program = renderables->program;

	std::map<unsigned int, std::list<Engine::Object *>>::const_iterator it;
	for (it = renderables->objects.cbegin(); it != renderables->objects.cend(); it++)
	{
		// Stablish vao to use
		glBindVertexArray(it->first);

		std::list<Engine::Object *> meshes = it->second;
		std::list<Engine::Object *>::iterator listIt;

		for (listIt = meshes.begin(); listIt != meshes.end(); listIt++)
		{
			Object * objToRender = *listIt;
			program->onRenderObject(objToRender, viewMatrix, projMatrix);

			unsigned int vertexPerFace = objToRender->getMesh()->getNumVerticesPerFace();
			glDrawElements(objToRender->getRenderMode(), objToRender->getMesh()->getNumFaces() * vertexPerFace, GL_UNSIGNED_INT, (void*)0);
		}
	}
}

void Engine::ForwardRenderer::onResize(unsigned int w, unsigned int h)
{

}