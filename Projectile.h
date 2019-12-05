#pragma once
#include "Mesh.h"
#include "Object.h"
using namespace _Object;

namespace _Projectile {
	class Projectile : public Object {
	public:

		vec3 start;
		vec3 path;
		vec3 direction;
		float duration = 10;
		float initialFrame = 0;

		/* Members */
		bool active = true;
		bool isChild = false;
		/* Methods */
		Projectile(std::string file, std::string texture, vec3 start, vec3 direction, float initialFrame, float duration, bool child = false);
		
		void kill(Projectile * explode);

		virtual void update(float interval);

	};
}