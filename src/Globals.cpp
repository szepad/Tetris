#include "Globals.h"

void renderShadow(sf::RenderWindow* window, sf::Text& text)
{
	float offset = float(text.getCharacterSize()) * 0.0625f;

	text.move(offset, offset);
	sf::Color savedColor = text.getFillColor();

	window->draw(text);

	text.setFillColor(sf::Color(0, 0, 0, 100));
	window->draw(text);

	text.move(-offset, -offset);
	text.setFillColor(savedColor);
}

void setCenteredOrigin(sf::Text& text)
{
	sf::Vector2f scale = text.getScale();
	text.setScale({1.f, 1.f});
	text.setOrigin(text.getGlobalBounds().width / 2.f, text.getGlobalBounds().height / 2.f);
	text.setScale(scale);
}