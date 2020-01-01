#include "Entity.h"
#include "Engine.h"
#include <iostream>

Entity::Entity() {

}

Entity::Entity(float xx, float yy) {
	x = xx;
	y = yy;
}

void Entity::update() {

}

void Entity::render(sf::RenderStates * states) {
	sf::CircleShape shape(20.f);
	shape.setFillColor(sf::Color(0, 255, 0));
	shape.move(x - 10, y - 10);
	Engine::windowPtr->draw(shape);
}

void Entity::addVelocity(float vxx, float vyy) {
	if (grounded && vyy != 0)
	{
		std::cout << "Grounded atk" << std::endl;
		if (vyy > 0) vyy = -vyy;
		grounded = 0;
	}
	vx += vxx;
	vy += vyy;
}

float Entity::distTo(Entity * e2) {
	float dx = x - e2->x;
	float dy = y - e2->y;
	return sqrt(dx * dx + dy * dy);
}
