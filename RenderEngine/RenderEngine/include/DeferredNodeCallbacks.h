/**
* @author Nadir Román Guerrero
* @email nadir.ro.gue@gmail.com
*/

#pragma once

#include "Object.h"
#include "Program.h"
#include "DeferredRenderObject.h"
#include "Camera.h"

namespace Engine
{
	class DeferredCallback
	{
	public:
		DeferredCallback();
		~DeferredCallback();
		virtual void initialize(Object *obj, Program * prog, DeferredRenderObject * buffer);
		virtual void execute(Object * obj, Program * prog, DeferredRenderObject * buffer, Camera * cam) = 0;
	};

	// ========================================================
}