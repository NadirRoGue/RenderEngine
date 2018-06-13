#pragma once

namespace Engine
{
	class Time
	{
	public:
		static float deltaTime;
		static float timeSinceBegining;
		static unsigned long long frame;

		static void update(double time);
	};
}