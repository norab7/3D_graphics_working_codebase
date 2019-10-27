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
		/* Methods */
	protected:
		/* Members */
		/* Methods */
	public:
		/* Members */
		/* Methods */
		Projectile(vec3 start, vec3 direction);
		Projectile(vec3 start, vec3 direction, float duration);
	};
}