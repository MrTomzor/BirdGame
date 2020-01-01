#include "Consumable.h"
#include "Engine.h"

sf::Texture * Consumable::consumablesTexture = NULL;

void Consumable::loadTextures(int arg) {
	consumablesTexture = Engine::resourceLoader->getTexture("consumables.png");
}

Consumable::Consumable(float xx, float yy, ConsumableType t) {
	x = xx;
	y = yy;
	vx = 0;
	vy = 0;
	ctype = t;
	type = EntityType::ETConsumable;
	pickupable = 1;
	radius = 10;
	useGravity = 0;
}

void Consumable::update() {
	if(!grounded)
		rotation += 120 * Engine::dt;
}

void Consumable::render(sf::RenderStates * states) {
	float realScale = Bird::defTextureScale * 2;
	sft::drawSprite(Engine::windowPtr, x, y, Bird::defHatTextureWidth, realScale, realScale, rotation,
		consumablesTexture, new sf::IntRect(ctype * Bird::defHatTextureWidth, 0, Bird::defHatTextureWidth, Bird::defHatTextureWidth));
}
