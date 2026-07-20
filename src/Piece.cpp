#include "Piece.h"
#include "Game.h"

std::string Piece::templates[7] = {
	"0000"
	"1111"
	"0000"
	"0000",

	"0000"
	"0220"
	"0220"
	"0000",

	"300"
	"333"
	"000",

	"004"
	"444"
	"000",

	"055"
	"550"
	"000",

	"660"
	"066"
	"000",

	"070"
	"777"
	"000"
};

Piece::Piece(Game* game, int x, int y) :
	m_game(game),
	m_x(x),
	m_y(y)
{
	m_rand = Utils::getRand(0, 6);
	setup();
}

Piece::Piece(Game* game, int x, int y, int rand) :
	m_game(game),
	m_x(x),
	m_y(y),
	m_rand(rand)
{
	setup();
}

Piece::~Piece()
{
	delete m_matrix;
}

bool Piece::moveAndCollide(int dx, int dy, Matrix* collisionMatrix)
{
	m_x += dx;
	m_y += dy;

	if (checkCollision(collisionMatrix))
	{
		m_x -= dx;
		m_y -= dy;
		return false;
	}

	return true;
}

bool Piece::checkCollision(Matrix* collisionMatrix)
{
	for (size_t i = 0; i < m_size; ++i)
		for (size_t j = 0; j < m_size; ++j)
		{
			int current = m_matrix->at(i, j);

			if (current == 0)
				continue;

			if (collisionMatrix->at(m_y + i, m_x + j) != 0)
			{
				return true;
			}
		}

	return false;
}

int Piece::getX() const
{
	return m_x;
}

void Piece::setX(int val)
{
	m_x = val;
}

int Piece::getY() const
{
	return m_y;
}

void Piece::setY(int val)
{
	m_y = val;
}

unsigned int Piece::getSize() const
{
	return m_size;
}

Matrix* Piece::getMatrix() const
{
	return m_matrix;
}

int Piece::getRandTetromino() const
{
	return m_rand;
}

void Piece::setup()
{
	if (m_rand <= 1)
		m_size = 4;
	else m_size = 3;

	m_matrix = new Matrix(m_size, m_size);

	for (size_t i = 0; i < m_size; ++i)
		for (size_t j = 0; j < m_size; ++j)
		{
			m_matrix->set(i, j, std::stoi(std::string(1, templates[m_rand].at(i * m_size + j))));
		}
}
