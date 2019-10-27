#include "Entity.h"
using namespace _Entity;

Entity::Entity(vec3 position):Entity(position, vec3(position.x, position.y, position.z + 1), true) {
}

Entity::Entity(vec3 position, vec3 target):Entity(position, target, false) {
}

Entity::Entity(vec3 position, vec3 target, bool isStatic) {
	if (position == target) {
		// TODO: Check this works for central objects
		printMsg("Position and Target are the same; Adjusting vectors...");
		target = vec3(position.x, position.y, position.z + 1);
	}

	this->position = position;
	this->isStatic = isStatic;

	setTarget(target);

	// Order of glm::cross parameters matters!
	right = normalize(cross(direction, worldUp));
}

void Entity::translation(vec3 translation) {
	position = position + translation;
}

void Entity::translation(float x, float y, float z) {

}

void Entity::rotation(float degrees, vec3 axis) {
	matrix = rotate(matrix, radians(degrees), axis);
}

void Entity::doRotation(float degrees, vec3 axis, int duration) {

}

void Entity::animate(vec3* animations, float* durations) {

}

void Entity::setPosition(vec3 position) {
	this->position = position;
}

void Entity::setTarget(vec3 target) {
	this->target = target;
	direction = normalize(target - position);
}

void _Entity::Entity::setDeltaTime(float deltaTime) {
	this->deltaTime = deltaTime;
}

vec3 Entity::getPosition() {
	return position;
}

mat4 Entity::getMatrix() {
	return matrix;
}

void Entity::update() {
	matrix = lookAt(position, position - direction, worldUp);
}

void Entity::update(GLfloat interval) {
	this->interval = interval;
	update();
}

void Entity::draw() {
	// Intentionally Blank
}

void Entity::processInput(std::string key) {
	// Intentionally Blank
}

void Entity::processMouseInput(int button) {
	// Intentionally Blank
}

void Entity::processMouse(float x, float y) {
	if (firstMouse) {
		mouseX = x;
		mouseY = y;
		firstMouse = false;
	}

	offsetX = (x - mouseX) * mouseSensitivity;
	offsetY = (mouseY - y) * mouseSensitivity;
	mouseX = x;
	mouseY = y;
}

void Entity::printVector( vec3 vector) {
	std::cout << "Pos:(" << vector.x << "," << vector.y << "," << vector.z << ")" << std::endl;
}

void Entity::printMsg( std::string msg ) {
	std::cout << msg << std::endl;
}