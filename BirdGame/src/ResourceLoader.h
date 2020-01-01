#pragma once
#include <SFML\Graphics.hpp>
#include <SFML\Audio.hpp>
#define TEXTURE_COUNT 7
#define MEDIA_COUNT 7

class ResourceLoader {
public:
	int maxTextures = 100;
	sf::Texture * textures[TEXTURE_COUNT];
	std::string textureNames[TEXTURE_COUNT] = {"pigeon_standing.png", "pigeon_flapping.png" , "pigeon_flying.png" ,"pigeon_slapping.png", "background_sherwood.png",
	"hats.png", "consumables.png"};
	bool texturesLoaded[TEXTURE_COUNT];

	sf::SoundBuffer * soundBuffers[MEDIA_COUNT];
	std::string soundBuffersNames[MEDIA_COUNT] = {"hit.wav", "sht_sound.wav", "wing_flap.wav", "oof.wav", "thug_life_short.wav", "airstrike.wav",
	"slurp.wav"};
	bool soundBuffersLoaded[MEDIA_COUNT];

	sf::Font * fonts[5];
	std::string resRoot;

	ResourceLoader();
	void initialize();
	void initNames();
	sf::Texture * loadTexture(std::string name, bool * success);
	sf::SoundBuffer * loadSoundBuffer(std::string name, bool * success);
	void loadTextures(int arg);
	void loadSoundBuffers(int arg);
	void loadFonts(int arg);

	sf::Texture * getTexture(int index);
	sf::Texture * getTexture(std::string name);
	sf::SoundBuffer * getSoundBuffer(int index);
	sf::SoundBuffer * getSoundBuffer(std::string name);
	sf::Font * getFont(int index);
	
};