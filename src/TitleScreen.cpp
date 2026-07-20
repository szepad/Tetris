#include "TitleScreen.h"
#include "GameScreen.h"
#include "Game.h"

TitleScreen::TitleScreen(Game* game) :
	Screen(game),
	m_title(),
	m_timer(0.f),
	m_letterIndex(5),
	m_colorIndex(1),
	m_optionMenu(m_game, 1.1f)
{
	m_optionMenu.addOption({sf::Text("PLAY", Game::getResources().font), [&]()
	{
		finish(new GameScreen(m_game));
	}});
	m_optionMenu.getOptions()[0].first.setPosition(float(SCREEN_WIDTH / 2), 300.f);
	setCenteredOrigin(m_optionMenu.getOptions()[0].first);

	m_optionMenu.addOption({sf::Text("", Game::getResources().font), [&]()
	{
		m_game->toggleFullscreen();
	}});
	setFullscreenButton();

	m_optionMenu.addOption({sf::Text("EXIT", Game::getResources().font), [&]()
	{
		m_game->exit();
	}});
	m_optionMenu.getOptions()[2].first.setPosition(float(SCREEN_WIDTH / 2), 420.f);
	setCenteredOrigin(m_optionMenu.getOptions()[2].first);

	m_title.push_back(sf::Text("T", Game::getResources().font, 100u));
	m_title.push_back(sf::Text("E", Game::getResources().font, 100u));
	m_title.push_back(sf::Text("T", Game::getResources().font, 100u));
	m_title.push_back(sf::Text("R", Game::getResources().font, 100u));
	m_title.push_back(sf::Text("I", Game::getResources().font, 100u));
	m_title.push_back(sf::Text("S", Game::getResources().font, 100u));

	int fullWidth = 0;
	for (size_t i = 0; i < m_title.size(); ++i)
		fullWidth += int(m_title[i].getGlobalBounds().width);

	m_title[0].setPosition(float(SCREEN_WIDTH / 2 - fullWidth / 2 - 15), 125.f);
	m_title[1].setPosition(float(m_title[0].getPosition().x + m_title[0].getGlobalBounds().width + 5), 125.f);
	m_title[2].setPosition(float(m_title[1].getPosition().x + m_title[1].getGlobalBounds().width + 5), 125.f);
	m_title[3].setPosition(float(m_title[2].getPosition().x + m_title[2].getGlobalBounds().width + 5), 125.f);
	m_title[4].setPosition(float(m_title[3].getPosition().x + m_title[3].getGlobalBounds().width + 5), 125.f);
	m_title[5].setPosition(float(m_title[4].getPosition().x + m_title[4].getGlobalBounds().width + 5), 125.f);

	for (size_t i = 0; i < m_title.size(); ++i)
		m_title[i].setFillColor(numToColor.at(7));

	m_optionMenu.updateCursor(0);
}

TitleScreen::~TitleScreen()
{
	
}

void TitleScreen::handleEvents(const sf::Event& evt)
{
	if (evt.type == sf::Event::KeyPressed)
	{
		if (evt.key.code == sf::Keyboard::Escape)
			m_game->exit();

		if (evt.key.code == sf::Keyboard::Enter)
			m_optionMenu.select();

		if (evt.key.code == sf::Keyboard::Up)
		{
			m_optionMenu.updateCursor(-1);
		}

		if (evt.key.code == sf::Keyboard::Down)
		{
			m_optionMenu.updateCursor(1);
		}
	}
}

void TitleScreen::update(float elapsed)
{
	if (m_timer <= 0.f)
	{
		m_timer = COLOR_CHANGE_TIME;

		m_title[m_letterIndex].setScale(1.0f, 1.0f);

		m_letterIndex++;
		if (m_letterIndex >= int(m_title.size()))
		{
			m_letterIndex = 0;

			m_colorIndex++;
			if (m_colorIndex > 7)
				m_colorIndex = 1;
		}

		m_title[m_letterIndex].setFillColor(numToColor.at(m_colorIndex));
		m_title[m_letterIndex].setScale(1.1f, 1.1f);
	}
	else m_timer -= elapsed;

	float scale = 0.1f / COLOR_CHANGE_TIME * elapsed;
	m_title[m_letterIndex].setScale(m_title[m_letterIndex].getScale() - sf::Vector2f(scale, scale));
}

void TitleScreen::render(sf::RenderWindow* window)
{
	for (size_t i = 0; i < m_title.size(); ++i)
	{
		renderShadow(window, m_title[i]);
		window->draw(m_title[i]);
	}

	m_optionMenu.render(window);
}

void TitleScreen::onToggleFullscreen()
{
	setFullscreenButton();
}

void TitleScreen::setFullscreenButton()
{
	m_optionMenu.getOptions()[1].first.setString(m_game->isFullscreen() ? "GO WINDOWED" : "GO FULLSCREEN");
	setCenteredOrigin(m_optionMenu.getOptions()[1].first);
	m_optionMenu.getOptions()[1].first.setPosition(float(SCREEN_WIDTH / 2), 360.f);
}
