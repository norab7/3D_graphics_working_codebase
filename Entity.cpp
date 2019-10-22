#include "Entity.h"
using namespace _Entity;

Entity::Entity(vec3 position):Entity(position, vec3(position.x, position.y, position.z + 1), true) {
}

Entity::Entity(vec3 position, vec3 target):Entity(position, target, false) {
}

Entity::Entity(vec3 position, vec3 target, bool isStatic) {
	if (position == target) {
		// TODO: Check this works for central objects
		target = vec3(position.x, position.y, position.z + 1);
	}

	this->position = position;
	setTarget(target);
	this->isStatic = isStatic;
	right = normalize(cross(worldUp, direction));

	update();
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

mat4 Entity::getView() {
	return view;
}

void Entity::update() {
	if (isStatic) {
		view = lookAt(position, position - direction, worldUp); 
	}
}

void Entity::processInput(std::string key) {
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