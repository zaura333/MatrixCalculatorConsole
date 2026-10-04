#include <gtest/gtest.h>
#include <exception>
#include <type_traits>
#include "Matrix.h"

// Hierarchia sprawdzana ju¿ w czasie kompilacji
static_assert(std::is_base_of_v<MatrixException, SizeMismatchException>);
static_assert(std::is_base_of_v<MatrixException, NonSquareMatrixException>);
static_assert(std::is_base_of_v<MatrixException, IndexOutOfBoundsException>);
static_assert(std::is_base_of_v<std::exception, MatrixException>);

TEST(MatrixExceptions, AdditionOfDifferentSizesThrowsSizeMismatch) {
	Matrix<int> a(2, 3, 0), b(3, 2, 0);
	EXPECT_THROW(static_cast<void>(a + b), SizeMismatchException);
}

TEST(MatrixExceptions, DeterminantOfNonSquareThrowsNonSquare) {
	Matrix<int> m(2, 3, 0);
	EXPECT_THROW(static_cast<void>(m.getDet()), NonSquareMatrixException);
}

TEST(MatrixExceptions, BadIndexThrowsIndexOutOfBounds) {
	Matrix<int> m(2, 2, 0);
	EXPECT_THROW(m(3, 1), IndexOutOfBoundsException);
}

TEST(MatrixExceptions, AllErrorsCatchableAsMatrixException) {
	Matrix<int> a(2, 3, 0), b(3, 2, 0);
	EXPECT_THROW(static_cast<void>(a + b), MatrixException);
	EXPECT_THROW(static_cast<void>(a.getDet()), MatrixException);
	EXPECT_THROW(a(5, 5), MatrixException);
}

TEST(MatrixExceptions, CatchableAsStdExceptionWithMessage) {
	Matrix<int> m(2, 2, 0);
	try {
		m(3, 3);
		FAIL() << "expected IndexOutOfBoundsException";
	}
	catch (const std::exception& e) {
		EXPECT_STRNE(e.what(), "");
	}
}

TEST(MatrixExceptions, SizeMismatchIsNotAnIndexError) {
	Matrix<int> a(2, 3, 0), b(3, 2, 0);
	try {
		static_cast<void>(a + b);
		FAIL() << "expected an exception";
	}
	catch (const IndexOutOfBoundsException&) {
		FAIL() << "wrong exception type";
	}
	catch (const SizeMismatchException&) {
		SUCCEED();
	}
}
