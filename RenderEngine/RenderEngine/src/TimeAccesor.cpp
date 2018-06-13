#include "TimeAccesor.h"

float Engine::Time::deltaTime = 1.0f/60.0f;
float Engine::Time::timeSinceBegining = 0.0f;
unsigned long long Engine::Time::frame = 0;

void Engine::Time::update(double time)
{
	float fltTime = float(time);
	deltaTime = fltTime - timeSinceBegining;
	timeSinceBegining = fltTime;
	frame++;
	frame = frame % 2048;
}