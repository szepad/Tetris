#include "OptionMenu.h"
#include "Game.h"

OptionMenu::OptionMenu(Game* game, float highlightScale) :
	//m_cursorText(">", game->getResources()->font),
	m_optionIndex(0),
	m_options(),
	m_scale(highlightScale)
{
	//m_cursorText.setFillColor(sf::Color::Yellow);
	//m_cursorText.setOrigin(m_cursorText.getGlobalBounds().width, 0.f);
}

void OptionMenu::addOption(const Option& option)
{
	m_options.push_back(option);
}

void OptionMenu::updateCursor(int increment)
{
	m_options[m_optionIndex].first.setScale(1.0f, 1.0f);
	m_options[m_optionIndex].first.setFillColor(sf::Color::White);

	m_optionIndex += increment;

	if (m_optionIndex < 0)
		m_optionIndex = m_options.size() - 1;
	else if (m_optionIndex >= int(m_options.size()))
		m_optionIndex = 0;

	m_options[m_optionIndex].first.setScale(m_scale, m_scale);
	m_options[m_optionIndex].first.setFillColor(sf::Color::Yellow);
	//m_cursorText.setPosition(m_options[m_optionIndex].first.getPosition().x - float(int(m_options[m_optionIndex].first.getGlobalBounds().width) / 2) - 15.f, m_options[m_optionIndex].first.getPosition().y - 10.f);
	
	if (increment != 0)
		Game::playSound(Sound::SELECT);
}

void OptionMenu::select()
{
	m_options[m_optionIndex].second();
}

void OptionMenu::render(sf::RenderWindow* window)
{
	//renderShadow(window, m_cursorText);
	//window->draw(m_cursorText);

	for (size_t i = 0; i < m_options.size(); ++i)
	{
		renderShadow(window, m_options[i].first);
		window->draw(m_options[i].first);
	}
}

std::vector<Option>& OptionMenu::getOptions()
{
	return m_options;
}
