/*
* @author Nadir Román Guerrero
* @email nadir.ro.gue@gmail.com
*/

#pragma once

#include <map>
#include "Object.h"

namespace Engine
{
	// ==================================================
	// Base class of all per-frame updateable animations (such as the automatic camera movement)
	class Animation
	{
	private:
		std::string name;
	protected:
		Object * target;
	public:
		Animation(const std::string & animName, Object * obj);

		const std::string & getName() const;

		virtual void update() = 0;
	};

	// ==================================================
	// Will hold all the animations and will handle updating them
	class AnimationTable
	{
	private:
		// Animation list
		std::map<std::string, Animation *> animationTable;
	public:
		AnimationTable();
		~AnimationTable();

		void registerAnimation(Animation * anim);
		Animation * getAnimation(std::string name);
		void tick();
		void destroy();
	};
}