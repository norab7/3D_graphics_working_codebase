#pragma once
#include "Mesh.h"
#include "Entity.h"
using namespace _Entity;

namespace _Object {
	class Object : public Entity {
	private:
		/* Members */
		Mesh* mesh;
		
		bool meshLoaded = false;
		/* Methods */
	protected:
		/* Members */
		/* Methods */
	public:
		/* Members */
		std::string obj;
		std::string texture;
		/* Methods */
		Object(std::string file, std::string texture, vec3 position);

		void loadMesh(std::string file);

		virtual void draw();
		virtual void processInput(std::string key);
		virtual void processMouseInput(int button);
		virtual void processMouse(float x, float y);
	};
}