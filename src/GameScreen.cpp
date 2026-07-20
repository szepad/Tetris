#include "GameScreen.h"
#include "TitleScreen.h"
#include "GameOverScreen.h"
#include "Game.h"
#include "Matrix.h"
#include "Piece.h"

GameScreen::GameScreen(Game* game) :
	Screen(game),
	m_matrix(nullptr),
	m_tickTime(0.8f),
	m_tickTimer(0.f),
	m_moveTimer(0.f),
	m_currentPiece(nullptr),
	m_nextPiece(nullptr),
	m_ghost(nullptr),
	m_score(0),
	m_lines(0),
	m_scoreText("", Game::getResources().font),
	m_lineText("", Game::getResources().font),
	m_nextText("NEXT:", Game::getResources().font),
	m_paused(false),
	m_pausedText("PAUSED", Game::getResources().font, 100u),
	m_rowsToClear(),
	m_rowClearAnimTimer()
{
	m_scoreText.setPosition(float(SCREEN_WIDTH / 5) * 3.5f, 150.f);
	m_lineText.setPosition(float(SCREEN_WIDTH / 5) * 3.5f, 200.f);
	m_nextText.setPosition(int(float(SCREEN_WIDTH / 5) * 3.5f), 250.f);
	m_pausedText.setPosition(float(SCREEN_WIDTH / 2), floorf(float(SCREEN_HEIGHT) / 2.5f));
	setCenteredOrigin(m_pausedText);

	createGameObjects();

	Game::getResources().music.play();
	Game::getResources().music.setLoop(true);
}

GameScreen::~GameScreen()
{
	freeGameObjects();
	Game::getResources().music.stop();
}

void GameScreen::handleEvents(const sf::Event& evt)
{
	if (evt.type == sf::Event::KeyPressed)
	{
		if (evt.key.code == sf::Keyboard::Escape)
		{
			if (!m_paused)
			{
				m_paused = true;
				Game::getResources().music.pause();
			}
			else finish(new TitleScreen(m_game));
		}

		if (evt.key.code == sf::Keyboard::Enter)
		{
			m_paused = false;
			Game::getResources().music.play();
		}

		if (!m_paused && m_rowClearAnimTimer <= 0.f)
		{
			if (evt.key.code == sf::Keyboard::Left)
			{
				move(-1, 0);
				m_moveTimer = MOVE_LOCK_TIME;
			}

			if (evt.key.code == sf::Keyboard::Right)
			{
				move(1, 0);
				m_moveTimer = MOVE_LOCK_TIME;
			}

			if (evt.key.code == sf::Keyboard::Up)
			{
				rotateMatrixCW(m_currentPiece->getMatrix());
				rotateMatrixCW(m_ghost->getMatrix());
				if (m_currentPiece->checkCollision(m_matrix))
				{
					rotateMatrixCCW(m_currentPiece->getMatrix());
					rotateMatrixCCW(m_ghost->getMatrix());
				}

				updateGhost();
			}

			if (evt.key.code == sf::Keyboard::Space)
			{
				while (m_currentPiece->moveAndCollide(0, 1, m_matrix));
				applyPiece();
			}
		}
	}
}

