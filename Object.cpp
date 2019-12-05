#include "Object.h"
using namespace _Object;

Object::Object(std::string obj, std::string texture, vec3 position):Entity(position) {
	mesh = new Mesh();

	this->obj = obj;
	this->texture = texture;
	// this->position = position;

}

void Object::loadMesh(std::string obj) {
	this->obj = obj;
	mesh->LoadModel(obj);
}

const char* Object::get_texture() {
	return texture.c_str();
}

void Object::draw() {
	if (!meshLoaded) {
		mesh->LoadModel(obj);
		meshLoaded = true;
	}
	mesh->Draw();
}

void Object::processInput(std::string key) {
	if (key == "e") { 
		position.y += sin(interval*10) / 10;
	}
}

void Object::processMouseInput(int button) {
	// Blank
}

void Object::processMouse(float x, float y) {
	// Blank
}