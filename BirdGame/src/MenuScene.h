#pragma once
#include "Scene.h"
#include "GameData.h"
#include <SFML\Graphics.hpp>

enum MenuScreen {
	MainMenuScreen,
	MapSelectScreen,
	BirdSelectScreen
};

class MenuScene : public Scene {
public:
	MenuScreen currentScreen;
	int mainScreenSelected;
	int mapScreenSelected;

	int mainMenuOptions = 4;
	int mapSelectOptions = 3;

	Bird * displayBirds[6];
	
	sf::Font menuFont;

	MenuScene(int id);

	void initialize();
	void update();
	void renderUI();
	void render() override;
	void handleInput();

	void drawTextBox(sf::String str, float x, float y, float w, float h, float olw);
	void drawPlayerSetup(float x, float y, float w, float h, int plr);

	static void renderLoadingScreen();
};