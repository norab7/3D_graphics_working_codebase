#include "Object.h"
using namespace _Object;

Object::Object() {
	mesh = new Mesh();
}

Object::Object(std::string file):Object(file, vec3(0,0,0)) {
}

Object::Object(std::string file, vec3 position):Object() {
	this->file = file;
	this->position = position;
}

void Object::loadMesh(std::string file) {
	this->file = file;
	mesh->LoadModel(file);
}

void Object::draw() {
	if (!meshLoaded) {
		mesh->LoadModel(file);
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