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
		
		bool meshLoaded = false;
		/* Methods */
	protected:
		/* Members */
		/* Methods */
	public:
		/* Members */
		/* Methods */
		Object();
		Object(std::string file);
		Object(std::string file, vec3 position);

		void loadMesh(std::string file);

		virtual void draw();
		virtual void processInput(std::string key);
		virtual void processMouseInput(int button);
		virtual void processMouse(float x, float y);
	};
}