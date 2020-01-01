#pragma once
#include "Entity.h"

constexpr auto CONSUMABLE_TYPE_COUNT = 2;;

enum ConsumableType {
	CTAirstrike = 0,
	CTEnergyDrink,
	/*CTChilli,
	CTBozkov,
	CTGrenade,
	CTKnife,
	CTMath*/
};

class Consumable : public Entity {
public:
	static sf::Texture * consumablesTexture;
	static void loadTextures(int arg);

	ConsumableType ctype;
	bool pickupable;
	float rotation = 0;

	Consumable(float xx, float yy, ConsumableType t);

	void update();
	void render(sf::RenderStates * states);
	
};