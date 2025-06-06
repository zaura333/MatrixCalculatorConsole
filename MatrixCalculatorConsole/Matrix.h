#pragma once
#include <stdexcept>
#include <iostream>
#include <ostream>
#include <complex>
#include <type_traits>
#include "MatrixExceptions.h"

template<typename T>
class Matrix
{
public:
	static_assert(
        std::is_same_v<T, int> ||
        std::is_same_v<T, double> ||
        std::is_same_v<T, std::complex<double>>,
        "Matrix<T>: T może być tylko int, double lub std::complex<double>"
    );
	// Konstruktor
	Matrix(int r = 0, int c = 0);
	// Konstruktor z inicjalizacją
	Matrix(int r, int c, T initial);
	//Konstruktor kopia
	Matrix(const Matrix<T>& mt);
	// Pobranie wartości elementu, zwraca referencje do obiektu i umożliwia edytowanie, np.: M(2,3)=5
	T& operator()(int x, int y) const;
	// Wyświetlanie całej macierzy (https://learn.microsoft.com/en-us/cpp/standard-library/overloading-the-output-operator-for-your-own-classes?view=msvc-170)
	template<typename U>
	friend std::ostream& operator<<(std::ostream& os, const Matrix<U>& mt);
	// Kopiowanie macierzy
	Matrix<T>& operator=(const Matrix& toCopy);
	// Dodawanie macierzy
	Matrix<T> operator+(Matrix<T>& mt);
	// Odejmowanie macierzy
	Matrix<T> operator-(Matrix<T>& mt);
	// Mnożenie przez liczbę
	Matrix<T> operator*(T n);
	template<typename U>
	friend Matrix<U> operator*(U n, Matrix<U>& mt);
	// Mnożenie macierzy
	Matrix<T> operator*(Matrix& mt);
	// Transponowanie macierzy
	Matrix<T> transpose();
	// Liczenie wyznacznika
	T getDet();
	// Destruktor
	~Matrix();
private:
	// Liczba wierszy/kolumn macierzy
	int rows, columns;
	T** data; // Pointer do tablicy wierszy
	void freeDataMemory() const; // Funkcja usuwajca tablice i czyszacza dane macierzy.
	void copyData(const Matrix<T>& mt);
};

template<typename T>
Matrix<T>::Matrix(int r, int c) : rows(r), columns(c) {
	data = new T * [rows];

	for (int i = 0; i < rows; i++) {
		data[i] = new T[columns];
	}
}

template<typename T>
Matrix<T>::Matrix(int r, int c, T initial) : rows(r), columns(c) {
	data = new T * [rows];

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
	this->rows = mt.rows;
	this->columns = mt.columns;
	copyData(mt);
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

	copyData(toCopy);

	return *this;
}

template<typename T>
Matrix<T> Matrix<T>::operator+(Matrix<T>& mt) {
	if (mt.rows != rows || mt.columns != columns) {
		throw Matrix_size_not_match("Error: Matrix sizes must match to perform addition.");
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
		throw Matrix_size_not_match("Error: Matrix sizes must match to perform subtraction.");
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
		throw Matrix_size_not_match("Error: Number of rows of the first matrix must be equal to the number of columns of the second matrix.");
	}

	int common = columns;

	Matrix result(rows, mt.columns);

	for (int i = 1; i <= result.rows; i++) {
		for (int j = 1; j <= result.columns; j++) {
			T element = 0;

			for (int k = 1; k <= common; k++) {
				T a = this->operator()(i, k);
				T b = mt(k, j);
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
T Matrix<T>::getDet()
{
	if (rows != columns) {
		throw Matrix_size_not_match("Error: Matrix must be square to calculate determinant.");
	}

	int n = rows;

	if (n == 1) {
		return (*this)(1, 1);
	}

	if (n == 2) {
		return (*this)(1, 1) * (*this)(2, 2) - (*this)(1, 2) * (*this)(2, 1);
	}

	T res = 0;
	for (int col = 1; col <= n; ++col) {
		// Tworzenie podmacierzy (bez pierwszego wiersza i kolumny col)
		Matrix<T> subMat(n - 1, n - 1);

		for (int i = 2; i <= n; ++i) {
			int subCol = 1;
			for (int j = 1; j <= n; ++j) {
				if (j == col) continue;
				subMat(i - 1, subCol) = (*this)(i, j);
				++subCol;
			}
		}

		int sign = ((col % 2) == 1) ? 1 : -1;
		res += sign * (*this)(1, col) * subMat.getDet();
	}

	return res;
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
inline void Matrix<T>::copyData(const Matrix<T>& mt)
{
	rows = mt.rows;
	columns = mt.columns;

	data = new T * [rows];

	for (int i = 0; i < rows; i++) {
		data[i] = new T[columns];
		for (int j = 0; j < columns; j++) {
			data[i][j] = mt.data[i][j];
		}
	}
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

template<typename U>
inline Matrix<U> operator*(U n, Matrix<U>& mt)
{
	return mt * n;
}
