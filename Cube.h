#pragma once
#include "Object.h"
using namespace _Object;

namespace _Cube {
	class Cube : public Object {
	private:
		/* Members */
		/* Methods */
		Cube();
	protected:
		/* Members */
		/* Methods */
	public:
		/* Members */
		/* Methods */
		Cube(float x, float y, float z);
		Cube(vec3 position);
		Cube(vec3 position, vec3 rotation);
		Cube(mat4 matrix);
	};
}