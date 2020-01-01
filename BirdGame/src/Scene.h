#pragma once
#include <SFML\Graphics.hpp>

extern class Engine;

class Scene {
public:
	int ID;
	Scene();
	Scene(int scene_id);
	virtual void update();
	virtual void render();
	virtual void handleEvent(int type, void * data);

	virtual void initialize();
	Engine *parentEngine;
	sf::RenderWindow *window;

};