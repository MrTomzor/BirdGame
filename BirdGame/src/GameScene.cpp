#include "GameScene.h"
///#include "ResourceLoader.h"
#include "Engine.h"
#include "Projectile.h"

#include <ctgmath>
#include <iostream>
#include <list>

GameScene::GameScene(int scene_id) : Scene(scene_id) {
	
}

void GameScene::initialize() {
	W = 4800 / 2;
	H = 3600 / 2;
	state = GameSceneState::GSSPlaying;
	std::cout << "INFO: Scene created, my ID is " << ID << std::endl;

	Engine::resourceLoader->loadTextures(0);
	Bird::loadTextures(0);
	backgroundTexture = Engine::resourceLoader->getTexture("background_sherwood.png");

	for (int p = 0; p < Engine::gameData->playerCount; p++)
	{
		std::cout << "bird spawned, plr: " << p << std::endl;
		playerBirds[p] = (Bird *)spawnEntity(new Bird((W / (Engine::gameData->playerCount + 1)) * (p + 1), 50, BirdModel::BMPigeon, p));
	}
}

void GameScene::restart() {
	timeAfterLastBird = 0;
	victoryScreenOn = 0;

	std::list<Entity *>::iterator eit = entities.begin();
	while (eit != entities.end())
	{
		delete(*eit);
		eit = entities.erase(eit);
		continue;
	}

	for (int p = 0; p < Engine::gameData->playerCount; p++)
	{
		std::cout << "INFO: Bird respawned, plr: " << p << std::endl;
		playerBirds[p] = (Bird *)spawnEntity(new Bird((W / (Engine::gameData->playerCount + 1)) * (p + 1), 50, BirdModel::BMPigeon, p));
		Engine::gameData->playerLives[p] = Engine::gameData->maxPlayerLives;
	}
	
	Engine::timeMod = 1;
	state = GameSceneState::GSSPlaying;
	std::cout << "INFO: Restarted" << std::endl;
}


void GameScene::update(){
	if (restartRequest)
	{
		restart();
		restartRequest = 0;
	}
	//TODO if(exitRequest)

	handleInput();
	handlePhysics();
	handleCollisions();
	handleRules();
	camFollow();
}

void GameScene::renderUI() {
	renderHPBars();
	if (victoryScreenOn)
		renderVictoryScreen();
	
}

void GameScene::renderHPBars() {
	GameData gd = *Engine::gameData;
	float distX = Engine::screenW / (gd.playerCount + 1);
	for (int i = 0; i < gd.playerCount; i++)
	{
		float hpBarWidth = 180;
		float hpBarHeight = 30;
		float outlineThickness = 4;
		float hpBarPadding = 20;
		float relHP = playerBirds[i]->HP / playerBirds[i]->MAXHP;

		sf::RectangleShape MAXHPshape(sf::Vector2f(hpBarWidth + outlineThickness * 2, hpBarHeight + outlineThickness * 2));
		sf::RectangleShape HPshape(sf::Vector2f(relHP * hpBarWidth, hpBarHeight));
		MAXHPshape.setFillColor(sf::Color(50, 50, 50));
		HPshape.setFillColor(sf::Color(200, 0, 0));

		MAXHPshape.move(((i + 1) * distX - hpBarWidth / 2 - outlineThickness), (hpBarPadding - outlineThickness));
		HPshape.move(((i + 1) * distX - hpBarWidth / 2), hpBarPadding);

		window->draw(MAXHPshape);
		window->draw(HPshape);

		float hatDist = 20;
		float hatTextureScale = 2;
		for (int k = 0; k < Engine::gameData->playerLives[i]; k++)
		{
			sft::drawSprite(Engine::windowPtr, (i + 1) * distX - hpBarWidth / 2 + hatDist * k, hpBarPadding * 2 + hpBarHeight, Bird::defHatTextureWidth, hatTextureScale, hatTextureScale, 45,
				Bird::hatsTexture, new sf::IntRect(i * Bird::defHatTextureWidth, 0, Bird::defHatTextureWidth, Bird::defHatTextureWidth));
		}
	}
}

