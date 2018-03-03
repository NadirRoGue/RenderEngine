/*
* @author Nadir Román Guerrero
* @email nadir.ro.gue@gmail.com
*/

#pragma once

#include <map>
#include <list>

#include "Camera.h"

namespace Engine
{
	// =========================================================================

	class MouseHandler
	{
	private:
		std::string name;
	protected:
		std::list<int> usedButtons;
	public:
		MouseHandler(std::string nam);
		~MouseHandler();

		const std::string & getName() const;
		const std::list<int> & getUsedButtons() const;

		virtual void handleMouseClick(int button, int state, int x, int y);
	};

	// =========================================================================

	class MouseMotionHandler
	{
	private:
		std::string name;
	protected:
		std::list<int> usedButtons;
	public:
		MouseMotionHandler(std::string nam);
		~MouseMotionHandler();

		const std::string & getName() const;
		const std::list<int> & getUsedButtons() const;

		virtual void handleMotion(int x, int y);
		virtual void finishMotion();
	};

	// =========================================================================

	class CameraMotion : public MouseMotionHandler
	{
	private:
		Camera * camera;

		int previousX, previousY;

		float horizontalSpeed;
		float verticalSpeed;
	public:
		CameraMotion(std::string nam, Camera * cam);
		
		void handleMotion(int x, int y);
		void finishMotion();
	};

	// =========================================================================
	// =========================================================================

	class MouseEventManager
	{
	private:
		static MouseEventManager * INSTANCE;

	private:
		MouseHandler * activeMouseHandler[3];
		MouseMotionHandler * activeMotionHandler[3];

		std::map<std::string, MouseHandler *> mouseHandlers;
		std::map<std::string, MouseMotionHandler *> motionHandlers;

		int lastButtonPressed;

	private:
		MouseEventManager();
	public:
		static MouseEventManager & getInstance();
		~MouseEventManager();

		void registerMouseHandler(MouseHandler * handler);
		void registerMouseMotionHandler(MouseMotionHandler * handler);

		void activateMouseHandler(const std::string & name);
		void activateMouseHandler(MouseHandler * handler);

		void activateMouseMotionHandler(const std::string & name);
		void activateMouseMotionHandler(MouseMotionHandler * handler);


		void handleMouseClick(int button, int state, int x, int y);
		void handleMouseMotion(int x, int y);
	};
}