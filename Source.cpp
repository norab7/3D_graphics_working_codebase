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


int main() {
	std::cout << "Starting Up!" << std::endl << std::endl;

	// Order of flow for setting up a window in OpenGL
	init();
	callbackSetup(); 
	debugGL();
	setupRender();								

	camera = new _Camera::Camera();
	cube = new Mesh();
	// Setup all necessary information for startup (aka. load texture, shaders, models, etc).
	startup();		

	// Main loop of program
	std::cout << std::endl << " --- Starting Game Loop --- " << std::endl;
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
	std::cout << std::endl << " --- Finished Game Loop --- " << std::endl;

	endProgram();			
	cout << "\nPress return to close...";
	cin.get(); // delay closing console to read debugging errors.

	return 0;
}

int init() {

	if (!glfwInit()) {
		std::cout << "[Failure] GLFW init" << std::endl;
		return 0;
	}
	glfwSetErrorCallback(errorCallbackGLFW);

	// Sets fullscreen on primary monitor, otherwise is a window
	window = glfwCreateWindow(windowWidth, windowHeight, "Main_Window", ((_fullscreen) ? glfwGetPrimaryMonitor() : NULL), NULL);
	glfwMakeContextCurrent(window);

	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);	
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	
	// Force all debugging
	glewExperimental = GL_TRUE;

	GLenum errGLEW = glewInit();
	if (GLEW_OK != errGLEW) {
		cout << "GLEW failed to initialise";
		endProgram();
	}
}

void callbackSetup() {
	glfwSetWindowSizeCallback(window, onResizeCallback);	
	glfwSetKeyCallback(window, onKeyCallback);				
	glfwSetMouseButtonCallback(window, onMouseButtonCallback);
	glfwSetCursorPosCallback(window, onMouseMoveCallback);	
	glfwSetScrollCallback(window, onMouseWheelCallback);
	//glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	
	//MOUSE DISABLE
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

void errorCallbackGLFW(int error, const char* description) {
	//cout << "Error GLFW: " << description << "\n";
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

	//// Load main object model and shaders

	// TODO: ALL OF THIS CAN GO INTO A OBJECT CLASS

	// TODO: Scale Objects
	cube->LoadModel("chair.obj");

	program = glCreateProgram();

	//// read shaders from files
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
	modelPosition = glm::vec3(10.0f, 0.0f, 0.0f); // ( R, U, D 

	// A few optimizations.
	glFrontFace(GL_CCW);
	glCullFace(GL_BACK);
	glEnable(GL_CULL_FACE);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	// Calculate proj_matrix for the first time.
	aspect = (float) windowWidth / (float) windowHeight;
	proj_matrix = glm::perspective(glm::radians(fovy), aspect, 0.1f, 1000.0f);
}

void update(GLfloat currentTime) {
	
	// TODO: Change processInput to accept the key, not a string
	// TODO: Change to use Array of Entities from Entity class
	// TODO: Go through only entites in array that in view
	
	camera->setDeltaTime(deltaTime);
	
	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) { camera->processInput("shift"); }
	
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) { camera->processInput("space"); }
	if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS) { camera->processInput("ctrl"); }

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) { camera->processInput("w"); }
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) { camera->processInput("s"); }
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) { camera->processInput("a"); }
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) { camera->processInput("d"); }
	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) { camera->processInput("left"); }
	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) { camera->processInput("right"); }

}

void render(GLfloat currentTime) {

	glViewport(0, 0, windowWidth, windowHeight);

	// Clear colour buffer
	vec4 backgroundColor = vec4(0.2f, 0.2f, 0.2f, 1.0f); 
	glClearBufferfv(GL_COLOR, 0, &backgroundColor[0]);

	// Clear deep buffer
	static const GLfloat one = 1.0f; 
	glClearBufferfv(GL_DEPTH, 0, &one);

	// Enable blend
	glEnable(GL_BLEND); 
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Use our shader programs
	glUseProgram(program);

	// Do some translations, rotations and scaling
	glm::mat4 modelMatrix = glm::translate(glm::mat4(1.0f), modelPosition);
	//modelMatrix = glm::rotate(modelMatrix, modelRotation.x, glm::vec3(1.0f, 0.0f, 0.0f));
	//modelMatrix = glm::rotate(modelMatrix, modelRotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
	//modelMatrix = glm::rotate(modelMatrix, modelRotation.z, glm::vec3(0.0f, 0.0f, 1.0f));
	//modelMatrix = glm::scale(modelMatrix, glm::vec3(0.2f, 0.2f, 0.2f));

	// glm::mat4 mv_matrix = viewMatrix * modelMatrix;

	camera->update();
	

	// TODO: each one of these is a separate thing in the scene, change to a loop for everything
	// TODO: Add lighting here ***
	// glUniform4f(glGetUniformLocation(objectModel.program, "lightPosition"),lightDisp.x, lightDisp.y, lightDisp.z, 1.0);
	glUniformMatrix4fv(glGetUniformLocation(program, "model_matrix"), 1, GL_FALSE, &modelMatrix[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(program, "view_matrix"), 1, GL_FALSE, &camera->getView()[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(program, "proj_matrix"), 1, GL_FALSE, &proj_matrix[0][0]);

	 cube->Draw();
}

void onResizeCallback(GLFWwindow* window, int w, int h) {
	windowWidth = w;
	windowHeight = h;

	aspect = (float)w / (float)h;
	proj_matrix = glm::perspective(glm::radians(fovy), aspect, 0.1f, 1000.0f);
}

void onKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	//if (action == GLFW_PRESS) keyStatus[key] = true;
	//else if (action == GLFW_RELEASE) keyStatus[key] = false;

	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GLFW_TRUE);
}

void onMouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {

}

void onMouseMoveCallback(GLFWwindow* window, double x, double y) {
	// TODO: Change to Entity to affect all objects
	camera->processMouse(x, y);
}

static void onMouseWheelCallback(GLFWwindow* window, double xoffset, double yoffset) {
	int yoffsetInt = static_cast<int>(yoffset);
}

void debugGL() {
	//Output some debugging information
	std::cout << std::endl;
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
/*
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
	cout << "-----------------------------------------" << endl;*/
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
	return "";
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


/*
Torch to be used when walking around
- head bob
- delay on rotation to camera

Spot Lights
Sun

Internal and External Structures

Lights behind grids for shadowed areas
- rotating
- translating

Train track with train going by

*/