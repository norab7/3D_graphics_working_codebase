#pragma once

#pragma comment(linker, "/NODEFAULTLIB:MSVCRT")
#define GLM_ENABLE_EXPERIMENTAL
#define STB_IMAGE_IMPLEMENTATION

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>		
#include <vector>

#include <GL/glew.h>			// Add library to extend OpenGL to newest version
#include <GLFW/glfw3.h>			// Add library to launch a window
#include <GLM/glm.hpp>			// Add helper maths library
#include <GLM/gtx/transform.hpp>

#include <stb_image.h>			// Add library to load images for textures

// #include "Window.h"
#include "Camera.h"
#include "Mesh.h"				// Simplest mesh holder and OBJ loader - can update more - from https://github.com/BennyQBD/ModernOpenGLTutorial

using namespace std;
using namespace glm;

/* Members */
GLFWwindow * window;
int windowWidth = 1280;
int windowHeight = 720;

GLuint program;

_Camera::Camera * camera;

Mesh * cube;
mat4 proj_matrix;// Projection Matrix
vec3 modelPosition;

GLfloat	deltaTime =	0.0f;
GLfloat lastFrame = 0.0f;

float fovy = 90.0f;

/* Methods */
int init();
void callbackSetup();

void onResizeCallback(GLFWwindow* window, int w, int h);
void onKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
void onMouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
void onMouseMoveCallback(GLFWwindow* window, double x, double y);
void onMouseWheelCallback(GLFWwindow* window, double xoffset, double yoffset);

bool _fullscreen = false;
bool running = true;
float aspect = 0.0f;