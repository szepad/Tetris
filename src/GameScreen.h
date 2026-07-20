#pragma once

#include "Screen.h"

class Game;
class Matrix;
class Piece;

class GameScreen : public Screen
{
private:
	Matrix* m_matrix;
	float m_tickTime;
	float m_tickTimer;
	const float MOVE_LOCK_TIME = 0.25f;
	const float MOVE_TIME = 0.1f;
	float m_moveTimer;
	Piece* m_currentPiece;
	Piece* m_ghost;
	Piece* m_nextPiece;
	uint m_score;
	uint m_lines;
	sf::Text m_scoreText;
	sf::Text m_lineText;
	sf::Text m_nextText;
	bool m_paused;
	sf::Text m_pausedText;
	std::vector<int> m_rowsToClear;
	const float ROW_CLEAR_ANIMATION_TIME = 0.5f;
	float m_rowClearAnimTimer;

public:
	GameScreen(Game* game);
	~GameScreen();

	void handleEvents(const sf::Event& evt) override;
	void update(float elapsed) override;
	void render(sf::RenderWindow* window) override;
	
private:
	void createGameObjects();
	void freeGameObjects();

	void tick();
	void move(int dx, int dy);
	void applyPiece();
	void generateNewPiece();
	void checkRows();
	void clearRow(int row);
	void rotateMatrixCW(Matrix* matrix);
	void rotateMatrixCCW(Matrix* matrix);
	void restart();
	void updateGhost();
	void updateUI();

	void renderBoard(sf::RenderWindow* window);
	void renderTiles(sf::RenderWindow* window);
	void renderGrid(sf::RenderWindow* window);
	void renderUI(sf::RenderWindow* window);
	void renderPieces(sf::RenderWindow* window);
	void renderLineClearAnim(sf::RenderWindow* window);

public:
	uint getScore() const;
	uint getLines() const;
};

