/*
* @author Nadir Román Guerrero
* @email nadir.ro.gue@gmail.com
*/

#include "Animation.h"
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>

using namespace Engine;

AnimationTable * AnimationTable::INSTANCE = new AnimationTable();

AnimationTable & AnimationTable::getInstance()
{
	return *INSTANCE;
}

AnimationTable::AnimationTable()
{
}

AnimationTable::~AnimationTable()
{
	destroy();
}

void AnimationTable::registerAnimation(Animation * anim)
{
	if (anim != 0)
	{
		animationTable[anim->getName()] = anim;
	}
}

Animation * AnimationTable::getAnimation(std::string name)
{
	std::map<std::string, Animation *>::iterator it = animationTable.find(name);

	if (it != animationTable.end())
	{
		return it->second;
	}

	return NULL;
}

void AnimationTable::tick()
{
	std::map<std::string, Animation *>::iterator it = animationTable.begin();
	while (it != animationTable.end())
	{
		it->second->update();
		it++;
	}
}

void AnimationTable::destroy()
{
	std::map<std::string, Animation *>::iterator it = animationTable.begin();
	while (it != animationTable.end())
	{
		delete it->second;
		it++;
	}
}

// ==================================================
// ==================================================

Animation::Animation(const std::string & animName, Object * obj)
{
	name = std::string(animName);
	target = obj;
}

const std::string & Animation::getName() const
{
	return name;
}

// ==================================================

DefaultCubeSpin::DefaultCubeSpin(const std::string & animName, Object * obj)
	:Animation(animName, obj)
{
	limit = 3.141592f * 2.0f;
	angle = 0;
}

void DefaultCubeSpin::update()
{
	angle = (angle > limit) ? 0 : angle + 0.01f;
	target->rotate(angle, glm::vec3(1.0f, 1.0f, 0.0f));
}

// ==================================================

OrbitingCube::OrbitingCube(const std::string & animName, Object * obj)
	:Animation(animName, obj)
{
	limit = 3.141592f * 2.0f;
	angle = 0;
}

void OrbitingCube::update()
{
	angle = (angle > limit) ? 0 : angle + 0.02f;
	
	target->setModelMatrix(glm::rotate(glm::mat4(1.0f), angle, glm::vec3(0.0f, 1.0f, 0.0f))
		* glm::translate(glm::mat4(1.0f), glm::vec3(-3.0f, 0.0f, 0.0f))
		* glm::rotate(glm::mat4(1.0f), angle, glm::vec3(0.0f, 1.0f, 0.0f)));
}

// ==================================================

BezierOrbitingCube::BezierOrbitingCube(const std::string & animName, Object *obj)
	:Animation(animName, obj)
{
	points[0] = glm::vec3(1, 2, -2);
	points[1] = glm::vec3(2.6, 2, -0.6);
	points[2] = glm::vec3(1, 2, 4);
	points[3] = glm::vec3(5, 2, 4);
	points[4] = glm::vec3(9, 2, 4);
	points[5] = glm::vec3(7, 2, -0.6);
	points[6] = glm::vec3(8.6, 2, -2);
	points[7] = glm::vec3(10.2, 2, -3.4);
	points[8] = glm::vec3(4, 2, -4);
	points[9] = glm::vec3(2.6, 2, -2);
	points[10] = glm::vec3(1.2, 2, 0);
	points[11] = glm::vec3(-0.6, 2, -3.4);
	points[12] = glm::vec3(1, 2, -2);

	currentIndex = 0;
	alpha = 0.0f;

	alphaStep = 0.02f;
}

glm::vec3 BezierOrbitingCube::evaluateBezier()
{
	glm::vec3 p1 = points[currentIndex];
	glm::vec3 p2 = points[currentIndex + 1];
	glm::vec3 p3 = points[currentIndex + 2];
	glm::vec3 p4 = points[currentIndex + 3];

	const float oneMinusAlpha = 1 - alpha;

	glm::vec3 result = 
		((oneMinusAlpha * oneMinusAlpha * oneMinusAlpha)*p1)
		+ (3*(oneMinusAlpha*oneMinusAlpha)*alpha*p2)
		+ (3*(oneMinusAlpha)*alpha*alpha*p3)
		+ (alpha*alpha*alpha*p4);

	return result;
}

void BezierOrbitingCube::update()
{
	glm::vec3 nextPosition = evaluateBezier();

	target->setModelMatrix(glm::translate(glm::mat4(1.0), nextPosition));

	alpha += alphaStep;
	if (alpha >= 1.0)
	{
		currentIndex += 3;
		currentIndex %= 12;
		alpha = 0.0f;
	}
}