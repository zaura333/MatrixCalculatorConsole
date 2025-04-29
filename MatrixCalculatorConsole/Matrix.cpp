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

Matrix::~Matrix() {
	for (int i = 0; i < rows; i++) {
		delete[] data[i];
	}
	delete[] data;
}
