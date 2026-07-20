#include "Matrix.h"
#include <cstddef>

Matrix::Matrix(unsigned int rows, unsigned int cols) :
	m_rows(rows),
	m_cols(cols)
{
	m_array = new int* [rows * cols];

	for (size_t i = 0; i < m_rows; ++i)
		for (size_t j = 0; j < m_cols; ++j)
		{
			m_array[i * m_cols + j] = new int(0);
		}
}

Matrix::~Matrix()
{
	for (size_t i = 0; i < m_rows * m_cols; ++i)
		delete m_array[i];

	delete[] m_array;
}

unsigned int Matrix::getRows() const
{
	return m_rows;
}

unsigned int Matrix::getCols() const
{
	return m_cols;
}

bool Matrix::verify(int row, int col) const
{
	return row >= 0 && row < int(m_rows) && col >= 0 && col < int(m_cols);
}

int Matrix::at(int row, int col) const
{
	if (!verify(row, col))
		return -1;

	return *m_array[row * m_cols + col];
}

void Matrix::set(int row, int col, int val)
{
	if (!verify(row, col))
		return;

	*m_array[row * m_cols + col] = val;
}
