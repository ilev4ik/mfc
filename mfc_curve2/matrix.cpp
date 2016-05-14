#include "matrix.h"

SquareMatrix::SquareMatrix()
{
	matrix = new double*[DIM];

	for (int i = 0; i < DIM; ++i)
		matrix[i] = new double[DIM];
}

SquareMatrix::~SquareMatrix()
{
	for (int i = 0; i < DIM; ++i)
	{
		delete[] matrix[i];
	}
	delete[] matrix;
}

double& SquareMatrix::operator() (int i, int j)
{
	return matrix[i - 1][j - 1];
}