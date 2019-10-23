#pragma once
#include "Entity.h"
using namespace _Entity;

namespace _Camera {
	class Camera : public Entity {
	private:
		/* Members */
		bool magnify = false;

		/* Methods */
	protected:
		/* Members */
		float cameraSpeed = 2.5f;
		float speedModifier = 1.0f;

		vec3 cameraUp;
		vec3 front;

		float yaw = -90.0f;
		float pitch = 0.0f;

		/* Methods */

	public:
		/* Members */
		/* Methods */
		Camera();
		Camera(vec3 position, vec3 target);

		void update();
		void processInput(std::string key);
		void processMouse(float x, float y);
	};
}