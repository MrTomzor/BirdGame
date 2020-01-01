#pragma once
#include "Entity.h"

enum ParticleType {
	PTFeather,
	PTBlood,
	PTLaser
};
class Particle : public Entity {
public:
	ParticleType ptype;
	float lifeTime;
	int featherType;
	Particle(float x, float y, float vx, float vy, ParticleType t, void * d);

	void update();
	void render(sf::RenderStates * states);

};