// 3D Graphics and Animation - Main Template
// Visual Studio 2019
// Last Changed 01/10/2019

#include "Source.h"

// Prebuild Main Functions
void setupRender();
void startup();
void update();
void render();
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

// Objects
_Camera::Camera * camera = new _Camera::Camera(vec3(0,5,0),vec3(0,5,-1));

std::string dir_textures = "textures/";

vector<GLuint> textures = {};
vector<Object*> entities;
vector<Object> inv_entities;

// Comment and Uncomment ICE_LAKE and THE_ROOM respectfully to choice!! 

/* ICE_LAKE */
//bool ice_lake = true;
 //std::string dir_objects = "single_blends/";
 //vector<string> object_names = { "book", "crate","desk","drinks_can","filecabinet_tall","fire","Ice", "lamp_small","lamp_standing","piano","shack", "sofa","stool", "table_single","tv" };
 //vector<vec3> object_positions = { {7,-2,5},{7,0,5},{-5,0,12},{6.5,-2,5},{-8.5,0,12},{2,0,2},{0,0,0},{7.5,-2,5.5},{-2,0,12},{7,0,0},{0,0,-5},{-7,0,0},{2,0,0},{0,0,10},{0,-2,10} };
 //vector<vec3> object_pos_invert = { {-7,-2,5},{-7,0,5},{5,0,12},{-6.5,-2,5},{8.5,0,12},{-2,0,2},{0,10000,0},{-7.5,-2,5.5},{2,0,12},{-7,0,0},{0,0,-5},{7,0,0},{-2,0,0},{0,0,10},{0,-2,10} };
/* ICE_LAKE */

/* THE_ROOM */
bool ice_lake = false;
std::string dir_objects = "the_room/";
vector<string> object_names = { 
	"ashtray",
	"book","book001","book002","book003","book004","book005","book006","book007","book008","book009","book010",
	"crate","desk",
	"drinks_can","drinks_can001","drinks_can002",
	"filecabinet_tall","lamp_small","lamp_standing","piano", "pipe", "poster_alyx","room",
	"sofa","sofa001",
	"table_single", "table_single001","table_single002",
	"tv",
	"window","window001","window002","window003"};
vector<vec3> object_positions = {};
vector<vec3> object_pos_invert = {};
/* THE_ROOM */

vector<_Projectile::Projectile*> projectiles;

float spot_light_active = 1.0f;

int main() {
	std::cout << "Starting Up!" << std::endl << std::endl;

	// Order of flow for setting up a window in OpenGL
	init();
	callbackSetup(); 
	debugGL();
	setupRender();	

	int i;
	for (i = 0; i < object_names.size(); i++) {
		std::string object_string = dir_objects + object_names.at(i) + ".obj";
		std::string texture_string = dir_textures + object_names.at(i) + ".jpg";

		// Object *item = new _Object::Object(object_string, texture_string.c_str(), object_positions.at(i));
		entities.push_back(new _Object::Object(object_string, texture_string.c_str(), (object_positions.size() < 1) ? vec3(0,0,0): object_positions.at(i)));
	}
	if (ice_lake) {
		for (i = 0; i < object_names.size(); i++) {
			std::string object_string = dir_objects + object_names.at(i) + ".obj";
			std::string texture_string = dir_textures + object_names.at(i) + ".jpg";

			Object* item = new _Object::Object(object_string, texture_string.c_str(), (object_positions.size() < 1) ? vec3(0, 0, 0) : object_positions.at(i));
			item->worldUp = (i != 6) ? vec3(0, -1, 0) : vec3(0, 1, 0);
			inv_entities.push_back(*item);
		}
	}


	// Setup all necessary information for startup (aka. load texture, shaders, models, etc).
	startup();		

	// Main loop of program
	std::cout << std::endl << " --- Starting Game Loop --- " << std::endl;
	do {
		GLfloat currentFrame = (GLfloat)glfwGetTime();		// retrieve timelapse
		deltaTime = currentFrame - lastFrame;					// Calculate delta time
		lastFrame = currentFrame;								// Save for next frame calculations.
		glfwPollEvents();									// poll callbacks
		update();											// update (physics, animation, structures, etc)
		render();								// call render function.

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

	// Linking
	glLinkProgram(program);
	glUseProgram(program);

	int i;
	for (i = 0; i < entities.size(); i++) {
		GLuint tex;
		textures.push_back(tex);

		glGenTextures(1, &textures.at(i));
		glBindTexture(GL_TEXTURE_2D, textures.at(i));

		int width, height, nrChannels;
		stbi_set_flip_vertically_on_load(true);
		
		std::cout << "texture output:: " << entities.at(i)->get_texture() << std::endl;
		unsigned char* data = stbi_load(entities.at(i)->get_texture(), &width, &height, &nrChannels, 0);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
		stbi_image_free(data);

	}

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

void update() {
	
	// TODO: Change processInput to accept the key, not a string
	// TODO: Change to use Array of Entities from Entity class
	// TODO: Go through only entites in array that in view
	// TODO: Create inheritance conversion for camera to entity
	
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

	if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS) { spot_light_active *= -1.0f; }

	for (Object *o : entities) {
		if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) { o->processInput("e"); }

	}

}

