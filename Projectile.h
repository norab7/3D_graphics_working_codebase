#pragma once
#include "Mesh.h"
#include "Object.h"
using namespace _Object;

namespace _Projectile {
	class Projectile : public Object {
	private:
		/* Members */
		vec3 start;
		vec3 direction;
		float duration = 10;
		float initialFrame = 0;
		/* Methods */
	protected:
		/* Members */
		/* Methods */

	public:
		/* Members */
		bool active = true;
		bool isChild = false;
		/* Methods */
		Projectile(std::string file, vec3 start, vec3 direction, float initialFrame, float duration, bool child = false);
		
		void kill(Projectile * explode);

		virtual void update(float interval);

	};
}