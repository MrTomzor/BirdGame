#pragma once
#include <list>
#include <SFML/Audio.hpp>
#include "ResourceLoader.h"

class AudioPlayer {
public:
	ResourceLoader * resourceLoader;
	std::list<sf::Sound *> sounds;

	AudioPlayer(ResourceLoader * rsloader);

	void initialize();
	void playSound(std::string name);
	void update();
};