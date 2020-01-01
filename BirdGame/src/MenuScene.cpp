#include "MenuScene.h"
#include "sft.h"
#include "Engine.h"
#include "ResourceLoader.h"
#include <iostream>

MenuScene::MenuScene(int id) : Scene(id){

}

void MenuScene::initialize() {
	Engine::resetCamera();
	mainScreenSelected = 0;
	mapScreenSelected = 0;
	currentScreen = MenuScreen::MainMenuScreen;

	MenuScene::renderLoadingScreen();
	Engine::resourceLoader->loadTextures(0);
	Engine::resourceLoader->loadSoundBuffers(0);
	Bird::loadTextures(0);
	Consumable::loadTextures(0);
}

void MenuScene::update() {
	handleInput();
	if (currentScreen == MenuScreen::BirdSelectScreen)
	{
		bool allReady = Engine::gameData->playerCount > 1;
		for (int p = 0; p < Engine::gameData->playerCount; p++)
		{
			if (!Engine::gameData->playerReady[p])
			{
				allReady = 0;
				break;
			}
		}
		if (allReady) Engine::sceneChangeRequest = 1;
	}
}

void MenuScene::renderUI() {
}

void MenuScene::render() {
	window->clear(sf::Color(100, 100, 200));
	sf::RenderStates * states = new sf::RenderStates();
	int selectedOutline = 12;
	int normalOutline = 5;
	switch (currentScreen)
	{
	case MenuScreen::MainMenuScreen:
		drawTextBox("ARENA", Engine::screenW / 2, Engine::screenH / (mainMenuOptions + 1), 400, 100, mainScreenSelected == 0 ? selectedOutline : normalOutline);
		drawTextBox("DOMINATION", Engine::screenW / 2, 2 * Engine::screenH / (mainMenuOptions + 1), 400, 100, mainScreenSelected == 1 ? selectedOutline : normalOutline);
		drawTextBox("SETTINGS", Engine::screenW / 2, 3 * Engine::screenH / (mainMenuOptions + 1), 400, 100, mainScreenSelected == 2 ? selectedOutline : normalOutline);
		drawTextBox("QUIT", Engine::screenW / 2, 4 * Engine::screenH / (mainMenuOptions + 1), 400, 100, mainScreenSelected == 3 ? selectedOutline : normalOutline);
		break;
	case MenuScreen::MapSelectScreen:
		drawTextBox("DEATHMATCH", Engine::screenW / 2, Engine::screenH / (mapSelectOptions + 1), 600, 100, mapScreenSelected == 0 ? selectedOutline : normalOutline);
		drawTextBox("MAP: SHERWOOD", Engine::screenW / 2, 2 * Engine::screenH / (mapSelectOptions + 1), 600, 100, mapScreenSelected == 1 ? selectedOutline : normalOutline);
		drawTextBox("PLAY", Engine::screenW / 2, 3 * Engine::screenH / (mapSelectOptions + 1), 400, 100, mapScreenSelected == 2 ? selectedOutline : normalOutline);
		break;
	case MenuScreen::BirdSelectScreen:
		for (int plr = 0; plr < Engine::gameData->playerCount; plr++)
		{
			float ww = Engine::screenW / Engine::gameData->playerCount;
			drawPlayerSetup(plr * ww, 0, ww, Engine::screenH, plr);
		}
		break;
	}
	(*window).display();
}

