#include "GameData.h"

GameData::GameData() {
	playerCount = 0;
	mapID = MapType::Sherwood;
	for (int p = 0; p < 6; p++)
	{
		playerReady[p] = 0;
		birdSelected[p] = BirdModel::BMPigeon;
		playerLives[p] = maxPlayerLives;
		playerPoints[p] = 0;
	}
}

void GameData::addPlayer(int inputSource) {
	playerInputSource[playerCount] = inputSource;
	playerCount++;
}

int GameData::getOwnerOfSource(int src) {
	for (int k = 0; k < playerCount; k++)
	{
		if (playerInputSource[k] == src)
		{
			return k;
		}
	}
	return -1;
}
