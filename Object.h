#pragma once
#include "Mesh.h"
#include "Entity.h"
using namespace _Entity;

namespace _Object {
	class Object : public Entity {

	public:
		/* Members */
		/* Methods */
		Mesh* mesh;
		
		bool meshLoaded = false;
		std::string obj;
		std::string texture;

		Object(std::string file, std::string texture, vec3 position);

		void loadMesh(std::string file);

		const char* get_texture();

		virtual void draw();
		virtual void processInput(std::string key);
		virtual void processMouseInput(int button);
		virtual void processMouse(float x, float y);
	};
}