#include "AudioPlayer.h"
#include <iostream>

AudioPlayer::AudioPlayer(ResourceLoader * rsloader) {
	resourceLoader = rsloader;
}

void AudioPlayer::initialize() {

}

void AudioPlayer::playSound(std::string name) {
	sf::Sound * sound = new sf::Sound();
	sf::SoundBuffer * buffer = resourceLoader->getSoundBuffer(name);
	if (buffer == NULL)
	{
		std::cout << "WARN: Cannot play sound because of a missing file" << std::endl;
		return;
	}
	sound->setBuffer(*buffer);
	sounds.push_back(sound);
	sound->play();
	//std::cout << "INFO: Playing sound: " << name << std::endl;
}

void AudioPlayer::update() {
	std::list<sf::Sound *>::iterator it = sounds.begin();
	while (it != sounds.end())
	{
		sf::Sound * sound = *it;
		if (sound->getStatus() != sf::Sound::Status::Playing)
		{
			std::cout << "INFO: Removing sound" << std::endl;
			
			delete(sound);
			it = sounds.erase(it);
			continue;
		}
		it++;
	}
}
