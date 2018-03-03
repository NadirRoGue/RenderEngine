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

	class AnimationTable
	{
	private:
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