#pragma once

namespace Engine
{
	class Time
	{
	public:
		static float deltaTime;
		static float timeSinceBegining;

		static void update(double time);
	};
}