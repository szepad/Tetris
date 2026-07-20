#include "Resources.h"

static bool s_loaded = false;

void Resources::load()
{
	if (s_loaded)
		return;

	font.loadFromFile("res/nesfont.otf");
	tile.loadFromFile("res/tile.png");
	music.openFromFile("res/music.ogg");
	sounds[0].loadFromFile("res/select.wav");
	sounds[1].loadFromFile("res/move.wav");
	sounds[2].loadFromFile("res/place.wav");
	sounds[3].loadFromFile("res/clear.wav");

	s_loaded = true;
}
