#pragma once
#include <SFML\Graphics.hpp>

enum EntityType {
	ETBird,
	ETProjectile,
	ETParticle,
	ETConsumable,
};

class Entity {
public:
	EntityType type;
	int layerMask = 0b1111;
	bool useGravity = true;
	bool grounded = 0;
	bool deleteFlag = false;
	float mass = 1;
	float radius = 30;
	float x, y;
	float vx, vy;
	Entity();
	Entity(float xx, float yy);

	virtual void update();
	virtual void render(sf::RenderStates * states);
	void addVelocity(float vxx, float vyy);
	float distTo(Entity * e2);
};