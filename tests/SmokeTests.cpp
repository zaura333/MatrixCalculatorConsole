#include <gtest/gtest.h>
#include "Matrix.h"

TEST(Smoke, CreatesMatrix) {
	Matrix<int> m(2, 3, 1);
	EXPECT_EQ(m(1, 1), 1);
	EXPECT_EQ(m(2, 3), 1);
}

TEST(Smoke, ThrowsOnOutOfBoundsAccess) {
	Matrix<int> m(2, 3);
	EXPECT_THROW(m(0, 1), IndexOutOfBoundsException);
	EXPECT_THROW(m(1, 0), IndexOutOfBoundsException);
	EXPECT_THROW(m(3, 1), IndexOutOfBoundsException);
	EXPECT_THROW(m(1, 4), IndexOutOfBoundsException);
}

TEST(Smoke, AddsMatrices) {
	Matrix<int> a(2, 2, 1), b(2, 2, 2);
	Matrix<int> c = a + b;
	EXPECT_EQ(c(2, 2), 3);
}

TEST(Smoke, ThrowsOnSizeMismatch) {
	Matrix<int> a(2, 2, 0), b(3, 3, 0);
	EXPECT_ANY_THROW(static_cast<void>(a + b));
}