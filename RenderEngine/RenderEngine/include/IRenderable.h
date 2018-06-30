/**
* @author Nadir Román Guerrero
* @email nadir.ro.gue@gmail.com
*/
#pragma once

namespace Engine
{
	// Render mode
	enum RenderMode
	{
		RENDER_MODE_SHADED,
		RENDER_MODE_WIRE,
		RENDER_MODE_POINT
	};

	// Represents any object which might be subect to change the way its renderer
	class IRenderable
	{
	public:
		virtual void notifyRenderModeUpdate(RenderMode mode) = 0;
	};
}