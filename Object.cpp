#include "Object.h"
using namespace _Object;

Object::Object() {
	mesh = new Mesh();
}

Object::Object(std::string file):Object() {
	loadMesh(file);
}

void Object::loadMesh(std::string file) {
	this->file = file;
	mesh->LoadModel(file);
}

void Object::draw() {
	mesh->Draw();
}

void Object::processInput(std::string key) {

}

void Object::processMouse(float x, float y) {

}