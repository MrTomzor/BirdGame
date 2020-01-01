#include "sft.h"
#include "Engine.h"

void sft::fillRect(sf::RenderWindow * window, float x, float y, float w, float h, sf::Color clr) {
	sf::RectangleShape shape(sf::Vector2f(w, h));
	shape.move(x, y);
	shape.setFillColor(clr);
	window->draw(shape);
}

void sft::fillText(sf::RenderWindow * window, float x, float y, std::string str, int charSize, sf::Color clr, bool alignCenter) {
	sf::Text txt;
	txt.setString(str);
	txt.setFont(*Engine::resourceLoader->getFont(0));
	txt.setCharacterSize(charSize);
	txt.setFillColor(clr);
	if (alignCenter)
	{
		sf::FloatRect bounds = txt.getLocalBounds();
		txt.move(x - bounds.width / 2, y - bounds.height * 0.75);
	}
	else
	{
		txt.move(x, y);
	}
	window->draw(txt);
	
}

void sft::drawSprite(sf::RenderWindow * window, float x, float y, float size, float scaleX, float scaleY, float rotation, sf::Texture * txtr, sf::IntRect * txtrRect) {
	sf::Sprite sprite;
	sprite.setTexture(*txtr);
	if (txtrRect != NULL)
	{
		sprite.setTextureRect(*txtrRect);
	}
	sprite.setOrigin(sf::Vector2f(size / 2, size / 2));
	drawSprite(window, x, y, size, scaleX, scaleY, rotation, sprite);
}

void sft::drawSprite(sf::RenderWindow * window, float x, float y, float size, float scaleX, float scaleY, float rotation, sf::Sprite sprite) {
	sprite.rotate(rotation);
	sprite.scale(scaleX, scaleY);
	sprite.move(x, y);
	Engine::windowPtr->draw(sprite);
}

sf::Vector2f sft::rotateVectorDegs(float x, float y, float rotDegrees) {
	float rotRads = 3.14145926 * rotDegrees / 180;
	float rotSin = sin(rotRads);
	float rotCos = cos(rotRads);

	float nx = x * rotCos + y * rotSin;
	float ny = x * (-rotSin) + y * rotCos;
	return sf::Vector2f(nx, ny);
}
