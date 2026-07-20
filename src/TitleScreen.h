#pragma once

#include "Screen.h"
#include "OptionMenu.h"

class Game;

class TitleScreen : public Screen
{
private:
	std::vector<sf::Text> m_title;
	float m_timer;
	const float COLOR_CHANGE_TIME = 0.25f;
	int m_letterIndex;
	int m_colorIndex;
	OptionMenu m_optionMenu;

public:
	TitleScreen(Game* game);
	~TitleScreen();

	void handleEvents(const sf::Event& evt) override;
	void update(float elapsed) override;
	void render(sf::RenderWindow* window) override;
	void onToggleFullscreen() override;

private:
	void setFullscreenButton();
};
