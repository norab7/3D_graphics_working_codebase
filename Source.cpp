// 3D Graphics and Animation - Main Template
// Visual Studio 2019
// Last Changed 01/10/2019

#include "Source.h"

// Prebuild Main Functions
void setupRender();
void startup();
void update(GLfloat currentTime);
void render(GLfloat currentTime);
void endProgram();

// HELPER FUNCTIONS OPENGL
void hintsGLFW();
string readShader(string name);
void checkErrorShader(GLuint shader);
void errorCallbackGLFW(int error, const char* description);
void debugGL();
static void APIENTRY openGLDebugCallback(GLenum source,
	GLenum type,
	GLuint id,
	GLenum severity,
	GLsizei length,
	const GLchar* message,
	const GLvoid* userParam);

// CALLBACK FUNCTIONS FOR WINDOW
void onResizeCallback(GLFWwindow* window, int w, int h);
void onKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
void onMouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
void onMouseMoveCallback(GLFWwindow* window, double x, double y);
void onMouseWheelCallback(GLFWwindow* window, double xoffset, double yoffset);


// VARIABLES

bool			running = true;									// Are we still running?
mat4			proj_matrix;									// Projection Matrix

float           aspect = (float)windowWidth / (float)windowHeight;
float			fovy = 45.0f;

GLuint			program;
GLint			proj_location;
vec3			modelPosition;
vec3			modelRotation;

Mesh cube;		// Add a cube object
Mesh cube2;		// Second cube object

// My Variables
string title = "My OpenGL";
bool _fullscreen = true;
bool _camera = true;
vec3 viewRotation;




int main() {

	// Order of flow for setting up a window in OpenGL
	if (!init()) return 0;
	if (!windowSetup()) return 0;
	hintsGLFW();									
	if (!glewSetup()) return 0;
	callbackSetup();
	debugGL();	
	setupRender();								

	// Setup all necessary information for startup (aka. load texture, shaders, models, etc).
	startup();		

	// Main loop of program
	do {
		GLfloat currentFrame = (GLfloat)glfwGetTime();		// retrieve timelapse
		deltaTime = currentFrame - lastFrame;					// Calculate delta time
		lastFrame = currentFrame;								// Save for next frame calculations.
		glfwPollEvents();									// poll callbacks
		update(currentFrame);								// update (physics, animation, structures, etc)
		render(currentFrame);								// call render function.

		glfwSwapBuffers(window);							// swap buffers (avoid flickering and tearing)

		running &= (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_RELEASE);	// exit if escape key pressed
		running &= (glfwWindowShouldClose(window) != GL_TRUE);
	} while (running);

	endProgram();			// Close and clean everything up...

	cout << "\nPress return to close...";
	cin.get(); // delay closing console to read debugging errors.

	return 0;
}

bool init() {
	if (!glfwInit()) {							// Checking for GLFW
		cout << "Could not initialise GLFW...";
		return false;
	}
	glfwSetErrorCallback(errorCallbackGLFW);	// Setup a function to catch and display all GLFW errors.
	return true;
}

bool windowSetup() {
	if (_fullscreen) {
		// Fullscreen
		const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
		windowWidth = mode->width; windowHeight = mode->height; //fullscreen
		window = glfwCreateWindow(windowWidth, windowHeight, title.c_str(), glfwGetPrimaryMonitor(), NULL); // fullscreen
	}
	else {
		// Window
		window = glfwCreateWindow(windowWidth, windowHeight, title.c_str(), NULL, NULL);
	}
	if (!window) {								// Window or OpenGL context creation failed
		cout << "Could not initialise GLFW...";
		endProgram();
		return false;
	}
	glfwMakeContextCurrent(window);				// making the OpenGL context current
	return true;
}

bool glewSetup() {
	// Start GLEW (note: always initialise GLEW after creating your window context.)
	glewExperimental = GL_TRUE;					// hack: catching them all - forcing newest debug callback (glDebugMessageCallback)
	GLenum errGLEW = glewInit();
	if (GLEW_OK != errGLEW) {					// Problems starting GLEW?
		cout << "Could not initialise GLEW...";
		endProgram();
		return false;
	}
	return true;
}

