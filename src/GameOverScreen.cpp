#include "GameOverScreen.h"
#include "GameScreen.h"
#include "TitleScreen.h"
#include "Game.h"

GameOverScreen::GameOverScreen(Game* game, uint score, uint lines) :
	Screen(game),
	m_score(score),
	m_lines(lines),
	m_texts(),
	m_optionMenu(m_game, 1.05f)
{
	m_texts.push_back(sf::Text("GAME OVER", Game::getResources().font, 80u));
	m_texts[0].setPosition(float(SCREEN_WIDTH / 2), 150.f);
	m_texts[0].setFillColor(sf::Color(175, 0, 0));
	setCenteredOrigin(m_texts[0]);

	m_texts.push_back(sf::Text("SCORE: " + std::to_string(score), Game::getResources().font));
	m_texts[1].setPosition(float(SCREEN_WIDTH / 2) + 100.f, 300.f - float(int(m_texts[1].getGlobalBounds().height) / 2));

	m_texts.push_back(sf::Text("LINES: " + std::to_string(lines), Game::getResources().font));
	m_texts[2].setPosition(float(SCREEN_WIDTH / 2) + 100.f, 350.f - float(int(m_texts[1].getGlobalBounds().height) / 2));

	float optionX = float(SCREEN_WIDTH / 2) - 150.f;

	m_optionMenu.addOption({sf::Text("PLAY AGAIN", Game::getResources().font), [&]()
	{
		finish(new GameScreen(m_game));
	}});
	m_optionMenu.getOptions()[0].first.setPosition(optionX, 300.f);
	setCenteredOrigin(m_optionMenu.getOptions()[0].first);

	m_optionMenu.addOption({sf::Text("QUIT TO TITLE\nSCREEN", Game::getResources().font), [&]()
	{
		finish(new TitleScreen(m_game));
	}});
	m_optionMenu.getOptions()[1].first.setPosition(optionX + float((m_optionMenu.getOptions()[1].first.getGlobalBounds().width - m_optionMenu.getOptions()[0].first.getGlobalBounds().width) / 2.f), 375.f);
	setCenteredOrigin(m_optionMenu.getOptions()[1].first);

	m_optionMenu.updateCursor(0);
}

void GameOverScreen::handleEvents(const sf::Event& evt)
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

void GameOverScreen::update(float elapsed)
{
}

void GameOverScreen::render(sf::RenderWindow* window)
{
	for (size_t i = 0; i < m_texts.size(); ++i)
	{
		renderShadow(window, m_texts[i]);
		window->draw(m_texts[i]);
	}

	m_optionMenu.render(window);
}
