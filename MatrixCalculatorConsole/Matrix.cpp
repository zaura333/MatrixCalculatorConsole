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
		// Show that and error occured, terminate the program;
		std::cerr << "Error: Invalid matrix indices (" << x << ", " << y << ")\n";
		std::exit(-1); 
		//throw std::out_of_range("Indices out of range of the matrix.");

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
		std::cerr << "Error: Matrix sizes must match.\n";
		std::exit(-1);
	}

	Matrix result(rows, columns);

	for (int i = 1; i <= rows; i++) {
		for (int j = 1; j <= columns; j++) {
			result(i, j) = mt(i, j) + this->operator()(i, j);
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
			os << ' ' << mt(i, j);
		}
		os << '\n';
	}
	os << "]\n";
	
	return os;
}