void callbackSetup() {
	// Setup callback to catch openGL errors.
	// Setup all the message loop callbacks.
	glfwSetWindowSizeCallback(window, onResizeCallback);		// Set callback for resize
	glfwSetKeyCallback(window, onKeyCallback);					// Set Callback for keys
	glfwSetMouseButtonCallback(window, onMouseButtonCallback);	// Set callback for mouse click
	glfwSetCursorPosCallback(window, onMouseMoveCallback);		// Set callback for mouse move
	glfwSetScrollCallback(window, onMouseWheelCallback);		// Set callback for mouse wheel.
	//glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);	// Set mouse cursor. Fullscreen
	
	// MOUSE DISABLE
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);	// Set mouse cursor FPS fullscreen.

}

void errorCallbackGLFW(int error, const char* description) {
	cout << "Error GLFW: " << description << "\n";
}

void hintsGLFW() {
	// Setup glfw with various hints.
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);			// Create context in debug mode - for debug message callback
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
}

void endProgram() {
	glfwMakeContextCurrent(window);		// destroys window handler
	glfwTerminate();	// destroys all windows and releases resources.
}

void setupRender() {
	// setup some render variables.
	glfwSwapInterval(1);	// Ony render when synced (V SYNC)
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_SAMPLES, 2);
	glfwWindowHint(GLFW_STEREO, GL_FALSE);
}

void startup() {

	// Load main object model and shaders
	cube.LoadModel("cube_uv.obj");

	program = glCreateProgram();

	// read shaders from files
	string vs_text = readShader("vs_model.glsl"); 
	string fs_text = readShader("fs_model.glsl");
	const char* vs_source = vs_text.c_str();
	const char* fs_source = fs_text.c_str();

	// vertex shader
	GLuint vs = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vs, 1, &vs_source, NULL);
	glCompileShader(vs);
	checkErrorShader(vs);
	glAttachShader(program, vs);

	// fragment shader
	GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fs, 1, &fs_source, NULL);
	glCompileShader(fs);
	checkErrorShader(fs);
	glAttachShader(program, fs);

	glLinkProgram(program);
	glUseProgram(program);

	// Start from the centre  
	modelPosition = glm::vec3(0.0f, 0.0f, 0.0f); // ( R, U, D 

	// A few optimizations.
	glFrontFace(GL_CCW);
	glCullFace(GL_BACK);
	glEnable(GL_CULL_FACE);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	// Calculate proj_matrix for the first time.
	aspect = (float)windowWidth / (float)windowHeight;
	proj_matrix = glm::perspective(glm::radians(fovy), aspect, 0.1f, 1000.0f);
}

