#include "Matrix.h"

Matrix::Matrix(int r, int c) : rows(r), columns(c) {
	data = new double* [rows];

	for (int i = 0; i < rows; i++) {
		data[i] = new double[columns];
	}
}

Matrix::Matrix(int r, int c, double initial) : rows(r), columns(c) {
	data = new double* [rows];

	for (int i = 0; i < rows; i++) {
		data[i] = new double[columns];

		for (int j = 0; j < columns; j++) {
			data[i][j] = initial;
		}
	}
}

Matrix::Matrix(const Matrix& mt)
{
	*this = mt;
}

double& Matrix::operator()(int x, int y) const {
	if (x > rows || x <= 0 || y > columns || y <= 0) {
		throw std::out_of_range("Error: Indices out of bounds of the matrix.");
	}

	return data[x - 1][y - 1];
}

Matrix& Matrix::operator=(const Matrix& toCopy)
{
	if (this == &toCopy) {
		return *this;
	}

	freeDataMemory();

	rows = toCopy.rows;
	columns = toCopy.columns;

	data = new double* [rows];

    for (int i = 0; i < rows; i++) {
        data[i] = new double[columns];
        for (int j = 0; j < columns; j++) {
            data[i][j] = toCopy.data[i][j];
        }
    }

	return *this;
}

Matrix Matrix::operator+(Matrix& mt) {
	if (mt.rows != rows || mt.columns != columns) {
		throw std::invalid_argument("Error: Matrix sizes must match to perform addition.");
	}

	Matrix result(rows, columns);

	for (int i = 1; i <= rows; i++) {
		for (int j = 1; j <= columns; j++) {
			result(i, j) = mt(i, j) + this->operator()(i, j);
		}
	}
	
	return result;
}

Matrix Matrix::operator-(Matrix& mt)
{
	if (mt.rows != rows || mt.columns != columns) {
		throw std::invalid_argument("Error: Matrix sizes must match to perform subtraction.");
	}

	Matrix result(rows, columns);

	for (int i = 1; i <= rows; i++) {
		for (int j = 1; j <= columns; j++) {
			result(i, j) = this->operator()(i, j) - mt(i, j);
		}
	}

	return result;
}

Matrix Matrix::operator*(double n)
{
	Matrix result(rows, columns);

	for (int i = 1; i <= rows; i++) {
		for (int j = 1; j <= columns; j++) {
			result(i, j) = this->operator()(i, j) * n;
		}
	}

	return result;
}

Matrix Matrix::operator*(Matrix& mt)
{
	if (columns != mt.rows) {
		throw std::invalid_argument("Error: Number of rows of the first matrix must be equal to the number of columns of the second matrix.");
	}

	int common = columns;

	Matrix result(rows, mt.columns);

	for (int i = 1; i <= result.rows; i++) {
		for (int j = 1; j <= result.columns; j++) {
			double element = 0;

			for (int k = 1; k <= common; k++) {
				int a = this->operator()(i, k);
				int b = mt(k, j);
				element += (this->operator()(i, k) * mt(k, j));
			}

			result(i, j) = element;
		}
	}

	return result;
}

Matrix Matrix::transpose()
{
	Matrix result(columns, rows);

	for (int i = 1; i <= rows; i++) {
		for (int j = 1; j <= columns; j++) {
			result(j, i) = this->operator()(i, j);
		}
	}

	return result;
}

Matrix::~Matrix() {
	freeDataMemory();
}

void Matrix::freeDataMemory() const
{
	for (int i = 0; i < rows; i++) {
		delete[] data[i];
	}
	delete[] data;
}

std::ostream& operator<<(std::ostream& os, const Matrix& mt)
{
	os << "\n[\n";
	for (int i = 1; i <= mt.rows; i++) {
		for (int j = 1; j <= mt.columns; j++) {
			os << '\t' << mt(i, j);
		}
		os << '\n';
	}
	os << "]\n";
	
	return os;
}

Matrix operator*(double n, Matrix& mt) {
	return mt * n;
}