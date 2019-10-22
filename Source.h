#pragma once

// #pragma comment(linker, "/NODEFAULTLIB:MSVCRT")
#define GLM_ENABLE_EXPERIMENTAL
#define STB_IMAGE_IMPLEMENTATION

#include <iostream>
// #include <stdlib.h>
#include <fstream>
#include <string>
#include <sstream>		
#include <vector>

#include <GL/glew.h>			// Add library to extend OpenGL to newest version
#include <GLFW/glfw3.h>			// Add library to launch a window
#include <GLM/glm.hpp>			// Add helper maths library
#include <GLM/gtx/transform.hpp>

#include <stb_image.h>			// Add library to load images for textures

#include "Camera.h"
#include "Mesh.h"				// Simplest mesh holder and OBJ loader - can update more - from https://github.com/BennyQBD/ModernOpenGLTutorial


using namespace std;
using namespace glm;

// # # # Members # # #
// Window
GLFWwindow* window;											// Keep track of the window
int windowWidth = 1024;
int	windowHeight = 720;


// Time
GLfloat	deltaTime =	0.0f;
GLfloat lastFrame = 0.0f;

// Keyboard
bool keyStatus[1024];

// Vectors
vec3 worldUp = vec3(0.0f, 1.0f, 0.0f);

// Camera
_Camera::Camera camera; // = _Camera::Camera(vec3(0.0f, 0.0f, 10.0f), vec3(0.0f, 0.0f, 0.0f));

//GLfloat cameraSpeed = 0.1f * deltaTime;
//vec3 cameraPosition =		vec3(0.0f, 0.0f, 3.0f);
//vec3 cameraFront =			vec3(0.0f, 0.0f, 0.0f);
//
//vec3 cameraPositionCheck = vec3(0.0f, 0.0f, 0.0f);
//vec3 cameraFrontCheck = vec3(0.0f, 0.0f, 0.0f);
//
//vec3 cameraDirection;
//vec3 cameraRight;
//vec3 cameraUp;

//mat4 cameraView;

// # # # Methods # # # 
// Main
bool init();
bool windowSetup();
bool glewSetup();
void callbackSetup();