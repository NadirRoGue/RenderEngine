/*
* @author Nadir Román Guerrero
* @email nadir.ro.gue@gmail.com
*/

#include <map>

#include "Object.h"

namespace Engine
{

	// ==================================================
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

	// ==================================================
	// ==================================================

	class AnimationTable
	{
	private:
		static AnimationTable * INSTANCE;

		std::map<std::string, Animation *> animationTable;
	private:
		AnimationTable();
	public:
		static AnimationTable & getInstance();

		~AnimationTable();

		void registerAnimation(Animation * anim);
		Animation * getAnimation(std::string name);
		void tick();
		void destroy();
	};
}