#pragma once
#include "Entity.h"
#include "Consumable.h"
#include <SFML\Graphics.hpp>
#define BIRD_MODEL_COUNT 3
#define BIRD_MODEL_ANIM_FRAMES 4

#define BIRD_STATUS_EFFECT_COUNT 2

enum BirdModel {
	BMPigeon,
	BMChicken,
	BMSparrow, //Vrabec
};

enum BirdAnimType {
	BATDefault,
	BATFlap,
	BATSlap,
	BATHit
};

enum BirdStatusEffects {
	BSEAirstrike = 0,
	BSEEnergyDrink
};

class Bird : public Entity {
public:
	static sf::Texture * textures[BIRD_MODEL_COUNT][BIRD_MODEL_ANIM_FRAMES];
	static sf::Texture * hatsTexture;
	static sf::Color plrColors[6];
	static float defTextureScale;
	static int defTextureWidth;
	static int defHatTextureWidth;
	static void loadTextures(int arg);

	//ANIMATION
	float scale = 1;
	float rotation = 0;
	bool facedRight = 1;

	//STATS
	BirdModel mdl;
	float MAXHP = 300;
	float HP = 300;
	bool dead = 0;
	float deadTime = 0;
	bool AI = 0;
	int PLR = 0;
	int HATID = 0;
	bool slappedNow = 0;

	float drag = 0.5;
	float flapStr = 900;
	float slapStr = 700;
	float movSpeed = 1200;
	float slapRange = 100;
	float statusEffects[BIRD_STATUS_EFFECT_COUNT];

	//TIMERS
	float flapAnimTime = 0;
	float flapAnimMaxTime = 0.4;
	float slapAnimTime = 0;
	float slapAnimMaxTime = 0.5;
	float shootCD = 0;
	float maxShootCD = 1;

	Bird();
	Bird(float xx, float yy, BirdModel t, int p);

	void update();
	void render(sf::RenderStates * states);
	void renderNoTexture(sf::RenderStates * states);

	void flap();
	void slap();
	void shoot();
	void moveLR(float val);
	void brake();
	void recDamage(float dmg, float vx, float fy);
	void consumeConsumable(Consumable * cons);
	void addStatusEffect(BirdStatusEffects typ, float time);
};