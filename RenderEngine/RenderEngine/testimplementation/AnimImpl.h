#include "Animation.h"

namespace Engine
{
	namespace TestImplementation
	{
		class DefaultCubeSpin : public Animation
		{
		private:
			float limit;
			float angle;
		public:
			DefaultCubeSpin(const std::string & animName, Object * obj);
			void update();
		};

		class OrbitingCube : public Animation
		{
		private:
			float angle;
			float limit;
		public:
			OrbitingCube(const std::string & animName, Object * obj);
			void update();
		};

		class BezierOrbitingCube : public Animation
		{
		private:
			float alphaStep;

			glm::vec3 points[13];
			unsigned int currentIndex;
			float alpha;
		private:
			glm::vec3 evaluateBezier();
		public:
			BezierOrbitingCube(const std::string & animName, Object *obj);
			void update();
		};
	}
}