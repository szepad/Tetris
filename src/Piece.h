#pragma once

#include <string>
#include "Globals.h"
#include "Matrix.h"

class Game;

class Piece
{
private:
	Game* m_game;
	int m_x, m_y;
	unsigned int m_size;
	Matrix* m_matrix;
	int m_rand;

	static std::string templates[7];

public:
	Piece(Game* game, int x, int y);
	Piece(Game* game, int x, int y, int rand);
	~Piece();

	bool moveAndCollide(int dx, int dy, Matrix* collisionMatrix);
	bool checkCollision(Matrix* collisionMatrix);
	int getX() const;
	void setX(int val);
	int getY() const;
	void setY(int val);
	unsigned int getSize() const;
	Matrix* getMatrix() const;
	int getRandTetromino() const;

private:
	void setup();
};