void GameScene::renderVictoryScreen() {
	float w = Engine::screenW * 0.85;
	float h = Engine::screenH * 0.85;
	float borderWidth = 10;

	sft::fillRect(Engine::windowPtr, (Engine::screenW - w) / 2 - borderWidth, (Engine::screenH - h) / 2 - borderWidth, w + 2 * borderWidth, h + 2 * borderWidth, sf::Color(0, 0, 0));
	sft::fillRect(Engine::windowPtr, (Engine::screenW - w) / 2, (Engine::screenH - h) / 2, w, h, sf::Color(255,255,255));
	

	float dx = w / (1 + Engine::gameData->playerCount);
	float pillarWidth = 70;
	float pillarHeight = 70;
	float birdOffset = 70;
	for (int p = 0; p < Engine::gameData->playerCount; p++)
	{
		sft::fillRect(Engine::windowPtr, (Engine::screenW - w) / 2 + (p + 1) * dx - pillarWidth, h + (Engine::screenH - h) / 2 - (Engine::gameData->playerPoints[p] + 0.0) * pillarHeight, pillarWidth * 2, (Engine::gameData->playerPoints[p] + 0.0) * pillarHeight, sf::Color::Red);
		float ox = playerBirds[p]->x;
		float oy = playerBirds[p]->y;

		playerBirds[p]->x = (Engine::screenW - w) / 2 + (p + 1) * dx;
		playerBirds[p]->y = h + (Engine::screenH - h) / 2 - (Engine::gameData->playerPoints[p] + 0.0) * pillarHeight - birdOffset;
		playerBirds[p]->render(NULL);

		playerBirds[p]->x = ox;
		playerBirds[p]->y = oy;

		sft::fillText(window, (Engine::screenW - w) / 2 + (p + 1) * dx, (Engine::screenH - h) / 2 + 100, std::to_string(Engine::gameData->playerPoints[p]), 80, sf::Color::Black, 1);
	}
	sft::fillText(window, Engine::screenW / 2, (Engine::screenH - h) / 2 + 40, "PLAYER #" + std::to_string(winnerBird + 1) + " WINS!    Press ENTER to play again!", 40, sf::Color::Red, 1);
}

void GameScene::render(){
	window->clear(sf::Color(100, 100, 200));
	sf::Sprite backgroundSprite;
	backgroundSprite.setTexture(*backgroundTexture);
	backgroundSprite.scale(2, 2);
	(*window).draw(backgroundSprite);
	sf::RenderStates * states = new sf::RenderStates();
	for (std::list<Entity *>::iterator it = entities.begin(); it != entities.end(); it++)
	{
		(*it)->render(states);
	}
	window->setView(window->getDefaultView());
	renderUI();
	window->setView(*Engine::camera);
	(*window).display();
	
	
}


void GameScene::handleInput() {
	if (state == GameSceneState::GSSPaused)
	{

	}
	else if (state == GameSceneState::GSSVictory)
	{
		if (Engine::inputData.masterConfirmPressed) restartRequest = 1;
	}
	else
	{
		for (int p = 0; p < Engine::gameData->playerCount; p++)
		{
			int inputSource = Engine::gameData->playerInputSource[p];
			if (Engine::inputData.flapPressed[inputSource])
			{
				playerBirds[p]->flap();
			}
			if (Engine::inputData.slapPressed[inputSource])
			{
				playerBirds[p]->slap();
			}
			if (Engine::inputData.shootHeld[inputSource])
			{
				playerBirds[p]->shoot();
			}
			playerBirds[p]->moveLR(Engine::inputData.axisX[inputSource]);
		}
	}
}

