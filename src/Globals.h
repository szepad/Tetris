#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <unordered_map>
#include "Utils.h"

typedef unsigned int uint;

static const char* TITLE = "Tetris";

constexpr int SCREEN_WIDTH = 1280;
constexpr int SCREEN_HEIGHT = 720;

constexpr float CELL_SIZE = 32;
constexpr int ROWS = 20;
constexpr int COLS = 10;

constexpr float BOARD_WIDTH = CELL_SIZE * float(COLS);// + float(COLS - 1);
constexpr float BOARD_HEIGHT = CELL_SIZE * float(ROWS);// + float(ROWS - 1);
constexpr float BOARD_X = float(SCREEN_WIDTH) / 2.f - BOARD_WIDTH / 2.f;
constexpr float BOARD_Y = float(SCREEN_HEIGHT) / 2.f - BOARD_HEIGHT / 2.f;

const std::unordered_map<int, sf::Color> numToColor = {
	{0, sf::Color::Black},
	{1, sf::Color::Cyan},
	{2, sf::Color::Yellow},
	{3, sf::Color::Blue},
	{4, sf::Color(255, 165, 0, 255)},
	{5, sf::Color::Green},
	{6, sf::Color::Red},
	{7, sf::Color(148, 0, 211)},
};

void renderShadow(sf::RenderWindow* window, sf::Text& text);
void setCenteredOrigin(sf::Text& text);