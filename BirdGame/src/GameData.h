#pragma once

#include "Bird.h"

enum MapType {
	Sherwood
};

enum InputSource {
	Joystick1,
	Joystick2,
	Joystick3,
	Joystick4,
	Keyboard1,
	Keyboard2
};

class GameData {
public:
	MapType mapID;
	int playerCount;
	int maxPlayerLives = 1;
	int playerInputSource[4];
	BirdModel birdSelected[6];
	bool playerReady[6];
	int playerLives[6];
	int playerPoints[6];

	GameData();
	void addPlayer(int inputSource);
	int getOwnerOfSource(int src);
};