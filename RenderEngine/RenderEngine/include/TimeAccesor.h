/*
* @author Nadir Román Guerrero
* @email nadir.ro.gue@gmail.com
*/
#pragma once

namespace Engine
{
	// Give access anywhere in the engine to the delta and elapsed time
	// It also holds information of the current frame number
	class Time
	{
	public:
		static float deltaTime;
		static float timeSinceBegining;
		static unsigned long long frame;

		static void update(double time);
	};
}