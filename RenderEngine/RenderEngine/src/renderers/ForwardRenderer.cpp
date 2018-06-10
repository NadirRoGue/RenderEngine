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

	std::map<std::string, Engine::ProgramRenderables *>::const_iterator renderableIt;
	for (renderableIt = renders.cbegin(); renderableIt != renders.cend(); renderableIt++)
	{
		// Stablish program to use
		// changing program is expensive -> 
		// https://www.opengl.org/discussion_boards/showthread.php/185615-cheep-expensive-calls
		renderableIt->second->program->use();

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
	Engine::ScreenManager::REAL_SCREEN_HEIGHT = Engine::ScreenManager::SCREEN_WIDTH = h;
	Engine::ScreenManager::REAL_SCREEN_WIDTH = Engine::ScreenManager::SCREEN_WIDTH = w;
}