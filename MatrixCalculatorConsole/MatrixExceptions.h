#pragma once
#include <stdexcept>
#include <string>

class MatrixException : public std::logic_error {
public:
	using std::logic_error::logic_error;
};

class SizeMismatchException : public MatrixException {
public:
	using MatrixException::MatrixException;
};

class NonSquareMatrixException : public MatrixException {
public:
	using MatrixException::MatrixException;
};

class IndexOutOfBoundsException : public MatrixException {
public:
	using MatrixException::MatrixException;
};

class InvalidDimensionException : public MatrixException {
public:
	using MatrixException::MatrixException;
};
