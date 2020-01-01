#include "Projectile.h"
#include "Engine.h"

const int PROJECTILE_SCALE = 1.5;

Projectile::Projectile(float xx, float yy, float vxx, float vyy, int plr) {
	x = xx;
	y = yy;
	vx = vxx;
	vy = vyy;
	type = ETProjectile;
	radius = 15 * PROJECTILE_SCALE;

	PLR = plr;

}

void Projectile::update() {
	lifeTime += Engine::dt;
	if (lifeTime > armTime)
	{
		if (grounded) armed = 0;
		else armed = 1;
	}
}

void Projectile::render(sf::RenderStates * states) {
	sf::RectangleShape shape(sf::Vector2f(16 * PROJECTILE_SCALE, 16 * PROJECTILE_SCALE));
	shape.setFillColor(sf::Color(245, 243, 240));

	sf::RectangleShape shape2(sf::Vector2f(20 * PROJECTILE_SCALE, 20 * PROJECTILE_SCALE));
	shape2.setFillColor(sf::Color(20, 20, 20));

	shape2.move(x - 10 * PROJECTILE_SCALE, y - 10 * PROJECTILE_SCALE);
	Engine::windowPtr->draw(shape2);
	shape.move(x - 8 * PROJECTILE_SCALE, y - 8 * PROJECTILE_SCALE);
	Engine::windowPtr->draw(shape);
	
}
