#pragma once
#include "Scene.h"
#include <list>
#include <stdlib.h>
#include "Bird.h"

enum GameSceneState {
	GSSPlaying,
	GSSPaused,
	GSSVictory
};

class GameScene : public Scene {
public:
	int W;
	int H;
	GameSceneState state;
	float Wborder = 10;
	float Hborder = 10;
	int sW, sH;
	float camSpeed = 0.05;
	float gravity = 1000;
	bool camFollowPlayer = 1;
	std::list<Entity *> entities;
	Bird * playerBirds[6];
	sf::Texture * backgroundTexture;

	//VICTORY SCREEN
	float timeAfterLastBird = 0;
	float timeAfterLastBirdMax = 2;
	bool victoryScreenOn = false;
	int winnerBird = -1;
	
	//REQUESTS
	bool restartRequest = 0;
	bool exitRequest = 0;

	//CONSUMABLE SPAWNING
	float consumableSpawnTime = 0;
	float consumableSpawnTimeMax = 3;

	GameScene(int scene_id);
	void initialize();
	void restart();

	void update();
	void renderUI();
	void renderHPBars();
	void renderVictoryScreen();
	void render() override;
	void handleInput();
	void handlePhysics();
	void handleCollisions();
	void handleRules();
	void handleUI();
	void camFollow();
	Entity * spawnEntity(Entity * ent);
	std::list<Entity *> getEntsCircle(float xx, float yy, float r);
};