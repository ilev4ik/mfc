#ifndef MATRIX
#define MATRIX

#define DIM 3

class SquareMatrix
{
public:
	SquareMatrix();
	~SquareMatrix();
	double& operator() (int i, int j);
private:
	double** matrix;
};

#endif