void GameScene::handlePhysics() {
	std::list<Entity *>::iterator eit = entities.begin(); //update and divide to sectors
	while (eit != entities.end())
	{
		if ((*eit)->y + (*eit)->radius >= H - Hborder && (*eit)->vy >= 0)
		{
			(*eit)->vy = 0;
			(*eit)->y = H - (*eit)->radius - Hborder;

			(*eit)->grounded = 1;
			
		}
		else if ((*eit)->y < (*eit)->radius && (*eit)->vy < 0)
		{
			(*eit)->y = (*eit)->radius;
			(*eit)->vy = -(*eit)->vy * 0.6;
		}
		else
		{
			if((*eit)->useGravity)
				(*eit)->vy += gravity * Engine::dt;
			(*eit)->y += (*eit)->vy * Engine::dt;
			(*eit)->grounded = 0;
		}

		(*eit)->x += (*eit)->vx * Engine::dt;
		if((*eit)->x < Wborder)
		{
			(*eit)->x = Wborder;
			(*eit)->vx = -(*eit)->vx * 0.6;
		}
		else if ((*eit)->x > W - Wborder)
		{
			(*eit)->x = W - Wborder;
			(*eit)->vx = -(*eit)->vx * 0.6;
		}

		eit++;
	}
}

void GameScene::handleCollisions() {
	std::list<Entity *>::iterator eit = entities.begin(); //update and divide to sectors
	while (eit != entities.end())
	{
		if ((*eit)->type == EntityType::ETProjectile)
		{
			Projectile * proj = (Projectile *)(*eit);
			if (proj->armed)
			{
				for (int pl = 0; pl < Engine::gameData->playerCount; pl++)
				{

					if (pl != proj->PLR)
					{
						if (proj->distTo(playerBirds[pl]) <= proj->radius + playerBirds[pl]->radius) //HIT
						{
							playerBirds[pl]->recDamage(proj->DMG, proj->vx, proj->vy);
							playerBirds[pl]->addVelocity(proj->vx / playerBirds[pl]->mass, proj->vy / playerBirds[pl]->mass);

							proj->deleteFlag = 1;
							break;
						}
					}
				}
				/*if (proj->y > H || hitSomeBird)
				{
					delete(proj);
					eit = entities.erase(eit);
					continue;
				}*/
			}
		}
		else if ((*eit)->type == EntityType::ETConsumable)
		{
			Consumable * cons = (Consumable*)(*eit);
			if (cons->pickupable)
			{
				for (int pl = 0; pl < Engine::gameData->playerCount; pl++)
				{
					if (cons->distTo(playerBirds[pl]) <= cons->radius + playerBirds[pl]->radius) //HIT
					{
						playerBirds[pl]->consumeConsumable(cons);

						cons->deleteFlag = 1;
						break;
					}
				}
			}
			
		}

		if ((*eit)->deleteFlag)
		{
			delete(*eit);
			eit = entities.erase(eit);
			continue;
		}
		(*eit)->update();
		eit++;
	}
}

void GameScene::handleRules() {
	if (state == GameSceneState::GSSPlaying)
	{
		//CONSUMABLE SPAWNING
		consumableSpawnTime += Engine::dt;
		if (consumableSpawnTime > consumableSpawnTimeMax)
		{
			consumableSpawnTime -= consumableSpawnTimeMax;
			spawnEntity(new Consumable(rand() % W, rand() % H, (ConsumableType)(rand() % CONSUMABLE_TYPE_COUNT)));
		}

		//END GAME CHECK
		int totallyDeadBirds = 0;
		for (int p = 0; p < Engine::gameData->playerCount; p++)
		{
			if (playerBirds[p]->dead && playerBirds[p]->deadTime > 2)
			{
				if (Engine::gameData->playerLives[p] > 0)
				{
					playerBirds[p]->dead = false;
					playerBirds[p]->vy = -gravity * 2;
					playerBirds[p]->vx = rand() % 1600 - 800;
					playerBirds[p]->HP = playerBirds[p]->MAXHP;
					playerBirds[p]->deadTime = 0;
				}
				else
				{
					totallyDeadBirds++;
				}
			}
		}
		if (totallyDeadBirds == Engine::gameData->playerCount - 1) // END OF GAME
		{
			state = GameSceneState::GSSVictory;
			Engine::audioPlayer->playSound("thug_life_short.wav");
			//Find winner bird player
			winnerBird = -1;
			for (int p = 0; p < Engine::gameData->playerCount; p++)
			{
				if (Engine::gameData->playerLives[p] > 0)
				{
					winnerBird = p;
				}
			}
			Engine::gameData->playerPoints[winnerBird]++;
		}

	}
	else if (state == GameSceneState::GSSVictory)
	{
		if (!victoryScreenOn)
		{
			timeAfterLastBird += Engine::udt;
			if (timeAfterLastBird >= timeAfterLastBirdMax)
				victoryScreenOn = 1;
			else
				Engine::timeMod = 0.3 - (timeAfterLastBird / timeAfterLastBirdMax) * 0.3;
		}
		else
		{
			Engine::timeMod = 0;
		}
	}
}

