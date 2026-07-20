#include "Screen.h"

Screen::Screen(Game* game) :
	m_finished(false),
	m_game(game),
	m_nextScreen()
{
}

Screen::~Screen()
{

}

void Screen::finish(Screen* nextScreen)
{
	m_finished = true;
	m_nextScreen = nextScreen;
}

bool Screen::getFinished() const
{
	return m_finished;
}

Screen* Screen::getNextScreen()
{
	return m_nextScreen;
}