void MenuScene::handleInput() {
	bool screenChange = 0;
	switch (currentScreen)
	{
	case MenuScreen::MainMenuScreen:
		for (int i = 0; i < 6 ; i++)
		{
			if (Engine::inputData.upPressed[i])
				mainScreenSelected = (mainScreenSelected - 1 + mainMenuOptions) % mainMenuOptions;
			else if (Engine::inputData.downPressed[i])
				mainScreenSelected = (mainScreenSelected + 1 + mainMenuOptions) % mainMenuOptions;
			else if (Engine::inputData.confirmPressed[i] || Engine::inputData.masterConfirmPressed)
			{
				switch (mainScreenSelected)
				{
				case 0:
					if (Engine::gameData != NULL) delete(Engine::gameData);
					Engine::gameData = new GameData();
					currentScreen = MenuScreen::MapSelectScreen;
					mapScreenSelected = 2;
					break;
				case 1:
					//TODO
					break;
				case 2:
					//TODO
					break;
				case 3:
					Engine::windowPtr->close();
					Engine::running = 0;
					break;
				}
				break;
			}
		}
		break;
	case MenuScreen::MapSelectScreen:
		for (int i = 0; i < 6; i++)
		{
			if (Engine::inputData.upPressed[i])
				mapScreenSelected = (mapScreenSelected - 1 + mapSelectOptions) % mapSelectOptions;
			else if (Engine::inputData.downPressed[i])
				mapScreenSelected = (mapScreenSelected + 1 + mapSelectOptions) % mapSelectOptions;
			else if(Engine::inputData.backPressed[i] || Engine::inputData.masterBackPressed)
				currentScreen = MenuScreen::MainMenuScreen;
			else if (Engine::inputData.confirmPressed[i] || Engine::inputData.masterConfirmPressed)
			{
				switch (mapScreenSelected)
				{
				case 0:
					//TODO change lives
					break;
				case 1:
					//TODO change map
					break;
				case 2:
					currentScreen = MenuScreen::BirdSelectScreen;
					break;
				}
				break;
			}
		}
		break;
	case MenuScreen::BirdSelectScreen:
		GameData * gd = Engine::gameData;
		int addedPlrs = 0;
		for (int s = InputSource::Joystick1; s <= InputSource::Keyboard2; s++)
		{
			if (Engine::inputData.upPressed[s] || Engine::inputData.downPressed[s] || Engine::inputData.leftPressed[s] || Engine::inputData.rightPressed[s] 
				|| Engine::inputData.slapPressed[s] || Engine::inputData.flapPressed[s])
			{
				int ownerOfInputSource = gd->getOwnerOfSource(s);
				if (ownerOfInputSource == -1) //someone unconnected has pressed UP
				{
					displayBirds[gd->playerCount] = new Bird(0, 0, BirdModel::BMPigeon, gd->playerCount);
					gd->addPlayer(s);
					
					std::cout << "added plr" << std::endl;
					addedPlrs = 1;
					continue;
				}
			}
		}
		for (int p = 0; p < gd->playerCount; p++)
		{
			if (Engine::inputData.confirmPressed[gd->playerInputSource[p]])
			{
				if (gd->playerReady[p])
				{
					std::cout << "Even more ready" << std::endl;
				}
				else
				{
					gd->playerReady[p] = 1;
				}
			}
			else if (Engine::inputData.backPressed[gd->playerInputSource[p]])
			{
				gd->playerReady[p] = 0;
			}
			//TODO bird select
		}
		break;

	}
}

void MenuScene::drawTextBox(sf::String str, float x, float y, float w, float h, float olw) {
	

	sft::fillRect(window, x - w / 2 - olw, y - h / 2 - olw, w + olw * 2, h + olw * 2, sf::Color(0, 0, 0));
	sft::fillRect(window, x - w / 2, y - h / 2, w, h, sf::Color(255, 255, 255));
	sft::fillText(window, x, y, str, 32, sf::Color(0, 0, 0), true);
	//window->draw(txt);
}

void MenuScene::drawPlayerSetup(float x, float y, float w, float h, int plr) {
	float topOffset = 40;
	float botOffset = 40;
	sft::fillText(window, x + w/2, y + topOffset, "PLAYER #" + std::to_string(plr), 32, sf::Color(0, 0, 0), true);
	if (Engine::gameData->playerReady[plr])
	{
		sft::fillText(window, x + w / 2, h - botOffset, "READY" , 40, sf::Color(0, 50, 0), true);
	}
	else
	{
		sft::fillText(window, x + w / 2, h - botOffset, "PRESS ATK", 20, sf::Color(40, 40, 40), true);
	}
	sf::RenderStates states(sf::RenderStates::Default);
	//states.transform.scale(sf::Vector2f(2, 2), sf::Vector2f(Bird::defTextureWidth / 2, Bird::defTextureWidth / 2));
	displayBirds[plr]->x = x + w / 2;
	displayBirds[plr]->y = y + h / 2;
	displayBirds[plr]->scale = 2;
	displayBirds[plr]->render(NULL);
}

void MenuScene::renderLoadingScreen() {
	Engine::windowPtr->clear(sf::Color(255, 255, 255));
	sft::fillText(Engine::windowPtr, Engine::screenW / 2, Engine::screenH / 2, "This is a loading screen. Please wait.", 40, sf::Color::Black, 1);
	Engine::windowPtr->display();
}
