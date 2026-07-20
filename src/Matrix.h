#pragma once

class Matrix
{
private:
	unsigned int m_rows, m_cols;
	int** m_array;

public:
	Matrix(unsigned int rows, unsigned int cols);
	~Matrix();

	unsigned int getRows() const;
	unsigned int getCols() const;
	bool verify(int row, int col) const;
	int at(int row, int col) const;
	void set(int row, int col, int val);
};