void render() {

	glViewport(0, 0, windowWidth, windowHeight);

	// Clear colour buffer
	vec4 backgroundColor = vec4(0, 0, 0, 1);//vec4(0.2f, 0.2f, 0.6f, 1.0f); 
	glClearBufferfv(GL_COLOR, 0, &backgroundColor[0]);

	// Clear deep buffer
	static const GLfloat one = 1.0f; 
	glClearBufferfv(GL_DEPTH, 0, &one);

	// Enable blend
	glEnable(GL_BLEND); 
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Use our shader programs
	glUseProgram(program);


	// Light and Material values generated via following LearnOpenGL tutorials
	// TODO: Turn into light_object for quick simple calls
	/* Materials */
	glUniform1f(glGetUniformLocation(program, "material.shininess"), 32);

	// Directional Light
	glUniform3f(glGetUniformLocation(program, "d_light.direction"), -0.2, -10, -0.2);
	glUniform3f(glGetUniformLocation(program, "d_light.ambient"), 0,0,0);
	glUniform3f(glGetUniformLocation(program, "d_light.diffuse"), 0, 0, 0);
	glUniform3f(glGetUniformLocation(program, "d_light.specular"), 0, 0, 0);

	float flicker = (rand() % 100 < 95) ? 0.1 : 0.3;
	float fire_flicker = ((((float)(rand() % 50)) + 30) / 100.0);
	fire_flicker = (ice_lake) ? fire_flicker : flicker;

	// Point Light 1 - TV
	glUniform3f(glGetUniformLocation(program, "p_light[0].position"), -3.25,3,3.5);
	glUniform3f(glGetUniformLocation(program, "p_light[0].ambient"), 0.005, 0.005, 0.005);
	glUniform3f(glGetUniformLocation(program, "p_light[0].diffuse"), fire_flicker, flicker, flicker);
	glUniform3f(glGetUniformLocation(program, "p_light[0].specular"), 0,0,0);
	glUniform1f(glGetUniformLocation(program, "p_light[0].constant"), 0.01);
	glUniform1f(glGetUniformLocation(program, "p_light[0].linear"), 0.09);
	glUniform1f(glGetUniformLocation(program, "p_light[0].quadratic"), 0.032);

	// Point Light 2 - Standing Lamp
	glUniform3f(glGetUniformLocation(program, "p_light[1].position"), 4.7, 6, -5.75);
	glUniform3f(glGetUniformLocation(program, "p_light[1].ambient"), 0.05, 0.05, 0.05);
	glUniform3f(glGetUniformLocation(program, "p_light[1].diffuse"), 0.1, 0.1, 0.1);
	glUniform3f(glGetUniformLocation(program, "p_light[1].specular"), 0,0,0);
	glUniform1f(glGetUniformLocation(program, "p_light[1].constant"), 0.1);
	glUniform1f(glGetUniformLocation(program, "p_light[1].linear"), 0.09);
	glUniform1f(glGetUniformLocation(program, "p_light[1].quadratic"), 0.032);

	// Point Light 3 - Small Lamp
	glUniform3f(glGetUniformLocation(program, "p_light[2].position"), 4.7, 4, 3.25);
	glUniform3f(glGetUniformLocation(program, "p_light[2].ambient"), 0.005, 0.005, 0.005);
	glUniform3f(glGetUniformLocation(program, "p_light[2].diffuse"), 0.01, 0.01, 0.01);
	glUniform3f(glGetUniformLocation(program, "p_light[2].specular"), 0, 0, 0);
	glUniform1f(glGetUniformLocation(program, "p_light[2].constant"), 0.01);
	glUniform1f(glGetUniformLocation(program, "p_light[2].linear"), 0.09);
	glUniform1f(glGetUniformLocation(program, "p_light[2].quadratic"), 0.032);

	// Spot Light
	float spot_light_flash = (rand() % 100 < 95) ? 1.0 : 0.0;

	glUniform1f(glGetUniformLocation(program, "spot_light_flash"), spot_light_flash);
	glUniform1f(glGetUniformLocation(program, "spot_light_active"), spot_light_active);

	// Torch
	glUniform3f(glGetUniformLocation(program, "s_light[0].position"), camera->position.x, camera->position.y + (sin(lastFrame)/ 10), camera->position.z);
	glUniform3f(glGetUniformLocation(program, "s_light[0].direction"), camera->getFront().x, camera->getFront().y, camera->getFront().z);
	glUniform3f(glGetUniformLocation(program, "s_light[0].ambient"), 0,0,0);
	glUniform3f(glGetUniformLocation(program, "s_light[0].diffuse"), 1, 1, 1);
	glUniform3f(glGetUniformLocation(program, "s_light[0].specular"), 1, 1, 1);
	glUniform1f(glGetUniformLocation(program, "s_light[0].constant"), 1);
	glUniform1f(glGetUniformLocation(program, "s_light[0].linear"), 0.09);
	glUniform1f(glGetUniformLocation(program, "s_light[0].quadratic"), 0.032);
	glUniform1f(glGetUniformLocation(program, "s_light[0].cutOff"), cos(radians(15.0f)));
	glUniform1f(glGetUniformLocation(program, "s_light[0].outerCutOff"), cos(radians(25.0f)));

	// Standing Lamp
	glUniform3f(glGetUniformLocation(program, "s_light[1].position"), 4.7, 5, -5.75);
	glUniform3f(glGetUniformLocation(program, "s_light[1].direction"), 0, -1, 0);
	glUniform3f(glGetUniformLocation(program, "s_light[1].ambient"), 0, 0, 0);
	glUniform3f(glGetUniformLocation(program, "s_light[1].diffuse"), 1, 1, 1);
	glUniform3f(glGetUniformLocation(program, "s_light[1].specular"), 1, 1, 1);
	glUniform1f(glGetUniformLocation(program, "s_light[1].constant"), 1);
	glUniform1f(glGetUniformLocation(program, "s_light[1].linear"), 0.09);
	glUniform1f(glGetUniformLocation(program, "s_light[1].quadratic"), 0.032);
	glUniform1f(glGetUniformLocation(program, "s_light[1].cutOff"), cos(radians(40.0f)));
	glUniform1f(glGetUniformLocation(program, "s_light[1].outerCutOff"), cos(radians(45.0f)));

	// Small Lamp
	glUniform3f(glGetUniformLocation(program, "s_light[2].position"), 4.7, 4, 3.25);
	glUniform3f(glGetUniformLocation(program, "s_light[2].direction"), 0, -1, 0);
	glUniform3f(glGetUniformLocation(program, "s_light[2].ambient"), 0, 0, 0);
	glUniform3f(glGetUniformLocation(program, "s_light[2].diffuse"), 0.5, 0.5, 0.5);
	glUniform3f(glGetUniformLocation(program, "s_light[2].specular"), 0,0,0);
	glUniform1f(glGetUniformLocation(program, "s_light[2].constant"), 1);
	glUniform1f(glGetUniformLocation(program, "s_light[2].linear"), 0.09);
	glUniform1f(glGetUniformLocation(program, "s_light[2].quadratic"), 0.032);
	glUniform1f(glGetUniformLocation(program, "s_light[2].cutOff"), cos(radians(20.0f)));
	glUniform1f(glGetUniformLocation(program, "s_light[2].outerCutOff"), cos(radians(25.0f)));

	/* Lighting */
	// Ceiling Lamp
	glUniform3f(glGetUniformLocation(program, "s_light[3].position"), sin(lastFrame), 6, cos(lastFrame));
	glUniform3f(glGetUniformLocation(program, "s_light[3].direction"), 0, -1, 0);
	glUniform3f(glGetUniformLocation(program, "s_light[3].ambient"), 0.05, 0.05, 0.05);
	glUniform3f(glGetUniformLocation(program, "s_light[3].diffuse"), 0.8, 0.8, 0.8);
	glUniform3f(glGetUniformLocation(program, "s_light[3].specular"), 1, 1, 1);
	glUniform1f(glGetUniformLocation(program, "s_light[3].constant"), 1);
	glUniform1f(glGetUniformLocation(program, "s_light[3].linear"), 0.09);
	glUniform1f(glGetUniformLocation(program, "s_light[3].quadratic"), 0.032);
	glUniform1f(glGetUniformLocation(program, "s_light[3].cutOff"), cos(radians(40.0f)));
	glUniform1f(glGetUniformLocation(program, "s_light[3].outerCutOff"), cos(radians(60.0f)));

	// Investigator Torch

	vec3 torch_pos(camera->position.x + sin(lastFrame) * 3, 5, -12);
	vec3 torch_direction = camera->position - torch_pos;

	glUniform3f(glGetUniformLocation(program, "s_light[4].position"), torch_pos.x, torch_pos.y, torch_pos.z);
	glUniform3f(glGetUniformLocation(program, "s_light[4].direction"), torch_direction.x, torch_direction.y - 3, torch_direction.z);
	glUniform3f(glGetUniformLocation(program, "s_light[4].ambient"), 1, 1, 1);
	glUniform3f(glGetUniformLocation(program, "s_light[4].diffuse"), 1, 1, 1);
	glUniform3f(glGetUniformLocation(program, "s_light[4].specular"), 1, 1, 1);
	glUniform1f(glGetUniformLocation(program, "s_light[4].constant"), 1);
	glUniform1f(glGetUniformLocation(program, "s_light[4].linear"), 0.09);
	glUniform1f(glGetUniformLocation(program, "s_light[4].quadratic"), 0.032);
	glUniform1f(glGetUniformLocation(program, "s_light[4].cutOff"), cos(radians(5.0f)));
	glUniform1f(glGetUniformLocation(program, "s_light[4].outerCutOff"), cos(radians(15.0f)));

	camera->update();
	glUniform3f(glGetUniformLocation(program, "viewPos"), camera->position.x, camera->position.y, camera->position.z);
	glUniformMatrix4fv(glGetUniformLocation(program, "proj"), 1, GL_FALSE, &proj_matrix[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(program, "view"), 1, GL_FALSE, &camera->getMatrix()[0][0]);

	// Main Objects
	for (int i = 0; i < entities.size(); i++) {
		glUniform1f(glGetUniformLocation(program, "count"), i);
		entities.at(i)->update(lastFrame);

		/* Textures */
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textures.at(i));
		glUniform1i(glGetUniformLocation(program, ("tex")), 0);
	

		glUniformMatrix4fv(glGetUniformLocation(program, "model"), 1, GL_FALSE, &entities.at(i)->getMatrix()[0][0]);
		entities.at(i)->draw();

		if (ice_lake) {
			inv_entities.at(i).update(lastFrame);
			glUniformMatrix4fv(glGetUniformLocation(program, "model"), 1, GL_FALSE, &inv_entities.at(i).getMatrix()[0][0]);
			inv_entities.at(i).draw();
		}
	}


	for (int i = 0; i < projectiles.size(); i++) {
		if (!projectiles.at(i)->active) {
			projectiles.at(i)->kill(projectiles.at(i));
			projectiles.erase(projectiles.begin() + i);
			continue;
		} 
		projectiles.at(i)->update(lastFrame);
		glUniformMatrix4fv(glGetUniformLocation(program, "model"), 1, GL_FALSE, &projectiles.at(i)->getMatrix()[0][0]);
		projectiles.at(i)->draw();
	}
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
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_1) == GLFW_PRESS) {
		std::cout << "mouseButton1" << std::endl;

		int i;
		for (i = 0; i < entities.size(); i++) {
			if (entities.at(i)->texture.find("tv") != string::npos) {
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, textures.at(i));
				glUniform1i(glGetUniformLocation(program, ("tex")), 0);
			}
		}

		projectiles.push_back(new _Projectile::Projectile("single_blends/tv.obj", "textures/tv.jpg", (-camera->getPosition()) - (camera->getFront() * 2.0f), camera->getFront(), lastFrame, 1000));
		projectiles.at(projectiles.size() - 1)->target = camera->getFront();

	}
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_2) == GLFW_PRESS) {}
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_3) == GLFW_PRESS) {}
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
	vector<GLchar> errorLog(maxLength);

	if (maxLength > 1) {
		// Get the log file
		glGetShaderInfoLog(shader, maxLength, &maxLength, &errorLog[0]);

		cout << "--------------Shader compilation error-------------\n";
		cout << errorLog.data();
	}
}

