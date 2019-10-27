#include "Cube.h"
using namespace _Cube;

Cube::Cube() {
	loadMesh("cube_uv.obj");
}

Cube::Cube(float x, float y, float z):Cube(vec3(x,y,z)) {

}

Cube::Cube(vec3 position):Cube() {
	this->position = position;
}

Cube::Cube(vec3 position, vec3 rotation):Cube() {
	this->position = position;
	// this->rotation = rotation; // not yet implemented
}

Cube::Cube(mat4 matrix):Cube() {
	this->matrix = matrix;
	this->position = vec3(matrix[0][0], matrix[1][1], matrix[2][2]);
}