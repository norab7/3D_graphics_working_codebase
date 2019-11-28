#include "Object.h"
using namespace _Object;

Object::Object(std::string obj, std::string texture, vec3 position) {
	this->obj = obj;
	this->texture = texture;
	this->position = position;

}

void Object::loadMesh(std::string file) {
	this->obj = file;
	mesh->LoadModel(file);
}

void Object::draw() {
	if (!meshLoaded) {
		mesh->LoadModel(obj);
		meshLoaded = true;
	}
	mesh->Draw();
}

void Object::processInput(std::string key) {
	// Blank
}

void Object::processMouseInput(int button) {
	// Blank
}

void Object::processMouse(float x, float y) {
	// Blank
}