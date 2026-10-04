#pragma once
#include <stdexcept>
#include <iostream>
#include <ostream>
#include <complex>
#include <type_traits>
#include <vector>
#include <cstddef>
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
	Matrix(int r = 0, int c = 0);
	Matrix(int r, int c, T initial);
	T& operator()(int x, int y);
	const T& operator()(int x, int y) const;
	template<typename U>
	friend std::ostream& operator<<(std::ostream& os, const Matrix<U>& mt);
	[[nodiscard]] Matrix<T> operator+(const Matrix<T>& mt) const;
	[[nodiscard]] Matrix<T> operator-(const Matrix<T>& mt) const;
	[[nodiscard]] Matrix<T> operator*(T n) const;
	template<typename U>
	friend Matrix<U> operator*(U n, const Matrix<U>& mt);
	[[nodiscard]] Matrix<T> operator*(const Matrix& mt) const;
	[[nodiscard]] Matrix<T> transpose() const;
	[[nodiscard]] T getDet() const;

private:
	int rows, columns;
	std::vector<T> data;
	std::size_t index(int x, int y) const noexcept;
	void checkBounds(int x, int y) const;
	static std::size_t checkedSize(int r, int c);
};

template<typename T>
Matrix<T>::Matrix(int r, int c)
	: rows(r), columns(c), data(checkedSize(r, c)) {
	// vector value-inicjalizuje elementy: 0, 0.0 lub (0,0)
}

template<typename T>
Matrix<T>::Matrix(int r, int c, T initial)
	: rows(r), columns(c), data(checkedSize(r, c), initial) {
}

template<typename T>
T& Matrix<T>::operator()(int x, int y) {
	checkBounds(x, y);
	return data[index(x, y)];
}

template<typename T>
const T& Matrix<T>::operator()(int x, int y) const {
	checkBounds(x, y);
	return data[index(x, y)];
}

template<typename T>
Matrix<T> Matrix<T>::operator+(const Matrix<T>& mt) const {
	if (mt.rows != rows || mt.columns != columns) {
		throw SizeMismatchException("Error: Matrix sizes must match to perform addition.");
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
Matrix<T> Matrix<T>::operator-(const Matrix<T>& mt) const
{
	if (mt.rows != rows || mt.columns != columns) {
		throw SizeMismatchException("Error: Matrix sizes must match to perform subtraction.");
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
Matrix<T> Matrix<T>::operator*(T n) const
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
Matrix<T> Matrix<T>::operator*(const Matrix<T>& mt) const
{
	if (columns != mt.rows) {
		throw SizeMismatchException("Error: Number of rows of the first matrix must be equal to the number of columns of the second matrix.");
	}

	int common = columns;

	Matrix result(rows, mt.columns);

	for (int i = 1; i <= result.rows; i++) {
		for (int j = 1; j <= result.columns; j++) {
			T element = 0;

			for (int k = 1; k <= common; k++) {
				element += (this->operator()(i, k) * mt(k, j));
			}

			result(i, j) = element;
		}
	}

	return result;
}

template<typename T>
Matrix<T> Matrix<T>::transpose() const
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
T Matrix<T>::getDet() const
{
	if (rows != columns) {
		throw NonSquareMatrixException("Error: Matrix must be square to calculate determinant.");
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
		Matrix<T> subMat(n - 1, n - 1);

		for (int i = 2; i <= n; ++i) {
			int subCol = 1;
			for (int j = 1; j <= n; ++j) {
				if (j == col) continue;
				subMat(i - 1, subCol) = (*this)(i, j);
				++subCol;
			}
		}

		const T sign = ((col % 2) == 1) ? 1 : -1;
		res += sign * (*this)(1, col) * subMat.getDet();
	}

	return res;
}

template<typename T>
std::size_t Matrix<T>::index(int x, int y) const noexcept
{
	return static_cast<std::size_t>(x - 1) * static_cast<std::size_t>(columns)
		+ static_cast<std::size_t>(y - 1);
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
[[nodiscard]] inline Matrix<U> operator*(U n, const Matrix<U>& mt)
{
	return mt * n;
}

template<typename T>
std::size_t Matrix<T>::checkedSize(int r, int c)
{
	if (r < 0 || c < 0) {
		throw InvalidDimensionException("Error: Matrix dimensions must not be negative.");
	}

	return static_cast<std::size_t>(r) * static_cast<std::size_t>(c);
}

template<typename T>
void Matrix<T>::checkBounds(int x, int y) const
{
	if (x > rows || x <= 0 || y > columns || y <= 0) {
		throw IndexOutOfBoundsException("Error: Indices out of bounds of the matrix.");
	}
}
