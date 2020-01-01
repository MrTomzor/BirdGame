#include "Particle.h"
#include "Engine.h"

Particle::Particle(float xx, float yy, float vxx, float vyy, ParticleType t, void * d) {
	x = xx;
	y = yy;
	vx = vxx;
	vy = vyy;
	type = EntityType::ETParticle;
	ptype = t;
	if (ptype == ParticleType::PTLaser)
	{
		lifeTime = 1;
	}
	else
	{
		lifeTime = 20;
	}
	featherType = rand() % 10;
}

void Particle::update() {
	lifeTime -= Engine::dt;
	if (lifeTime < 0) deleteFlag = 1;
	if (grounded)
	{
		vx = 0;
	}
	vx -= vx * 0.9 * Engine::dt;
	vy -= vy * 4 * Engine::dt;
	if (ptype == PTLaser)
	{
		vx = 0;
		vy = 0;
		y = 0;
	}
}

void Particle::render(sf::RenderStates * states) {
	if (ptype == PTFeather)
	{
		sf::RectangleShape shape(sf::Vector2f(10, 10));
		sf::Color clr;
		if(featherType < 3) clr = sf::Color(60, 60, 60);
		else if(featherType < 7) clr = sf::Color(90, 90, 90);
		else clr = sf::Color(140, 140, 140);
		shape.setFillColor(clr);
		shape.move(x - 5, y - 5);
		Engine::windowPtr->draw(shape);
	}
	else if (ptype == PTLaser)
	{
		sf::RectangleShape shape(sf::Vector2f(10 + lifeTime * 100, 10000));
		sf::Color clr = sf::Color(255, 0, 0, 150);

		shape.setFillColor(clr);
		shape.move(x - shape.getSize().x / 2, y);
		Engine::windowPtr->draw(shape);
	}
}
