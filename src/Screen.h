#pragma once

#include "Globals.h"

class Game;

class Screen
{
private:
	bool m_finished;

protected:
	Game* m_game;
	Screen* m_nextScreen;

public:
	Screen(Game* game);
	virtual ~Screen() = 0;

	virtual void handleEvents(const sf::Event& evt) = 0;
	virtual void update(float elapsed) = 0;
	virtual void render(sf::RenderWindow* window) = 0;
	virtual void onToggleFullscreen() {}

	void finish(Screen* nextScreen);
	bool getFinished() const;
	Screen* getNextScreen();
};