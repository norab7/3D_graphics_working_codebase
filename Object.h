#pragma once
#include "Mesh.h"
#include "Entity.h"
using namespace _Entity;

namespace _Object {
	class Object : public Entity {
	private:
		/* Members */
		std::string file;
		Mesh* mesh;
		/* Methods */
	protected:
		/* Members */
		/* Methods */
	public:
		/* Members */
		/* Methods */
		Object();
		Object(std::string file);

		void loadMesh(std::string file);
		void draw();

		virtual void processInput(std::string key);
		virtual void processMouse(float x, float y);
	};
}