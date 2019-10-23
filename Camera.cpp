#include "Camera.h"
using namespace _Camera;

Camera::Camera():Camera(vec3(0.0f, 0.0f, 5.0f), vec3(0.0f, 0.0f, 0.0f)) {
}

Camera::Camera(vec3 position, vec3 target) :Entity(position, target) {
	front = direction;
	cameraUp = normalize(cross(right, front));
	update();

	std::cout << "Camera Setup Complete" << std::endl;
}

void Camera::update() {
	view = lookAt(position, position + front, cameraUp);
}

void Camera::processInput(std::string key) {

	float movementSpeed = (cameraSpeed * deltaTime) * speedModifier;

	if (key == "space") { position -= movementSpeed * normalize(cross(front, right)); }
	if (key == "ctrl") { position += movementSpeed * normalize(cross(front, right)); }

	if (key == "w") { position += movementSpeed * front; }
	if (key == "s") { position -= movementSpeed * front; }
	if (key == "a") { position -= movementSpeed * normalize(cross(front, cameraUp)); }
	if (key == "d") { position += movementSpeed * normalize(cross(front, cameraUp)); }

	if (key == "shift") {
		speedModifier = 2.0f; 
	} else { 
		speedModifier = 1.0f;

		printMsg("position and front");
		printVector(position);
		printVector(front);
	}
}

void Camera::processMouse(float x, float y){
	Entity::processMouse(x, y);

	

	// Default yaw value as -90.0f
	yaw += offsetX;
	pitch += offsetY;

	if (pitch > 89.0f) { pitch = 89.0f; }
	if (pitch < -89.0f) { pitch = -89.0f; }

	front.x = cos(radians(yaw)) * cos(radians(pitch));
	front.y = sin(radians(pitch));
	front.z = sin(radians(yaw)) * cos(radians(pitch));
	
	front = normalize(front);
	right = normalize(cross(front, worldUp));
	cameraUp = normalize(cross(right, front));
}

/*
Cube:
	Top = Green
	Right = Red
	Face = Blue
*/