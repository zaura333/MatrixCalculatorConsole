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

double& Matrix::operator()(int x, int y) const {
	if (x > rows || x <= 0 || y > columns || y <= 0) {
		// Show that and error occured, terminate the program;
		std::cerr << "Error: Invalid matrix indices (" << x << ", " << y << ")\n";
		std::exit(EXIT_FAILURE); 

	}

	return data[x - 1][y - 1];
}

Matrix::~Matrix() {
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
