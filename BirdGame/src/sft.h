#pragma once

#include <SFML\Graphics.hpp>
namespace sft {
	void fillRect(sf::RenderWindow * window, float x, float y, float w, float h, sf::Color clr);
	void fillText(sf::RenderWindow * window, float x, float y, std::string str, int charSize, sf::Color clr, bool alignCenter);
	void drawSprite(sf::RenderWindow * window, float x, float y, float sizeXY, float scaleX, float scaleY, float rotation, sf::Texture * txtr, sf::IntRect * txtrRect);
	void drawSprite(sf::RenderWindow * window, float x, float y, float sizeXY, float scaleX, float scaleY, float rotation, sf::Sprite sprite);
	sf::Vector2f rotateVectorDegs(float x, float y, float rotDegrees);
}