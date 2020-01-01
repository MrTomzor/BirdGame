#include "Bird.h"
#include "Engine.h"
#include "GameScene.h"
#include "Particle.h"
#include "Projectile.h"
#include <iostream>

sf::Texture * Bird::textures[BIRD_MODEL_COUNT][BIRD_MODEL_ANIM_FRAMES];
sf::Texture * Bird::hatsTexture;
sf::Color Bird::plrColors[6] = { sf::Color(200,0,0), sf::Color(0,200,0) ,sf::Color(0,0,200) ,sf::Color(200,200,0) ,sf::Color(0,200,200), sf::Color(200,0,200) };
int Bird::defTextureWidth = 128;
int Bird::defHatTextureWidth = 64;
float Bird::defTextureScale = 2;

void Bird::loadTextures(int arg) {
	textures[BMPigeon][0] = Engine::resourceLoader->getTexture("pigeon_standing.png");
	textures[BMPigeon][1] = Engine::resourceLoader->getTexture("pigeon_flapping.png");
	textures[BMPigeon][2] = Engine::resourceLoader->getTexture("pigeon_flying.png");
	textures[BMPigeon][3] = Engine::resourceLoader->getTexture("pigeon_slapping.png");
	hatsTexture = Engine::resourceLoader->getTexture("hats.png");
}

Bird::Bird() {
	type = EntityType::ETBird;
	vx = 1;
	vy = 0;
	mass = 1;
	for (int i = 0; i < BIRD_STATUS_EFFECT_COUNT; i++)
	{
		statusEffects[i] = 0;
	}
}

Bird::Bird(float xx, float yy, BirdModel t, int plr) : Bird::Bird() {
	mdl = t;
	x = xx;
	y = yy;
	PLR = plr;
	HATID = PLR;

	radius = 90;
	//hatTexture = Engine::resourceLoader->getTexture("");
}

void Bird::update() {
	//TIMERS
	if (flapAnimTime > 0)
	{
		flapAnimTime -= Engine::dt;
		if (flapAnimTime < 0)	flapAnimTime = 0;
	}
	if (shootCD > 0)
	{
		shootCD -= Engine::dt;
		if (shootCD < 0) shootCD = 0;
	}
	if (slapAnimTime > 0)
	{
		slapAnimTime -= Engine::dt;
		if (slapAnimTime < 0) slapAnimTime = 0;
	}
	
	//DRAG
	vx -= grounded ? vx * drag * 5 * Engine::dt : vx * drag * Engine::dt;
	vy -= vy * drag * Engine::dt;

	if (dead)
	{
		deadTime += Engine::dt;
		if (!grounded)
		{
			if(vx> 0) rotation += Engine::dt * 200;
			else rotation -= Engine::dt * 200;
		}
	}
	else
	{
		rotation = vx / 50.0;
	}
	statusEffects[BirdStatusEffects::BSEEnergyDrink] -= Engine::dt;
	//AI
	if (AI)
	{
		if (y > 400)
		{
			flap();
		}
	}
}

void Bird::render(sf::RenderStates * states) {
	sf::Texture * txtr = NULL;
	scale = 1;

	float hatCenterOffsetX = 20;
	float hatCenterOffsetY = -30;

	if (dead)
	{
		txtr = Bird::textures[mdl][0];
	}
	else if (slapAnimTime > 0)
	{
		txtr = Bird::textures[mdl][3];
		scale = 1 + 0.5 * slapAnimTime / slapAnimMaxTime;
	}
	else if (flapAnimTime > 0)
	{
		txtr = Bird::textures[mdl][1];
		hatCenterOffsetY = -26;
	}
	else if (grounded && abs(vx) < 10)
	{
		txtr = Bird::textures[mdl][0];
	}
	else
	{
		txtr = Bird::textures[mdl][2];
		hatCenterOffsetY = -23;
		hatCenterOffsetX = 18;
	}
	float realScale = Bird::defTextureScale * scale;
	sft::drawSprite(Engine::windowPtr, x, y, Bird::defTextureWidth, facedRight ? realScale : -realScale, realScale, rotation, txtr, NULL);
	
	//HAT DRAWING
	sf::Vector2f hatTranslateVector = sft::rotateVectorDegs(hatCenterOffsetX * (facedRight ? realScale : -realScale), hatCenterOffsetY * realScale, -rotation);
	
	sft::drawSprite(Engine::windowPtr, x + hatTranslateVector.x, y + hatTranslateVector.y, Bird::defHatTextureWidth, facedRight ? realScale : -realScale, realScale, rotation,
		hatsTexture, new sf::IntRect(HATID * Bird::defHatTextureWidth, 0, Bird::defHatTextureWidth, Bird::defHatTextureWidth));
	
	//sft::fillRect(Engine::windowPtr, x, y - radius * 2, 50 * HP / MAXHP, 50 * HP / MAXHP, plrColors[PLR]);

}

