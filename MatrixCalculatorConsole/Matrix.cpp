#include "Matrix.h"

template<typename T>
Matrix<T>::Matrix(int r, int c) : rows(r), columns(c) {
	data = new T* [rows];

	for (int i = 0; i < rows; i++) {
		data[i] = new T[columns];
	}
}

template<typename T>
Matrix<T>::Matrix(int r, int c, T initial) : rows(r), columns(c) {
	data = new T* [rows];

	for (int i = 0; i < rows; i++) {
		data[i] = new T[columns];

		for (int j = 0; j < columns; j++) {
			data[i][j] = initial;
		}
	}
}

template<typename T>
Matrix<T>::Matrix(const Matrix<T>& mt)
{
	*this = mt;
}

template<typename T>
T& Matrix<T>::operator()(int x, int y) const {
	if (x > rows || x <= 0 || y > columns || y <= 0) {
		throw std::out_of_range("Error: Indices out of bounds of the matrix.");
	}

	return data[x - 1][y - 1];
}

template<typename T>
Matrix<T>& Matrix<T>::operator=(const Matrix<T>& toCopy)
{
	if (this == &toCopy) {
		return *this;
	}

	freeDataMemory();

	rows = toCopy.rows;
	columns = toCopy.columns;

	data = new T* [rows];

    for (int i = 0; i < rows; i++) {
        data[i] = new T[columns];
        for (int j = 0; j < columns; j++) {
            data[i][j] = toCopy.data[i][j];
        }
    }

	return *this;
}

template<typename T>
Matrix<T> Matrix<T>::operator+(Matrix<T>& mt) {
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

template<typename T>
Matrix<T> Matrix<T>::operator-(Matrix<T>& mt)
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

template<typename T>
Matrix<T> Matrix<T>::operator*(T n)
{
	Matrix result(rows, columns);

	for (int i = 1; i <= rows; i++) {
		for (int j = 1; j <= columns; j++) {
			result(i, j) = this->operator()(i, j) * n;
		}
	}

	return result;
}

template<typename T>
Matrix<T> Matrix<T>::operator*(Matrix<T>& mt)
{
	if (columns != mt.rows) {
		throw std::invalid_argument("Error: Number of rows of the first matrix must be equal to the number of columns of the second matrix.");
	}

	int common = columns;

	Matrix result(rows, mt.columns);

	for (int i = 1; i <= result.rows; i++) {
		for (int j = 1; j <= result.columns; j++) {
			T element = 0;

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

template<typename T>
Matrix<T> Matrix<T>::transpose()
{
	Matrix result(columns, rows);

	for (int i = 1; i <= rows; i++) {
		for (int j = 1; j <= columns; j++) {
			result(j, i) = this->operator()(i, j);
		}
	}

	return result;
}

template<typename T>
Matrix<T>::~Matrix() {
	freeDataMemory();
}

template<typename T>
void Matrix<T>::freeDataMemory() const
{
	for (int i = 0; i < rows; i++) {
		delete[] data[i];
	}
	delete[] data;
}

template<typename T>
std::ostream& operator<<(std::ostream& os, const Matrix<T>& mt)
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

template<typename T>
Matrix<T> operator*(T n, Matrix<T>& mt) {
	return mt * n;
}