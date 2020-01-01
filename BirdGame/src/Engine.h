#pragma once
#include "Scene.h"
#include "InputData.h"
#include "GameData.h"
#include "ResourceLoader.h"
#include "AudioPlayer.h"
#include <SFML\Graphics.hpp>

#include "sft.h"

class Engine{
public:
	Engine(void **args);

	void startup();
	void close();
	void update(float dt);
	void getInput();
	void render();
	
	static float dt;
	static float udt; //u stands for ultimate - is not affected by timeMod
	static float timeMod;
	static bool running;
	Scene *scene;
	static Scene *scenePtr;
	int playerID;
	static InputData inputData;
	static GameData * gameData;
	static ResourceLoader * resourceLoader;
	static AudioPlayer * audioPlayer;

	static sf::RenderWindow *windowPtr;
	static sf::View *camera;
	static void setCameraCenter(float xx, float yy);
	static void setCameraZoom(float z);

	static void resetCamera();
	static void entityEventRequest(int type, void * data);
	static float camX;
	static float camY;
	static float camScale;
	static float screenShake;
	static float screenShakeDamping;
	static int screenW;
	static int screenH;
	static int sceneChangeRequest;
	static int sceneChangeRequestArg;
	


};