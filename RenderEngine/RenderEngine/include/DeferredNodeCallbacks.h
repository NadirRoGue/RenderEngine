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

	class MotionBlurImpl : public DeferredCallback
	{
	private:
		bool enabled;
		float r, g, b, alpha;
	public:
		MotionBlurImpl();
		~MotionBlurImpl();

		glm::vec4 getMotionBlurStats();
		void switchEnabled();
		void setColorMask(float r, float g, float b);
		void setAlpha(float alpha);
		void execute(Object * obj, Program * prog, DeferredRenderObject * buffer, Camera * cam);
	};

	// =========================================================

	class DeferredShadingLightPopulation : public DeferredCallback
	{
	public:
		DeferredShadingLightPopulation();
		~DeferredShadingLightPopulation();
		void initialize(Object *obj, Program * prog, DeferredRenderObject * buffer);
		void execute(Object * obj, Program * prog, DeferredRenderObject * buffer, Camera * cam);
	};
}