void update(GLfloat currentTime) {
	
	// TODO: Change to use Array of Entities from Entity class
	// TODO: Go through only entites in array that in view

	camera.setDeltaTime(deltaTime);
	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) { camera.processInput("shift"); }
	
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) { camera.processInput("space"); }
	if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS) { camera.processInput("ctrl"); }

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) { camera.processInput("w"); }
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) { camera.processInput("s"); }
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) { camera.processInput("a"); }
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) { camera.processInput("d"); }


		//if (keyStatus[GLFW_KEY_W]) cameraPosition += cameraSpeed * cameraFront;
		//if (keyStatus[GLFW_KEY_S]) cameraPosition -= cameraSpeed * cameraFront;
		//if (keyStatus[GLFW_KEY_A]) cameraPosition -= normalize(cross(cameraFront, cameraUp)) * cameraSpeed;
		//if (keyStatus[GLFW_KEY_D]) cameraPosition += normalize(cross(cameraFront, cameraUp)) * cameraSpeed;

		//// OLD CODE
		//// Camera Rotation
		//if (keyStatus[GLFW_KEY_LEFT])			cameraFront.y += 0.05f;
		//if (keyStatus[GLFW_KEY_RIGHT])			cameraFront.y -= 0.05f;
		//if (keyStatus[GLFW_KEY_UP])				cameraFront.x += 0.05f;
		//if (keyStatus[GLFW_KEY_DOWN])			cameraFront.x -= 0.05f;

		//// Camera Movement
		//if (keyStatus[GLFW_KEY_SPACE])			cameraPosition.y += 0.10f;	// TODO: Create Jump
		//if (keyStatus[GLFW_KEY_D])				cameraPosition.x += 0.10f;
		//if (keyStatus[GLFW_KEY_A])				cameraPosition.x -= 0.10f;
		//if (keyStatus[GLFW_KEY_W])				cameraPosition.z -= 0.10f;
		//if (keyStatus[GLFW_KEY_S])				cameraPosition.z += 0.10f;
		//// OLD CODE


		//// Model Rotation
		//if (keyStatus[GLFW_KEY_LEFT])			modelRotation.y += 0.05f;
		//if (keyStatus[GLFW_KEY_RIGHT])			modelRotation.y -= 0.05f;
		//if (keyStatus[GLFW_KEY_UP])				modelRotation.x += 0.05f;
		//if (keyStatus[GLFW_KEY_DOWN])			modelRotation.x -= 0.05f;
		//if (keyStatus[GLFW_KEY_LEFT_BRACKET])	modelRotation.z += 0.05f; // ROLL IS NOT Z
		//if (keyStatus[GLFW_KEY_RIGHT_BRACKET])	modelRotation.z -= 0.05f; // ROLL IS NOT Z

		//// Model Position and Zoom
		//// Zoom
		//if (keyStatus[GLFW_KEY_EQUAL])			modelPosition.z += 0.10f;
		//if (keyStatus[GLFW_KEY_MINUS])			modelPosition.z -= 0.10f;
		//// Position
		//if (keyStatus[GLFW_KEY_W])				modelPosition.y += 0.10f;
		//if (keyStatus[GLFW_KEY_S])				modelPosition.y -= 0.10f;
		//if (keyStatus[GLFW_KEY_D])				modelPosition.x += 0.10f;
		//if (keyStatus[GLFW_KEY_A])				modelPosition.x -= 0.10f;

}

void render(GLfloat currentTime) {
	glViewport(0, 0, windowWidth, windowHeight);

	// Clear colour buffer
	glm::vec4 backgroundColor = glm::vec4(0.2f, 0.2f, 0.2f, 1.0f); glClearBufferfv(GL_COLOR, 0, &backgroundColor[0]);

	// Clear deep buffer
	static const GLfloat one = 1.0f; glClearBufferfv(GL_DEPTH, 0, &one);

	// Enable blend
	glEnable(GL_BLEND); glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Use our shader programs
	glUseProgram(program);

	// Setup camera (eye, centre, up)
	// glm::mat4 viewMatrix = lookAt(cameraPosition, cameraPosition + cameraFront, cameraUp);
/*
	cameraDirection = normalize(cameraPosition - cameraFront);
	cameraRight = normalize(cross(cameraUp, cameraFront));
	cameraUp = normalize(cross(cameraFront, cameraRight));*/

	// matrix = lookat( position, target, up )
	//cameraView = lookAt(cameraPosition, glm::vec3(0.0f, 0.0f, 0.0f), worldUp);

	//if (cameraPositionCheck != cameraPosition) {
	//	cout << "CameraPosition: " << cameraPosition.x << ", " << cameraPosition.y << ", " << cameraPosition.z << endl;
	//	cameraPositionCheck = cameraPosition;
	//}
	//if (cameraFrontCheck != cameraFront) {
	//	cout << "CameraFront: " << cameraFront.x << ", " << cameraFront.y << ", " << cameraFront.z << endl;
	//	cameraFrontCheck = cameraFront;
	//}
	
	// Do some translations, rotations and scaling
	glm::mat4 modelMatrix = glm::translate(glm::mat4(1.0f), modelPosition);
	//modelMatrix = glm::rotate(modelMatrix, modelRotation.x, glm::vec3(1.0f, 0.0f, 0.0f));
	//modelMatrix = glm::rotate(modelMatrix, modelRotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
	//modelMatrix = glm::rotate(modelMatrix, modelRotation.z, glm::vec3(0.0f, 0.0f, 1.0f));
	//modelMatrix = glm::scale(modelMatrix, glm::vec3(0.2f, 0.2f, 0.2f));

	// glm::mat4 mv_matrix = viewMatrix * modelMatrix;

	float radius = 10.0f;
	float camX = sin(lastFrame) * radius;
	float camZ = cos(lastFrame) * radius;

	//camera.setPosition(vec3(camX, 0.0f, camZ)); camera.update();

	// camera.view = lookAt(vec3(camX, 0.0f, camZ), vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f));
	// camera.view = lookAt(vec3(0.0f, 0.0f, 10.0f), vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f));

	camera.update();

	glUniformMatrix4fv(glGetUniformLocation(program, "model_matrix"), 1, GL_FALSE, &modelMatrix[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(program, "view_matrix"), 1, GL_FALSE, &camera.getView()[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(program, "proj_matrix"), 1, GL_FALSE, &proj_matrix[0][0]);

	cube.Draw();

}

void onResizeCallback(GLFWwindow* window, int w, int h) {
	windowWidth = w;
	windowHeight = h;

	aspect = (float)w / (float)h;
	proj_matrix = glm::perspective(glm::radians(fovy), aspect, 0.1f, 1000.0f);
}

void onKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	if (action == GLFW_PRESS) keyStatus[key] = true;
	else if (action == GLFW_RELEASE) keyStatus[key] = false;

	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GLFW_TRUE);
}

void onMouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {

}

void onMouseMoveCallback(GLFWwindow* window, double x, double y) {
	// TODO: Change to Entity to affect all objects
	camera.processMouse(x, y);
}

static void onMouseWheelCallback(GLFWwindow* window, double xoffset, double yoffset) {
	int yoffsetInt = static_cast<int>(yoffset);
}

void debugGL() {
	//Output some debugging information
	cout << "VENDOR: " << (char*)glGetString(GL_VENDOR) << endl;
	cout << "VERSION: " << (char*)glGetString(GL_VERSION) << endl;
	cout << "RENDERER: " << (char*)glGetString(GL_RENDERER) << endl;

	// Enable Opengl Debug
	//glEnable(GL_DEBUG_OUTPUT);
	glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
	glDebugMessageCallback((GLDEBUGPROC)openGLDebugCallback, nullptr);
	glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, NULL, true);
}

static void APIENTRY openGLDebugCallback(GLenum source,
	GLenum type,
	GLuint id,
	GLenum severity,
	GLsizei length,
	const GLchar* message,
	const GLvoid* userParam) {

	cout << "---------------------opengl-callback------------" << endl;
	cout << "Message: " << message << endl;
	cout << "type: ";
	switch (type) {
	case GL_DEBUG_TYPE_ERROR:
		cout << "ERROR";
		break;
	case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
		cout << "DEPRECATED_BEHAVIOR";
		break;
	case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
		cout << "UNDEFINED_BEHAVIOR";
		break;
	case GL_DEBUG_TYPE_PORTABILITY:
		cout << "PORTABILITY";
		break;
	case GL_DEBUG_TYPE_PERFORMANCE:
		cout << "PERFORMANCE";
		break;
	case GL_DEBUG_TYPE_OTHER:
		cout << "OTHER";
		break;
	}
	cout << " --- ";

	cout << "id: " << id << " --- ";
	cout << "severity: ";
	switch (severity) {
	case GL_DEBUG_SEVERITY_LOW:
		cout << "LOW";
		break;
	case GL_DEBUG_SEVERITY_MEDIUM:
		cout << "MEDIUM";
		break;
	case GL_DEBUG_SEVERITY_HIGH:
		cout << "HIGH";
		break;
	case GL_DEBUG_SEVERITY_NOTIFICATION:
		cout << "NOTIFICATION";
	}
	cout << endl;
	cout << "-----------------------------------------" << endl;
}

string readShader(string name) {
	string vs_text;
	std::ifstream vs_file(name);

	string vs_line;
	if (vs_file.is_open()) {

		while (getline(vs_file, vs_line)) {
			vs_text += vs_line;
			vs_text += '\n';
		}
		vs_file.close();
	}
	return vs_text;
}

void  checkErrorShader(GLuint shader) {
	// Get log lenght
	GLint maxLength;
	glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

	// Init a string for it
	std::vector<GLchar> errorLog(maxLength);

	if (maxLength > 1) {
		// Get the log file
		glGetShaderInfoLog(shader, maxLength, &maxLength, &errorLog[0]);

		cout << "--------------Shader compilation error-------------\n";
		cout << errorLog.data();
	}
}