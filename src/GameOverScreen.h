#pragma once

#include "Screen.h"
#include "OptionMenu.h"

class Game;

class GameOverScreen : public Screen
{
private:
	uint m_score;
	uint m_lines;
	std::vector<sf::Text> m_texts;
	OptionMenu m_optionMenu;

public:
	GameOverScreen(Game* game, uint score, uint lines);
	~GameOverScreen() = default;

	void handleEvents(const sf::Event& evt) override;
	void update(float elapsed) override;
	void render(sf::RenderWindow* window) override;
};

