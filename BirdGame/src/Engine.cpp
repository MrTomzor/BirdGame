#include "Engine.h"
#include "GameScene.h"
#include "MenuScene.h"
#include <iostream>

#include <time.h>
#include <ctime>
#include <chrono>
#include <thread>

InputData Engine::inputData; // creates space for the var
GameData * Engine::gameData = NULL;
ResourceLoader * Engine::resourceLoader = NULL;
AudioPlayer * Engine::audioPlayer = NULL;
float Engine::dt;
float Engine::udt;
float Engine::timeMod = 1;
sf::RenderWindow * Engine::windowPtr = NULL;
sf::View *Engine::camera;
Scene * Engine::scenePtr = NULL;
bool Engine::running = 1;

float Engine::camX = 0;
float Engine::camY = 0;
float Engine::camScale = 1;
float Engine::screenShake = 0;
float Engine::screenShakeDamping = 100;
int Engine::screenW = 1200;
int Engine::screenH = 900;
int Engine::sceneChangeRequest = 0;
int Engine::sceneChangeRequestArg = 0;

void Engine::startup() {
	running = 1;
	sf::ContextSettings windowSettings;
	windowSettings.antialiasingLevel = 8;
	sf::RenderWindow window(sf::VideoMode(screenW, screenH), "Birdame", sf::Style::Default, windowSettings);
	windowPtr = &window;
	camera = new sf::View(sf::FloatRect(0, 0, screenW, screenH));
	window.setView(*camera);

	resourceLoader = new ResourceLoader();
	resourceLoader->initialize();

	audioPlayer = new AudioPlayer(resourceLoader);
	audioPlayer->initialize();
	
	//Generate first scene, will be menu afterwards
	/*gameData = new GameData();
	gameData->addPlayer(InputSource::Keyboard1);
	gameData->addPlayer(InputSource::Keyboard2);

	scene = new GameScene(1);
	scene->window = &window;
	scene->parentEngine = this;
	scene->initialize();*/

	//OPEN MENU
	scene = new MenuScene(2);
	scene->window = &window;
	scene->initialize();


	scenePtr = scene;

	//Main game loop
	clock_t startTime = clock();
	clock_t endTime = startTime;
	int FPS = 40;
	double frameSeconds = 1.0 / FPS;
	double timer = 0;
	while (running)
	{
		endTime = clock();

		getInput();
		update(dt);
		render();

		startTime = endTime;
		endTime = clock();
		dt = (float)(endTime - startTime) / CLOCKS_PER_SEC;

		if (dt < frameSeconds)
		{
			std::this_thread::sleep_for(std::chrono::milliseconds((int)((frameSeconds - dt) * 1000)));
			timer += frameSeconds;
			dt = frameSeconds;
		}
		else
		{
			timer += dt;
			std::cout << "Cant keep up " << std::endl;
		}
		udt = dt;
		dt *= timeMod;
		//std::cout << "Timer :" << timer << std::endl;
	}
	std::cout << "Broken out of main loop" << std::endl;
	std::cin.get();
}

void Engine::close() {
	running = 0;
}

void Engine::update(float dt) {

	if (Engine::sceneChangeRequest != 0)
	{
		if (Engine::sceneChangeRequest == 1) //TO GAME
		{
			delete(scene);
			scene = new GameScene(1);
			scenePtr = scene;
			scene->window = windowPtr;
			scene->parentEngine = this;
			scene->initialize();
		}
		else if (Engine::sceneChangeRequest == 2) //TO RESULTS
		{
			
		}
		else if (Engine::sceneChangeRequest == 3) //REPLAY
		{ 

		}
		else if (Engine::sceneChangeRequest == 4) //BACK TO MENU
		{
			delete(scene);
			scene = new MenuScene(2);
			scene->window = windowPtr;
			scenePtr = scene;
			scene->parentEngine = this;
			scene->initialize();
		}
		sceneChangeRequest = 0;
		std::cout << "INFO: Changed Scene " << std::endl;
	}
	scene->update();
	audioPlayer->update();
	//SCREENSHAKE after scene update, cuz scene sets the view as well
	if (screenShake > 0)
	{
		int shakeIntVal = (int)screenShake;
		if (shakeIntVal == 0) shakeIntVal = 1;
		camera->setCenter(sf::Vector2f(camX + (rand() % (shakeIntVal * 2)) - shakeIntVal, camY + (rand() % (shakeIntVal * 2)) - shakeIntVal));

		windowPtr->setView(*camera);
		screenShake -= Engine::udt * screenShakeDamping;
	}
}

void Engine::render(){
	scene->render();
}

