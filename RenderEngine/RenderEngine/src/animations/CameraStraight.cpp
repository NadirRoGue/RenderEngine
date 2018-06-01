#include "animations/CameraStraight.h"

#include "WorldConfig.h"
#include "TimeAccesor.h"

Engine::CameraStraight::CameraStraight(Engine::Camera * cam, glm::vec3 & moveDir)
	:Engine::Animation("CameraStraight", NULL),cam(cam), moveDirection(moveDir)
{
	moveSpeed = 4.0f;
}

void Engine::CameraStraight::update()
{
	unsigned int tm = Engine::Settings::travelMethod;
	Engine::TravelMethod tmEnum = static_cast<Engine::TravelMethod>(tm);
	if (tmEnum == Engine::TravelMethod::TRAVEL_STRAIGHT)
	{
		glm::vec3 camPos = -cam->getPosition();
		glm::vec3 camFwd = -cam->getForwardVector();
		camFwd.y = 0;
		camFwd = glm::normalize(camFwd);
		glm::vec3 advanceMov = camPos + camFwd * Engine::Time::deltaTime * moveSpeed;
		cam->setLookAt(advanceMov, advanceMov + camFwd);
	}
}