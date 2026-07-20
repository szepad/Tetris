#pragma once

#include "Globals.h"
#include "Resources.h"
#include "Screen.h"

class Game
{
private:
	sf::RenderWindow* m_window;
	Screen* m_screen;
	bool m_fullscreen;
	static Resources s_resources;
	static std::vector<sf::Sound> s_sounds;

public:
	Game();
	~Game();
	
	void start();
	void exit();
	static Resources& getResources();
	bool isFullscreen() const;
	void toggleFullscreen();
	bool hasFocus() const;
	static void playSound(const Sound& sound);

private:
	void handleEvents();
	void update(float elapsed);
	void render();
	sf::RenderWindow* createWindowedWindow();
	sf::RenderWindow* createFullscreenWindow();
	void setWindowConfig();
};