void Engine::getInput() {
	sf::Event ev;

	//clear "pressed" input data
	for (int pl = 0; pl < 6; pl++)
	{
		Engine::inputData.flapPressed[pl] = 0;
		Engine::inputData.slapPressed[pl] = 0;
		Engine::inputData.upPressed[pl] = 0;
		Engine::inputData.downPressed[pl] = 0;
		Engine::inputData.leftPressed[pl] = 0;
		Engine::inputData.rightPressed[pl] = 0;
		Engine::inputData.confirmPressed[pl] = 0;
		Engine::inputData.backPressed[pl] = 0;
		Engine::inputData.masterConfirmPressed = 0;
		Engine::inputData.masterBackPressed = 0;
	}

	//calculate X axis for keyboard inputs
	Engine::inputData.axisX[InputSource::Keyboard1] = (int)Engine::inputData.rightHeld[InputSource::Keyboard1] - (int)Engine::inputData.leftHeld[InputSource::Keyboard1];
	Engine::inputData.axisX[InputSource::Keyboard2] = (int)Engine::inputData.rightHeld[InputSource::Keyboard2] - (int)Engine::inputData.leftHeld[InputSource::Keyboard2];

	//ask for joystick buttons pressed
	for (int c = 0; c < 4; c++)
	{
		if (!sf::Joystick::isConnected(c))
			continue;
		//A
		if (sf::Joystick::isButtonPressed(c, 0))
		{
			Engine::inputData.upHeld[c] = 1;
		}
		else
		{
			if (Engine::inputData.upHeld[c])
			{
				Engine::inputData.flapPressed[c] = 1;
			}
			Engine::inputData.upHeld[c] = 0;
		}

		//B
		if (sf::Joystick::isButtonPressed(c, 1))
		{
			Engine::inputData.shootHeld[c] = 1;
		}
		else
		{
			if (Engine::inputData.shootHeld[c])
			{
				Engine::inputData.backPressed[c] = 1;
			}
			Engine::inputData.shootHeld[c] = 0;
		}

		//X
		if (sf::Joystick::isButtonPressed(c, 2))
		{
			Engine::inputData.slapHeld[c] = 1;
		}
		else
		{
			if (Engine::inputData.slapHeld[c])
			{
				Engine::inputData.slapPressed[c] = 1;
				Engine::inputData.confirmPressed[c] = 1;
			}
			Engine::inputData.slapHeld[c] = 0;
		}

		//Horizontal axis
		float axisX = sf::Joystick::getAxisPosition(c, sf::Joystick::X);
		float axisY = sf::Joystick::getAxisPosition(c, sf::Joystick::Y);
		Engine::inputData.axisX[c] = axisX / 100;

		//up and down press
		Engine::inputData.upHeld[c] = axisY > 90 ? 1 : 0;
		Engine::inputData.downHeld[c] = axisY < -90 ? 1 : 0;
		if(!Engine::inputData.upHeld[c] && !Engine::inputData.upHeld[c])
		{
			if(Engine::inputData.upHeld[c]) Engine::inputData.upPressed[c] = 1;
			else if(Engine::inputData.downHeld[c]) Engine::inputData.downPressed[c] = 1;
		}

		//left and right press
		Engine::inputData.leftHeld[c] = axisX <= 0 ? 1 : 0;
		Engine::inputData.rightHeld[c] = axisX >= 0 ? 1 : 0;
		if(!Engine::inputData.leftHeld[c] && !Engine::inputData.rightHeld[c])
		{
			if (Engine::inputData.leftHeld[c]) Engine::inputData.leftPressed[c] = 1;
			else if (Engine::inputData.rightHeld[c]) Engine::inputData.rightPressed[c] = 1;
		}
	}
	
	//poll keyboard and other events
	while (windowPtr->pollEvent(ev))
	{
		switch (ev.type)
		{
		case sf::Event::Closed:
			std::cout << "Closed!" << std::endl;
			windowPtr->close();
			running = 0;
			break;
		case sf::Event::KeyPressed:
			switch (ev.key.code)
			{
				//KEYBOARD 1
			case sf::Keyboard::W:
				Engine::inputData.upHeld[InputSource::Keyboard1] = 1;
				break;
			case sf::Keyboard::S:
				Engine::inputData.downHeld[InputSource::Keyboard1] = 1;
				break;
			case sf::Keyboard::A:
				Engine::inputData.leftHeld[InputSource::Keyboard1] = 1;
				break;
			case sf::Keyboard::D:
				Engine::inputData.rightHeld[InputSource::Keyboard1] = 1;
				break;
			case sf::Keyboard::K:
				Engine::inputData.shootHeld[InputSource::Keyboard1] = 1;
				break;
				//KEYBOARD 2
			case sf::Keyboard::Up:
				Engine::inputData.upHeld[InputSource::Keyboard2] = 1;
				break;
			case sf::Keyboard::Down:
				Engine::inputData.downHeld[InputSource::Keyboard2] = 1;
				break;
			case sf::Keyboard::Left:
				Engine::inputData.leftHeld[InputSource::Keyboard2] = 1;
				break;
			case sf::Keyboard::Right:
				Engine::inputData.rightHeld[InputSource::Keyboard2] = 1;
				break;
			case sf::Keyboard::Numpad2:
				Engine::inputData.shootHeld[InputSource::Keyboard2] = 1;
				break;
			}
			break;
		case sf::Event::KeyReleased:
			switch (ev.key.code)
			{
			case sf::Keyboard::W:
				Engine::inputData.upHeld[InputSource::Keyboard1] = 0;
				Engine::inputData.flapPressed[InputSource::Keyboard1] = 1;
				Engine::inputData.upPressed[InputSource::Keyboard1] = 1;
				break;
			case sf::Keyboard::S:
				Engine::inputData.downHeld[InputSource::Keyboard1] = 0;
				Engine::inputData.downPressed[InputSource::Keyboard1] = 1;
				break;
			case sf::Keyboard::A:
				Engine::inputData.leftHeld[InputSource::Keyboard1] = 0;
				Engine::inputData.leftPressed[InputSource::Keyboard1] = 1;
				break;
			case sf::Keyboard::D:
				Engine::inputData.rightHeld[InputSource::Keyboard1] = 0;
				Engine::inputData.rightPressed[InputSource::Keyboard1] = 1;
				break;
			case sf::Keyboard::J:
				Engine::inputData.slapPressed[InputSource::Keyboard1] = 1;
				Engine::inputData.confirmPressed[InputSource::Keyboard1] = 1;
				break;
			case sf::Keyboard::K:
				Engine::inputData.shootHeld[InputSource::Keyboard1] = 0;
				Engine::inputData.backPressed[InputSource::Keyboard1] = 1;
				break;
				//special
			case sf::Keyboard::Enter:
				Engine::inputData.masterConfirmPressed = 1;
				break;
			case sf::Keyboard::BackSpace:
				Engine::inputData.masterBackPressed = 1;
				break;
				//KEYBOARD 2
			case sf::Keyboard::Up:
				Engine::inputData.upHeld[InputSource::Keyboard2] = 0;
				Engine::inputData.flapPressed[InputSource::Keyboard2] = 1;
				Engine::inputData.upPressed[InputSource::Keyboard2] = 1;
				break;
			case sf::Keyboard::Down:
				Engine::inputData.downHeld[InputSource::Keyboard2] = 0;
				Engine::inputData.downPressed[InputSource::Keyboard2] = 1;
				break;
			case sf::Keyboard::Left:
				Engine::inputData.leftHeld[InputSource::Keyboard2] = 0;
				Engine::inputData.leftPressed[InputSource::Keyboard2] = 1;
				break;
			case sf::Keyboard::Right:
				Engine::inputData.rightHeld[InputSource::Keyboard2] = 0;
				Engine::inputData.rightPressed[InputSource::Keyboard2] = 1;
				break;
			case sf::Keyboard::Numpad1:
				Engine::inputData.slapPressed[InputSource::Keyboard2] = 1;
				Engine::inputData.confirmPressed[InputSource::Keyboard2] = 1;
				break;
			case sf::Keyboard::Numpad2:
				Engine::inputData.shootHeld[InputSource::Keyboard2] = 0;
				Engine::inputData.backPressed[InputSource::Keyboard2] = 1;
				break;
			}
			
		}
	}
}

void Engine::setCameraCenter(float xx, float yy) {
	camX = xx;
	camY = yy;
	camera->setCenter(camX, camY);
	windowPtr->setView(*camera);
}

void Engine::setCameraZoom(float z) {
	camera->setSize(screenW * z, screenH * z);
	camScale = z;
	windowPtr->setView(*camera);
}

void Engine::resetCamera() {
	camera->setCenter(screenW / 2, screenH / 2);
	camera->setSize(screenW, screenH );
	camScale = 1;
	camX = screenW / 2;
	camY = screenH / 2;
	windowPtr->setView(*camera);
}

void Engine::entityEventRequest(int type, void * data) {
	scenePtr->handleEvent(type, data);
}

Engine::Engine(void **args) {
	//scenePtr = &scene;
	inputData;
}