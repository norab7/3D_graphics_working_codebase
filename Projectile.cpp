#include "Projectile.h"
using namespace _Projectile;

Projectile::Projectile(std::string file,std::string texture, vec3 position, vec3 direction, float initialFrame, float duration, bool child)
	:Object(file, texture, position) {
	this->isChild = child;
	this->direction = direction;
	this->duration = duration;
	this->initialFrame = initialFrame;
}

void Projectile::kill(Projectile * explode) {
	// std::cout << "Projectile Death" << std::endl;
	if (!isChild) {
		
	}
	delete this;
}

void Projectile::update(float interval) {
	Object::update(interval);

	if ((this->interval - initialFrame) >= 5) {
		active = false;
	}
	position += 0.05f * (-direction);
}