void GameScreen::update(float elapsed)
{
	if (m_paused)
		return;

	updateUI();

	m_rowClearAnimTimer -= elapsed;

	if (m_rowClearAnimTimer > 0.f)
		return;

	if (m_rowsToClear.size() != 0u)
	{
		for (size_t i = 0; i < m_rowsToClear.size(); ++i)
		{
			// + i because when the row is cleared the whole board moves down one cell
			clearRow(m_rowsToClear[i] + i);
		}

		m_rowsToClear.clear();
	}

	m_moveTimer -= elapsed;

	if (m_game->hasFocus())
	{
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
		{
			if (m_moveTimer <= 0.f)
			{
				move(-1, 0);
				m_moveTimer = MOVE_TIME;
			}
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
		{
			if (m_moveTimer <= 0.f)
			{
				move(1, 0);
				m_moveTimer = MOVE_TIME;
			}
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
		{
			if (m_moveTimer <= 0.f)
			{
				move(0, 1);
				m_moveTimer = MOVE_TIME;
			}
		}
	}

	if (m_tickTimer <= 0.f)
	{
		tick();
		m_tickTimer = m_tickTime;
	}
	else m_tickTimer -= elapsed;
}

void GameScreen::render(sf::RenderWindow* window)
{
	renderBoard(window);
	renderGrid(window);
	renderTiles(window);
	renderPieces(window);
	renderLineClearAnim(window);
	renderUI(window);
}

void GameScreen::createGameObjects()
{
	m_matrix = new Matrix(ROWS, COLS);
	m_currentPiece = new Piece(m_game, COLS / 2 - 2, 0);
	m_ghost = new Piece(m_game, m_currentPiece->getX(), m_currentPiece->getY(), m_currentPiece->getRandTetromino());
	m_nextPiece = new Piece(m_game, 0, 0);
}

void GameScreen::freeGameObjects()
{
	delete m_matrix;
	delete m_currentPiece;
	delete m_ghost;
	delete m_nextPiece;
}

void GameScreen::tick()
{
	if (!m_currentPiece->moveAndCollide(0, 1, m_matrix))
		applyPiece();

	updateGhost();
}

void GameScreen::move(int dx, int dy)
{
	if (m_currentPiece->moveAndCollide(dx, dy, m_matrix))
		Game::playSound(Sound::MOVE);
	
	updateGhost();
}

void GameScreen::applyPiece()
{
	Matrix* pm = m_currentPiece->getMatrix();
	int px = m_currentPiece->getX();
	int py = m_currentPiece->getY();

	for (size_t i = 0; i < pm->getRows(); ++i)
		for (size_t j = 0; j < pm->getCols(); ++j)
		{
			int current = pm->at(i, j);

			if (current == 0)
				continue;

			m_matrix->set(py + i, px + j, current);
		}

	Game::playSound(Sound::PLACE);

	checkRows();
	generateNewPiece();

	if (m_currentPiece->checkCollision(m_matrix))
		finish(new GameOverScreen(m_game, m_score, m_lines));
}

void GameScreen::generateNewPiece()
{
	delete m_currentPiece;
	delete m_ghost;
	m_currentPiece = m_nextPiece;
	m_currentPiece->setX(COLS / 2 - 2);
	m_nextPiece = new Piece(m_game, 0, 0);
	m_ghost = new Piece(m_game, m_currentPiece->getX(), m_currentPiece->getY(), m_currentPiece->getRandTetromino());
	updateGhost();
}

void GameScreen::checkRows()
{
	int linesCleared = 0;
	for (int row = int(m_matrix->getRows() - 1); row >= 0; --row)
	{
		bool toClear = true;

		for (size_t col = 0; col < m_matrix->getCols(); ++col)
		{
			if (m_matrix->at(row, col) == 0)
			{
				toClear = false;
				break;
			}
		}

		if (toClear)
		{
			m_rowsToClear.push_back(row);
			linesCleared++;
			m_rowClearAnimTimer = ROW_CLEAR_ANIMATION_TIME;
		}
	}

	if (linesCleared != 0)
		Game::playSound(Sound::CLEAR);

	m_lines += linesCleared;
	switch (linesCleared)
	{
		case 1: m_score += 40; break;
		case 2: m_score += 100; break;
		case 3: m_score += 300; break;
		case 4: m_score += 1200; break;
	}
}

void GameScreen::clearRow(int row)
{
	for (size_t col = 0; col < m_matrix->getCols(); ++col)
		for (int moveRow = row; moveRow >= 0; --moveRow)
		{
			int above = m_matrix->at(moveRow - 1, col);
			m_matrix->set(moveRow, col, above == -1 ? 0 : above);
		}
}

void GameScreen::rotateMatrixCW(Matrix* matrix)
{
	Matrix temp(matrix->getRows(), matrix->getCols());

	for (size_t i = 0; i < matrix->getRows(); ++i)
		for (size_t j = 0; j < matrix->getCols(); ++j)
		{
			temp.set(j, matrix->getRows() - 1 - i, matrix->at(i, j));
		}

	for (size_t i = 0; i < matrix->getRows(); ++i)
		for (size_t j = 0; j < matrix->getCols(); ++j)
		{
			matrix->set(i, j, temp.at(i, j));
		}
}

void GameScreen::rotateMatrixCCW(Matrix* matrix)
{
	Matrix temp(matrix->getRows(), matrix->getCols());

	for (size_t i = 0; i < matrix->getRows(); ++i)
		for (size_t j = 0; j < matrix->getCols(); ++j)
		{
			temp.set(matrix->getRows() - 1 - j, i, matrix->at(i, j));
		}

	for (size_t i = 0; i < matrix->getRows(); ++i)
		for (size_t j = 0; j < matrix->getCols(); ++j)
		{
			matrix->set(i, j, temp.at(i, j));
		}
}

void GameScreen::restart()
{
	Game::getResources().music.play();
	m_score = 0;
	m_lines = 0;
	freeGameObjects();
	createGameObjects();
}

void GameScreen::updateGhost()
{
	m_ghost->setX(m_currentPiece->getX());
	m_ghost->setY(m_currentPiece->getY());

	while (m_ghost->moveAndCollide(0, 1, m_matrix));
}

void GameScreen::updateUI()
{
	m_scoreText.setString("SCORE: " + std::to_string(m_score));
	m_lineText.setString("LINES: " + std::to_string(m_lines));
}

void GameScreen::renderBoard(sf::RenderWindow* window)
{
	sf::RectangleShape rect({BOARD_WIDTH, BOARD_HEIGHT});
	rect.setPosition(BOARD_X, BOARD_Y);
	rect.setFillColor(sf::Color::Transparent);
	rect.setOutlineColor(sf::Color::White);
	rect.setOutlineThickness(5.f);
	window->draw(rect);
}

void GameScreen::renderTiles(sf::RenderWindow* window)
{
	sf::Sprite sprite(Game::getResources().tile);
	sprite.setScale({CELL_SIZE / 16.f, CELL_SIZE / 16.f});
	for (int row = 0; row < ROWS; ++row)
		for (int col = 0; col < COLS; ++col)
		{
			if (m_matrix->at(row, col) == 0)
				continue;

			sprite.setColor(numToColor.at(m_matrix->at(row, col)));
			sprite.setPosition(BOARD_X + col * CELL_SIZE, BOARD_Y + row * CELL_SIZE);
			window->draw(sprite);
		}
}

void GameScreen::renderGrid(sf::RenderWindow* window)
{
	sf::RectangleShape rect({CELL_SIZE, CELL_SIZE});
	rect.setFillColor(sf::Color::Transparent);
	rect.setOutlineColor(sf::Color(40, 40, 40, 255));
	rect.setOutlineThickness(1.f);

	for (int row = 0; row < ROWS; ++row)
		for (int col = 0; col < COLS; ++col)
		{
			rect.setPosition(BOARD_X + col * CELL_SIZE, BOARD_Y + row * CELL_SIZE);
			window->draw(rect);
		}
}

void GameScreen::renderUI(sf::RenderWindow* window)
{
	renderShadow(window, m_scoreText);
	window->draw(m_scoreText);

	renderShadow(window, m_lineText);
	window->draw(m_lineText);
	
	renderShadow(window, m_nextText);
	window->draw(m_nextText);

	if (m_paused)
	{
		sf::RectangleShape rect({float(SCREEN_WIDTH), float(SCREEN_HEIGHT)});
		rect.setFillColor(sf::Color(0, 0, 0, 175));
		window->draw(rect);

		renderShadow(window, m_pausedText);
		window->draw(m_pausedText);
	}
}

void GameScreen::renderPieces(sf::RenderWindow* window)
{
	auto getPosAt = [&](Piece* piece, int i, int j) -> sf::Vector2f
	{
		return {BOARD_X + (piece->getX() + j) * CELL_SIZE, BOARD_Y + (piece->getY() + i) * CELL_SIZE};
	};

	// Current and ghost
	for (size_t i = 0; i < m_currentPiece->getSize(); ++i)
		for (size_t j = 0; j < m_currentPiece->getSize(); ++j)
		{
			int current = m_currentPiece->getMatrix()->at(i, j);

			if (current == 0)
				continue;

			sf::RectangleShape rect({CELL_SIZE, CELL_SIZE});
			rect.setFillColor(sf::Color::Transparent);
			rect.setOutlineColor(sf::Color::White);
			rect.setOutlineThickness(-1.f);
			rect.setPosition(getPosAt(m_ghost, i, j));
			window->draw(rect);
			
			sf::Sprite sprite(Game::getResources().tile);
			sprite.setScale({CELL_SIZE / 16.f, CELL_SIZE / 16.f});
			sprite.setColor(numToColor.at(current));
			sprite.setPosition(getPosAt(m_currentPiece, i, j));
			window->draw(sprite);
		}

	// Next piece
	float x = float(SCREEN_WIDTH / 5) * 3.5f + m_nextPiece->getX() * CELL_SIZE;
	float y = 315.f + m_nextPiece->getY() * CELL_SIZE;
	float size = 4.f * CELL_SIZE;

	sf::RectangleShape rect({size, size});
	rect.setPosition(x, y);
	rect.setFillColor(sf::Color::Transparent);
	rect.setOutlineColor(sf::Color::White);
	rect.setOutlineThickness(5.f);
	window->draw(rect);

	if (m_nextPiece->getRandTetromino() == 0)
		rotateMatrixCCW(m_nextPiece->getMatrix());

	for (size_t i = 0; i < m_nextPiece->getSize(); ++i)
		for (size_t j = 0; j < m_nextPiece->getSize(); ++j)
		{
			int current = m_nextPiece->getMatrix()->at(i, j);

			if (current == 0)
				continue;

			sf::Sprite sprite(Game::getResources().tile);
			sprite.setScale({CELL_SIZE / 16.f, CELL_SIZE / 16.f});
			sprite.setColor(numToColor.at(current));

			sprite.setPosition({x + j * CELL_SIZE + (m_nextPiece->getSize() == 3 ? CELL_SIZE * 0.5f : 0.f), y + i * CELL_SIZE + (m_nextPiece->getSize() == 3 ? CELL_SIZE : 0.f)});
			
			window->draw(sprite);
		}

	if (m_nextPiece->getRandTetromino() == 0)
		rotateMatrixCW(m_nextPiece->getMatrix());
}

void GameScreen::renderLineClearAnim(sf::RenderWindow* window)
{
	sf::RectangleShape rect({CELL_SIZE * float(COLS), CELL_SIZE});
	rect.setFillColor(sf::Color::White);
	for (int row : m_rowsToClear)
	{
		rect.setPosition(BOARD_X, BOARD_Y + row * CELL_SIZE);
		if (int(m_rowClearAnimTimer * 10.f) % 2 == 1)
			window->draw(rect);
	}
}

unsigned int GameScreen::getScore() const
{
	return m_score;
}

unsigned int GameScreen::getLines() const
{
	return m_lines;
}
