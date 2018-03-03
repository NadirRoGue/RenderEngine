/*
* @author Nadir Román Guerrero
* @email nadir.ro.gue@gmail.com
*/

#define GLM_FORCE_RADIANS

#include "Animation.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>

Engine::AnimationTable::AnimationTable()
{
}

Engine::AnimationTable::~AnimationTable()
{
	destroy();
}

void Engine::AnimationTable::registerAnimation(Engine::Animation * anim)
{
	if (anim != 0)
	{
		animationTable[anim->getName()] = anim;
	}
}

Engine::Animation * Engine::AnimationTable::getAnimation(std::string name)
{
	std::map<std::string, Engine::Animation *>::iterator it = animationTable.find(name);

	if (it != animationTable.end())
	{
		return it->second;
	}

	return NULL;
}

void Engine::AnimationTable::tick()
{
	std::map<std::string, Engine::Animation *>::iterator it = animationTable.begin();
	while (it != animationTable.end())
	{
		it->second->update();
		it++;
	}
}

void Engine::AnimationTable::destroy()
{
	std::map<std::string, Engine::Animation *>::iterator it = animationTable.begin();
	while (it != animationTable.end())
	{
		delete it->second;
		it++;
	}
}

// ==================================================
// ==================================================

Engine::Animation::Animation(const std::string & animName, Engine::Object * obj)
{
	name = std::string(animName);
	target = obj;
}

const std::string & Engine::Animation::getName() const
{
	return name;
}