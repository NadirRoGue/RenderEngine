/**
* @author Nadir Román Guerrero
* @email nadir.ro.gue@gmail.com
*/

#include "DeferredNodeCallbacks.h"

#include "Scene.h"
#include "PostProcessProgram.h"
#include "postprocessprograms/DeferredShadingProgram.h"
#include <iostream>

Engine::DeferredCallback::DeferredCallback()
{

}

Engine::DeferredCallback::~DeferredCallback()
{

}

void Engine::DeferredCallback::initialize(Engine::Object *obj, Program * prog, Engine::DeferredRenderObject * buffer)
{

}

// =========================================================