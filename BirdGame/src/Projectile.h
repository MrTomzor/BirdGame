#pragma once
#include "Entity.h"

class Projectile : public Entity {
public:
	int PLR;
	float DMG = 100;
	float lifeTime = 0;
	float armTime = 0.4;
	bool armed = 0;
	Projectile(float xx, float yy, float vx, float vy, int plr);

	void update();
	void render(sf::RenderStates * states);
};