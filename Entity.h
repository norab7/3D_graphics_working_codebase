#pragma once
#include <iostream>
#include <string>

#include <GLM/glm.hpp>			// Add helper maths library
#include <GLM/gtx/transform.hpp>
#include <GLFW/glfw3.h>

using namespace glm;

namespace _Entity {
	class Entity {
	private:
		/* Members */
		/* Methods */
	protected:
		/* Members */

		// Positional
		vec3 worldUp = vec3(0.0f, 1.0f, 0.0f);
		vec3 position;
		vec3 target;
		vec3 direction;
		vec3 right;

		// Mouse
		bool firstMouse = true;
		float mouseSensitivity = 0.05f;
		float mouseX;
		float mouseY;
		float offsetX;
		float offsetY;

		// Time
		float deltaTime = 1.0f;

		bool isStatic = true;
		bool canRotate = true;

		/* Methods */
		void translate(vec3 translation);
		void rotate();

	public:
		/* Members */
		mat4 view;

		/* Methods */
		Entity(vec3 position = vec3(0.0f, 0.0f, 0.0f));
		Entity(vec3 position, vec3 target);
		Entity(vec3 position, vec3 target, bool isStatic);

		void setPosition(vec3 position);
		void setTarget(vec3 target);
		// void setDirection();
		void setDeltaTime(float deltaTime);

		vec3 getPosition();
		mat4 getView();

		virtual void update();
		virtual void processInput(std::string key);
		virtual void processMouse(float x, float y);

		void printVector(vec3 vector);
		void printMsg(std::string msg);
	};
}


//		Motion
// moveTo

//		Appearance ( object only )
// Mesh
// Texture


// scale (object only)