void GameScene::handleUI() {
	switch (state)
	{
	case GSSPlaying:
		break;
	case GSSPaused:
		break;
	case GSSVictory:
		break;
	default:
		break;
	}
}

void GameScene::camFollow() {
	float cx = 0, cy = 0;
	float maxYdist = 0;
	float maxXdist = 0;

	int plrCount = Engine::gameData->playerCount;
	int livingBirdCount = 0;
	for (int i = 0; i < plrCount; i++)
	{
		if (Engine::gameData->playerLives[i] == 0) continue;
		livingBirdCount++;
		cx += playerBirds[i]->x;
		cy += playerBirds[i]->y;
		for (int j = i + 1; j < plrCount; j++)
		{
			float dx = abs(playerBirds[i]->x - playerBirds[j]->x);
			if (dx > maxXdist) maxXdist = dx;

			float dy = abs(playerBirds[i]->y - playerBirds[j]->y);
			if (dy > maxYdist) maxYdist = dy;
		}
	}
	cx /= livingBirdCount;
	cy /= livingBirdCount;
	float oldcx = cx;
	float oldcy = cy;
	float wScale = maxXdist / Engine::screenW;
	float hScale = maxYdist / Engine::screenH;
	float maxScale = wScale > hScale ? wScale : hScale;

	
	float newZoom;
	if (maxScale >= 0.5) newZoom = maxScale + 0.5;
	else newZoom = 1;

	if (Engine::screenH * newZoom > H)
	{
		newZoom = H / Engine::screenH;
	}
	if (Engine::screenW * newZoom > W)
	{
		newZoom = W / Engine::screenW;
	}

	float ex = cx + Engine::screenW * newZoom / 2;
	if (ex > W) cx -= ex - W;
	ex = cx - Engine::screenW * newZoom / 2;
	if (ex < 0) cx += -ex;

	ex = cy + Engine::screenH * newZoom / 2;
	if (ex > H) cy -= ex - H;
	ex = cy - Engine::screenH * newZoom / 2;
	if (ex < 0) cy += -ex;

	
	Engine::setCameraCenter(cx, cy);
	Engine::setCameraZoom(newZoom);
}

Entity * GameScene::spawnEntity(Entity * ent) {
	entities.push_back(ent);
	return ent;
}

std::list<Entity*> GameScene::getEntsCircle(float xx, float yy, float r) {
	std::list<Entity*> ret;
	//std::cout << "debug: " << entities.begin() << std::endl;
	std::list<Entity *>::iterator eit = entities.begin();
	int num = 0;
	
	while (eit != entities.end())
	{
		float dx = (*eit)->x - xx;
		float dy = (*eit)->y - yy;
		float mindist = r + (*eit)->radius;
		if (dx * dx + dy * dy <= mindist * mindist)
		{
			ret.push_back(*eit); num++;
		}
		eit++;
	}
	return ret;
}
