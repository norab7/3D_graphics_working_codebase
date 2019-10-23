#pragma once
#define GLM_ENABLE_EXPERIMENTAL
#define STB_IMAGE_IMPLEMENTATION

#include <iostream>
#include <string>

#include <GL/glew.h>
#include <GLFW\glfw3.h>
#include <glm/glm.hpp>
using namespace glm;

namespace _Window {
	class Window {
	private:
		// Members

		// Methods
		int init();

	protected:
		// Members
		const char* windowTitle = "New Window Class Window";

		bool fullscreen = false;
		int width = 1280;
		int height = 720;
		float aspect;

		// Methods

	public:
		// Members
		GLFWwindow * window;

		// Methods
		Window();
		Window(int width, int height);

		bool active() { return !glfwWindowShouldClose(window); }

		void setWindowSize(int width, int height);

		int getWidth();
		int getHeight();
		float getAspect();
	};
}