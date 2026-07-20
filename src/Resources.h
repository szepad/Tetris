#pragma once

#include "Globals.h"

enum class Sound
{
	SELECT = 0,
	MOVE,
	PLACE,
	CLEAR
};

struct Resources
{
	sf::Font font;
	sf::Texture tile;
	sf::Music music;
	sf::SoundBuffer sounds[4];

	Resources() = default;
	~Resources() = default;
	
	void load();
};
