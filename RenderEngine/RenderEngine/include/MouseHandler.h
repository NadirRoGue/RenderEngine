/*
* @author Nadir Román Guerrero
* @email nadir.ro.gue@gmail.com
*/

#pragma once

#include <map>
#include <list>
#include <iostream>

namespace Engine
{
	// =========================================================================
	// Parent class of the classes which implements the mouse button handling functionality
	class MouseHandler
	{
	private:
		std::string name;
	protected:
		// The list of mouse buttons to which this handler should be attached
		// must be stored on this list on handlers constructor
		std::list<int> usedButtons;
	public:
		MouseHandler(std::string nam);
		~MouseHandler();

		const std::string & getName() const;
		const std::list<int> & getUsedButtons() const;

		virtual void handleMouseClick(int button, int state, int x, int y);
	};

	// =========================================================================
	// Parent class of the classes which implements the mouse movement handling functionality
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
	// Stores, updates, and manages both mouse button and mouse movement handlers
	class MouseEventManager
	{
	private:
		MouseHandler * activeMouseHandler[3];
		MouseMotionHandler * activeMotionHandler[3];

		std::map<std::string, MouseHandler *> mouseHandlers;
		std::map<std::string, MouseMotionHandler *> motionHandlers;

		int lastButtonPressed;

	public:
		MouseEventManager();
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