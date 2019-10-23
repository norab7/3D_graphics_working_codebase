#include "Window.h"

using namespace _Window;

Window::Window():Window(width, height) {

}
Window::Window(int width, int height) {
	// Sets up the window from given parameters
	setWindowSize(width, height);
	this->fullscreen = fullscreen;
	this->aspect = (float)this->width / (float)this->height;

	// Sets fullscreen on primary monitor, otherwise is a window
	window = glfwCreateWindow(width, height, "Main_Window", ((fullscreen) ? glfwGetPrimaryMonitor() : NULL), NULL);
	glfwMakeContextCurrent(window);

	if (!init()) {
		std::cout << "[Failure] Window Setup" << std::endl;
	} else {
		std::cout << "Window Setup Complete" << std::endl;
	}
}


int Window::init() {

	if (!glfwInit()) {
		std::cout << "[Failure] GLFW init" << std::endl;
		return 0;
	}

	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

	window = glfwCreateWindow(width, height, windowTitle, NULL, NULL);
	glfwMakeContextCurrent(window);

	glewExperimental = GL_TRUE;

	GLenum errGLEW = glewInit();

	if (GLEW_OK != errGLEW) {
		std::cout << "[Failure] GLEW init" << std::endl;
		return 0;
	}

	// setup some render variables.
	glfwSwapInterval(1);	// Ony render when synced (V SYNC)
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_SAMPLES, 2);
	glfwWindowHint(GLFW_STEREO, GL_FALSE);

	//if (!glfwInit()) {		
	//	std::cout << "GLFW failed to initialise";
	//}

	//glewExperimental = GL_TRUE;					// hack: catching them all - forcing newest debug callback (glDebugMessageCallback)
	//GLenum errGLEW = glewInit();
	//if (GLEW_OK != errGLEW) {					// Problems starting GLEW?
	//	std::cout << "GLEW failed to initialise";
	//}

	//if (fullscreen) {
	//	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);	// Set mouse cursor FPS fullscreen.
	//}

	//glfwSwapInterval(1);	// Ony render when synced (V SYNC)
	//glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	//glfwWindowHint(GLFW_SAMPLES, 2);
	//glfwWindowHint(GLFW_STEREO, GL_FALSE);

	return 1;
}

void Window::setWindowSize(int width, int height) {
	this->width = width;
	this->height = height;
	this->aspect = (float)width / (float)height;
}

int Window::getWidth() {
	return this->width;
}

int Window::getHeight() {
	return this->height;
}

float Window::getAspect() {
	return this->aspect;
}
