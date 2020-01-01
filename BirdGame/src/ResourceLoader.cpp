#include "ResourceLoader.h"
#include <iostream>

ResourceLoader::ResourceLoader() {
}

void ResourceLoader::initialize() {
	resRoot = "C:/Users/tommy/source/repos/BirdGame/BirdGame/res/";
	loadFonts(0);
	for (int i = 0; i < TEXTURE_COUNT; i++)
	{
		texturesLoaded[i] = 0;
	}
	for (int i = 0; i < MEDIA_COUNT; i++)
	{
		soundBuffersLoaded[i] = 0;
	}
}

void ResourceLoader::initNames() {
	//textureNames[0] = "bird";
}

sf::Texture * ResourceLoader::loadTexture(std::string name, bool * success) {
	sf::Texture * res = new sf::Texture();
	*success = res->loadFromFile(ResourceLoader::resRoot + "textures/" + name);
	if(*success) std::cout << "INFO: Texture loaded succesfully: " << name << std::endl;
	else std::cout << "ERROR: Cannot load texture: " << name << std::endl;
	return res;
}

sf::SoundBuffer * ResourceLoader::loadSoundBuffer(std::string name, bool * success) {
	sf::SoundBuffer * res = new sf::SoundBuffer();
	*success = res->loadFromFile(ResourceLoader::resRoot + "media/" + name);
	if (*success) std::cout << "INFO: SoundBuffer loaded succesfully: " << name << std::endl;
	else std::cout << "ERROR: Cannot load SoundBuffer: " << name << std::endl;
	return res;
}

void ResourceLoader::loadTextures(int arg) {
	if (arg == 0) //load all
	{
		//load all textures and each class has to getTexture from here and if it wasnt loaded, it will just load it itself
		for (int i = 0; i < TEXTURE_COUNT; i++)
		{
			if (texturesLoaded[i]) continue;
			bool success = 0;
			textures[i] = loadTexture(textureNames[i], &success);
			if (success)
			{
				texturesLoaded[i] = 1;
			}
			else
			{
				texturesLoaded[i] = 0;
			}
		}
	}
}

void ResourceLoader::loadSoundBuffers(int arg) {
	if (arg == 0) //load all
	{
		//load all textures and each class has to getTexture from here and if it wasnt loaded, it will just load it itself
		for (int i = 0; i < MEDIA_COUNT; i++)
		{
			if (soundBuffersLoaded[i]) continue;
			bool success = 0;
			soundBuffers[i] = loadSoundBuffer(soundBuffersNames[i], &success);
			if (success)
			{
				soundBuffersLoaded[i] = 1;
			}
			else
			{
				soundBuffersLoaded[i] = 0;
			}
		}
	}
}

void ResourceLoader::loadFonts(int arg) {
	sf::Font * font1 = new sf::Font();
	font1->loadFromFile(ResourceLoader::resRoot + "fonts/GiantRobotArmy-Medium.ttf");
	fonts[0] = font1;
}

sf::Texture * ResourceLoader::getTexture(int index) {
	return textures[index];
}

sf::Texture * ResourceLoader::getTexture(std::string name) {
	for (int i = 0; i < TEXTURE_COUNT; i++)
	{
		if (textureNames[i].compare(name) == 0)
		{
			if (texturesLoaded[i])
			{
				std::cout << "INFO: Get texture succesful: " << name << std::endl;
				return textures[i];
			}
			else
			{
				std::cout << "INFO: Texture not loaded, attempting to load: " << name << std::endl;
				bool succ = 0;
				sf::Texture * res = loadTexture(name, &succ);
				if (succ) return res;
				return NULL;
			}
		}
	}
	std::cout << "ERROR: Texture not found by name: " << name << std::endl;
	return NULL;
}

sf::SoundBuffer * ResourceLoader::getSoundBuffer(int index) {
	return soundBuffers[index];
}

sf::SoundBuffer * ResourceLoader::getSoundBuffer(std::string name) {
	for (int i = 0; i < MEDIA_COUNT; i++)
	{
		if (soundBuffersNames[i].compare(name) == 0)
		{
			if (soundBuffersLoaded[i])
			{
				std::cout << "INFO: Get SoundBuffer succesful: " << name << std::endl;
				return soundBuffers[i];
			}
			else
			{
				std::cout << "INFO: SoundBuffer not loaded, attempting to load: " << name << std::endl;
				bool succ = 0;
				sf::SoundBuffer * res = loadSoundBuffer(name, &succ);
				if (succ) return res;
				return NULL;
			}
		}
	}
	std::cout << "ERROR: SoundBuffer not found by name: " << name << std::endl;
	return NULL;
}

sf::Font * ResourceLoader::getFont(int index) {
	return fonts[index];
}
