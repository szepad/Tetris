#include "Game.h"
#include "TitleScreen.h"
#include "GameScreen.h"
#include "GameOverScreen.h"

Resources Game::s_resources{};
std::vector<sf::Sound> Game::s_sounds = {};

Game::Game() :
	m_window(),
	m_screen(nullptr)
{
	m_window = createWindowedWindow();
	setWindowConfig();
}

Game::~Game()
{
	if (m_screen != nullptr)
		delete m_screen;

	delete m_window;
}

void Game::start()
{
	s_resources.load();
	m_screen = new TitleScreen(this);

	sf::Clock clock;
	float elapsed = 0.f;
	while (m_window->isOpen())
	{
		elapsed = clock.restart().asSeconds();
		handleEvents();
		update(elapsed);
		render();
	}
}

void Game::exit()
{
	m_window->close();
}

Resources& Game::getResources()
{
	return s_resources;
}

bool Game::isFullscreen() const
{
	return m_fullscreen;
}

void Game::toggleFullscreen()
{
	delete m_window;

	if (m_fullscreen)
		m_window = createWindowedWindow();
	else m_window = createFullscreenWindow();

	setWindowConfig();

	m_fullscreen = !m_fullscreen;

	m_screen->onToggleFullscreen();
}

bool Game::hasFocus() const
{
	return m_window->hasFocus();
}

void Game::playSound(const Sound& sound)
{
	s_sounds.push_back(sf::Sound(s_resources.sounds[static_cast<int>(sound)]));
	s_sounds.back().play();
}

void Game::handleEvents()
{
	sf::Event evt;
	while (m_window->pollEvent(evt))
	{
		if (evt.type == sf::Event::Closed)
			m_window->close();

		if (evt.type == sf::Event::KeyPressed)
		{
			if (evt.key.code == sf::Keyboard::F11)
				toggleFullscreen();
		}

		m_screen->handleEvents(evt);
	}
}

void Game::update(float elapsed)
{
	for (size_t i = 0; i < s_sounds.size(); ++i)
	{
		if (s_sounds[i].getStatus() == sf::Sound::Status::Stopped)
		{
			s_sounds.erase(s_sounds.begin() + i);
			i--;
		}
	}

	m_screen->update(elapsed);

	if (m_screen->getFinished())
	{
		if (TitleScreen* ts = dynamic_cast<TitleScreen*>(m_screen))
		{
			m_screen = ts->getNextScreen();
			delete ts;
		}
		else if (GameScreen* gs = dynamic_cast<GameScreen*>(m_screen))
		{
			m_screen = gs->getNextScreen();
			delete gs;
		}
		else if (GameOverScreen* gos = dynamic_cast<GameOverScreen*>(m_screen))
		{
			m_screen = gos->getNextScreen();
			delete gos;
		}
	}
}

void Game::render()
{
	m_window->clear(sf::Color::Black);

	m_screen->render(m_window);

	m_window->display();
}

sf::RenderWindow* Game::createWindowedWindow()
{
	return new sf::RenderWindow(sf::VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), TITLE, sf::Style::Default);
}

sf::RenderWindow* Game::createFullscreenWindow()
{
	return new sf::RenderWindow(sf::VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), TITLE, sf::Style::Fullscreen);
}

void Game::setWindowConfig()
{
	m_window->setFramerateLimit(144);
	m_window->setVerticalSyncEnabled(true);
	m_window->setKeyRepeatEnabled(false);
	m_window->setMouseCursorVisible(false);
}
