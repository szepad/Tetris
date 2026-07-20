#pragma once

#include "Globals.h"
#include <functional>

typedef std::pair<sf::Text, std::function<void()>> Option;

class Game;

class OptionMenu
{
private:
	int m_optionIndex;
	std::vector<Option> m_options;
	float m_scale;

public:
	OptionMenu(Game* game, float highlightScale);
	~OptionMenu() = default;

	void addOption(const Option& option);
	void updateCursor(int increment);
	void select();
	void render(sf::RenderWindow* window);
	std::vector<Option>& getOptions();
};