void Bird::renderNoTexture(sf::RenderStates * states) {
	if (dead) return;
	float rectSizeHalf = 25;
	if (slapAnimTime > 0)
	{
		float animBonus = slapAnimTime * 30;
		sf::RectangleShape shape(sf::Vector2f(rectSizeHalf * 2 + animBonus, rectSizeHalf * 2 + animBonus));
		switch (PLR) //TMP
		{
		case 0:
			shape.setFillColor(sf::Color(176, 6, 0));
			break;
		case 1:
			shape.setFillColor(sf::Color(0, 255, 0));
			break;
		case 2:
			shape.setFillColor(sf::Color(180, 66, 60));
			break;
		case 3:
			shape.setFillColor(sf::Color(0, 255, 255));
			break;
		}
		shape.move(x - rectSizeHalf - animBonus / 2, y - rectSizeHalf - animBonus / 2);
		Engine::windowPtr->draw(shape);
		return;
	}
	sf::RectangleShape shape(sf::Vector2f(rectSizeHalf * 2, rectSizeHalf * 2));
	switch (PLR) //TMP
	{
	case 0:
		shape.setFillColor(sf::Color(176, 6, 0));
		break;
	case 1:
		shape.setFillColor(sf::Color(0, 255, 0));
		break;
	case 2:
		shape.setFillColor(sf::Color(180, 66, 60));
		break;
	case 3:
		shape.setFillColor(sf::Color(0, 255, 255));
		break;
	}

	shape.move(x - rectSizeHalf, y - rectSizeHalf);
	Engine::windowPtr->draw(shape);
}

void Bird::flap() {
	if (dead) return;
	if (flapAnimTime == 0)
	{
		vy -= flapStr * (statusEffects[BirdStatusEffects::BSEEnergyDrink] > 0 ? 2 : 1);
		flapAnimTime = flapAnimMaxTime;
		Engine::audioPlayer->playSound("wing_flap.wav");
	}
}

void Bird::slap() {
	if (slapAnimTime > 0 || dead) return;
	slapAnimTime = slapAnimMaxTime;

	std::list<Entity *> ents = ((GameScene *)(Engine::scenePtr))->getEntsCircle(x, y, slapRange);
	std::list<Entity *>::iterator eit = ents.begin();
	while (eit != ents.end())
	{
		Entity * ent2 = *eit;
		if (ent2 != this && ent2->type == EntityType::ETBird) //found valid target in range that is not this entity
		{
			

			float dx = (ent2->x - x);
			float dy = (ent2->y - y);
			float mag = sqrt(dx * dx + dy * dy);
			float pushModX = dx / mag;
			float pushModY = dy / mag;
			float dvx = (slapStr * pushModX + vx * mass) / ent2->mass;
			float dvy = (slapStr * pushModY + vy * mass) / ent2->mass;

			float dmg = 20 + (abs(vx) + abs(vy)) / 10;
			std::cout << "DMG: " << dmg << std::endl;

			Bird * bird2 = ((Bird *)(*eit));
			bird2->recDamage(dmg, dvx, dvy);
			
			if (grounded)
			{
				dvy = -slapStr / 2;
			}

			ent2->addVelocity(dvx, dvy);
		}
		eit++;
	}
}

void Bird::shoot() {
	if (shootCD > 0) return;

	if (statusEffects[BirdStatusEffects::BSEAirstrike])
	{
		Engine::audioPlayer->playSound("airstrike.wav");
		((GameScene *)Engine::scenePtr)->spawnEntity(new Particle(x, 0, 0, 0, ParticleType::PTLaser, &PLR));
		for (int i = 0; i < 20; i++)
		{
			((GameScene *)Engine::scenePtr)->spawnEntity(new Projectile(x + rand() % 800 - 400,  -1000 - rand() % 5000, 0, 100, PLR));
		}
		statusEffects[BirdStatusEffects::BSEAirstrike] = 0;
		shootCD = maxShootCD;
		return;
	}
	shootCD = maxShootCD;
	((GameScene *)Engine::scenePtr)->spawnEntity(new Projectile(x, y - 20, 0, 300 + vy, PLR));
	Engine::audioPlayer->playSound("sht_sound.wav");
}

void Bird::moveLR(float val) {
	
	if (abs(val) < 0.15 || dead) return;
	//hopping
	if (val > 0) facedRight = 1;
	else facedRight = 0;
	if (grounded)
	{
		grounded = 0;
		vy = -120;
	}
	vx += movSpeed * val * Engine::dt * (statusEffects[BirdStatusEffects::BSEEnergyDrink] > 0 ? 2 : 1);;
}

void Bird::brake() {
	//vx = 0;
}

void Bird::recDamage(float dmg, float dvx, float dvy) {
	Engine::audioPlayer->playSound("hit.wav");
	Engine::screenShake += dmg * 0.35;
	for (int pi = 0; pi < dmg / 15; pi++)
	{
		((GameScene *)Engine::scenePtr)->spawnEntity(new Particle(x, y, rand() % 400 - 200 + dvx / 2, rand() % 400 - 200 + dvy / 2, ParticleType::PTFeather, &PLR));
	}

	if (dead) return;
	HP -= dmg;
	if (HP <= 0) //KILL
	{
		//DOUBLE VELOCITY AND SCREENSHAKE FOR EFFECT
		vx *= 2;
		vy *= 2;
		Engine::screenShake += dmg * 0.35;
		Engine::audioPlayer->playSound("oof.wav");
		for (int pi = 0; pi < 20; pi++)
		{
			((GameScene *)Engine::scenePtr)->spawnEntity(new Particle(x, y, rand() % 400 - 200 + dvx / 2, rand() % 400 - 200 + dvy / 2, ParticleType::PTFeather, &PLR));
		}
		HP = 0;
		dead = 1;
		Engine::gameData->playerLives[PLR] -= 1;
	}
}

void Bird::consumeConsumable(Consumable * cons) {
	if (cons->ctype == ConsumableType::CTAirstrike)
	{
		addStatusEffect(BirdStatusEffects::BSEAirstrike, 100);
	}
	else if (cons->ctype == ConsumableType::CTEnergyDrink)
	{
		Engine::audioPlayer->playSound("slurp.wav");
		addStatusEffect(BirdStatusEffects::BSEEnergyDrink, 5);
	}
}

void Bird::addStatusEffect(BirdStatusEffects typ, float time) {
	statusEffects[typ] += time;
}
