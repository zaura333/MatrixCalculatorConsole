#include <gtest/gtest.h>
#include <complex>
#include <initializer_list>
#include <sstream>
#include <stdexcept>
#include <type_traits>
#include <utility>
#include <vector>
#include "Matrix.h"

// ---------------------------------------------------------------------------
// Funkcje pomocnicze
// ---------------------------------------------------------------------------
namespace {

// Tworzy macierz rows x cols z wartosci podanych wierszami (indeksowanie od 1).
template <typename T>
Matrix<T> makeMatrix(int rows, int cols, std::initializer_list<T> values) {
	Matrix<T> m(rows, cols, T{});
	auto it = values.begin();
	for (int i = 1; i <= rows; ++i) {
		for (int j = 1; j <= cols; ++j) {
			m(i, j) = *it++;
		}
	}
	return m;
}

// Sprawdza wszystkie elementy macierzy (wartosci podane wierszami).
template <typename T>
void expectMatrixEq(const Matrix<T>& m, int rows, int cols, std::initializer_list<T> expected) {
	auto it = expected.begin();
	for (int i = 1; i <= rows; ++i) {
		for (int j = 1; j <= cols; ++j) {
			EXPECT_EQ(m(i, j), *it++) << "different element at (" << i << ", " << j << ")";
		}
	}
}

}  // namespace

// ---------------------------------------------------------------------------
// Testy dla wszystkich obslugiwanych typow: int, double, std::complex<double>
// ---------------------------------------------------------------------------
template <typename T>
class MatrixTyped : public ::testing::Test {};

using AllTypes = ::testing::Types<int, double, std::complex<double>>;
TYPED_TEST_SUITE(MatrixTyped, AllTypes);

TYPED_TEST(MatrixTyped, ConstructorWithInitialValueFillsAllElements) {
	Matrix<TypeParam> m(2, 3, TypeParam{7});
	expectMatrixEq<TypeParam>(m, 2, 3, {7, 7, 7, 7, 7, 7});
}

TYPED_TEST(MatrixTyped, IndexingIsOneBasedAndAddressesDistinctElements) {
	Matrix<TypeParam> m(2, 3, TypeParam{0});
	m(1, 1) = TypeParam{1};
	m(1, 3) = TypeParam{2};
	m(2, 1) = TypeParam{3};
	m(2, 3) = TypeParam{4};
	expectMatrixEq<TypeParam>(m, 2, 3, {1, 0, 2, 3, 0, 4});
}

TYPED_TEST(MatrixTyped, AddsElementwise) {
	auto a = makeMatrix<TypeParam>(2, 3, {1, 2, 3, 4, 5, 6});
	auto b = makeMatrix<TypeParam>(2, 3, {10, 20, 30, 40, 50, 60});
	Matrix<TypeParam> sum = a + b;
	expectMatrixEq<TypeParam>(sum, 2, 3, {11, 22, 33, 44, 55, 66});
}

TYPED_TEST(MatrixTyped, AdditionDoesNotModifyOperands) {
	auto a = makeMatrix<TypeParam>(1, 2, {1, 2});
	auto b = makeMatrix<TypeParam>(1, 2, {3, 4});
	Matrix<TypeParam> sum = a + b;
	static_cast<void>(sum);
	expectMatrixEq<TypeParam>(a, 1, 2, {1, 2});
	expectMatrixEq<TypeParam>(b, 1, 2, {3, 4});
}

TYPED_TEST(MatrixTyped, AdditionOfDifferentSizesThrows) {
	Matrix<TypeParam> a(2, 3, TypeParam{1});
	Matrix<TypeParam> b(3, 2, TypeParam{1});
	EXPECT_THROW(static_cast<void>(a + b), SizeMismatchException);
}

TYPED_TEST(MatrixTyped, SubtractsElementwise) {
	auto a = makeMatrix<TypeParam>(2, 3, {10, 20, 30, 40, 50, 60});
	auto b = makeMatrix<TypeParam>(2, 3, {1, 2, 3, 4, 5, 6});
	Matrix<TypeParam> diff = a - b;
	expectMatrixEq<TypeParam>(diff, 2, 3, {9, 18, 27, 36, 45, 54});
}

TYPED_TEST(MatrixTyped, SubtractionOfDifferentSizesThrows) {
	Matrix<TypeParam> a(2, 3, TypeParam{1});
	Matrix<TypeParam> b(2, 2, TypeParam{1});
	EXPECT_THROW(static_cast<void>(a - b), SizeMismatchException);
}

TYPED_TEST(MatrixTyped, MultipliesMatrices) {
	auto a = makeMatrix<TypeParam>(2, 3, {1, 2, 3, 4, 5, 6});
	auto b = makeMatrix<TypeParam>(3, 2, {7, 8, 9, 10, 11, 12});
	Matrix<TypeParam> product = a * b;
	expectMatrixEq<TypeParam>(product, 2, 2, {58, 64, 139, 154});
}

TYPED_TEST(MatrixTyped, MultiplicationWithIncompatibleSizesThrows) {
	Matrix<TypeParam> a(2, 3, TypeParam{1});
	Matrix<TypeParam> b(2, 2, TypeParam{1});   // a ma 3 kolumny, b ma 2 wiersze
	EXPECT_THROW(static_cast<void>(a * b), SizeMismatchException);
}

TYPED_TEST(MatrixTyped, MultipliesByScalarOnTheRight) {
	auto m = makeMatrix<TypeParam>(2, 2, {1, 2, 3, 4});
	Matrix<TypeParam> scaled = m * TypeParam{3};
	expectMatrixEq<TypeParam>(scaled, 2, 2, {3, 6, 9, 12});
}

TYPED_TEST(MatrixTyped, MultipliesByScalarOnTheLeft) {
	auto m = makeMatrix<TypeParam>(2, 2, {1, 2, 3, 4});
	Matrix<TypeParam> scaled = TypeParam{3} * m;
	expectMatrixEq<TypeParam>(scaled, 2, 2, {3, 6, 9, 12});
}

TYPED_TEST(MatrixTyped, MultiplyingByZeroGivesZeroMatrix) {
	auto m = makeMatrix<TypeParam>(2, 2, {1, 2, 3, 4});
	Matrix<TypeParam> zero = m * TypeParam{0};
	expectMatrixEq<TypeParam>(zero, 2, 2, {0, 0, 0, 0});
}

TYPED_TEST(MatrixTyped, TransposeSwapsRowsAndColumns) {
	auto m = makeMatrix<TypeParam>(2, 3, {1, 2, 3, 4, 5, 6});
	Matrix<TypeParam> t = m.transpose();
	expectMatrixEq<TypeParam>(t, 3, 2, {1, 4, 2, 5, 3, 6});
}

TYPED_TEST(MatrixTyped, TransposingTwiceGivesOriginal) {
	auto m = makeMatrix<TypeParam>(2, 3, {1, 2, 3, 4, 5, 6});
	Matrix<TypeParam> t = m.transpose();
	Matrix<TypeParam> back = t.transpose();
	expectMatrixEq<TypeParam>(back, 2, 3, {1, 2, 3, 4, 5, 6});
}

TYPED_TEST(MatrixTyped, DeterminantOf1x1IsTheElement) {
	auto m = makeMatrix<TypeParam>(1, 1, {5});
	EXPECT_EQ(m.getDet(), TypeParam{5});
}

TYPED_TEST(MatrixTyped, DeterminantOf2x2) {
	auto m = makeMatrix<TypeParam>(2, 2, {1, 2, 3, 4});
	EXPECT_EQ(m.getDet(), TypeParam{-2});        // 1*4 - 2*3
}

TYPED_TEST(MatrixTyped, DeterminantOf3x3) {
	auto m = makeMatrix<TypeParam>(3, 3, {6, 1, 1, 4, -2, 5, 2, 8, 7});
	EXPECT_EQ(m.getDet(), TypeParam{-306});
}

TYPED_TEST(MatrixTyped, DeterminantOfSingularMatrixIsZero) {
	auto m = makeMatrix<TypeParam>(3, 3, {1, 2, 3, 4, 5, 6, 7, 8, 9});
	EXPECT_EQ(m.getDet(), TypeParam{0});
}

TYPED_TEST(MatrixTyped, DeterminantOfNonSquareMatrixThrows) {
	Matrix<TypeParam> m(2, 3, TypeParam{1});
	EXPECT_THROW(static_cast<void>(m.getDet()), NonSquareMatrixException);
}

// ---------------------------------------------------------------------------
// Testy niezalezne od typu
// ---------------------------------------------------------------------------
TEST(MatrixBasics, DefaultConstructorCreatesEmptyMatrix) {
	EXPECT_NO_THROW(Matrix<int> m);
	Matrix<int> m;
	EXPECT_THROW(m(1, 1), IndexOutOfBoundsException);   // brak elementow
}

TEST(MatrixIndexing, OutOfRangeThrows) {
	Matrix<int> m(2, 3, 0);
	EXPECT_THROW(m(0, 1), IndexOutOfBoundsException);
	EXPECT_THROW(m(1, 0), IndexOutOfBoundsException);
	EXPECT_THROW(m(3, 1), IndexOutOfBoundsException);    // rows + 1
	EXPECT_THROW(m(1, 4), IndexOutOfBoundsException);    // columns + 1
	EXPECT_THROW(m(-1, 1), IndexOutOfBoundsException);
	EXPECT_THROW(m(1, -1), IndexOutOfBoundsException);
}

TEST(MatrixIndexing, BoundaryElementsAreAccessible) {
	Matrix<int> m(2, 3, 0);
	EXPECT_NO_THROW(m(1, 1));
	EXPECT_NO_THROW(m(2, 3));
}

TEST(MatrixIndexing, ConstMatrixIsReadable) {
	const Matrix<int> m(2, 2, 4);
	EXPECT_EQ(m(1, 2), 4);
	EXPECT_THROW(m(3, 1), IndexOutOfBoundsException);
}

TEST(MatrixOutput, PrintsRowsSeparatedByNewlines) {
	auto m = makeMatrix<int>(2, 2, {1, 2, 3, 4});
	std::ostringstream os;
	os << m;
	EXPECT_EQ(os.str(), "\n[\n\t1\t2\n\t3\t4\n]\n");
}

TEST(MatrixOutput, PrintsDoubles) {
	auto m = makeMatrix<double>(1, 2, {1.5, -2.25});
	std::ostringstream os;
	os << m;
	EXPECT_EQ(os.str(), "\n[\n\t1.5\t-2.25\n]\n");
}

TEST(MatrixOutput, PrintsComplexAsAPlusBi) {
	auto m = makeMatrix<std::complex<double>>(1, 2, {{1, 2}, {3, -4}});
	std::ostringstream os;
	os << m;
	EXPECT_EQ(os.str(), "\n[\n\t1+2i\t3-4i\n]\n");
}

// ---------------------------------------------------------------------------
// Kopiowanie i przenoszenie
// ---------------------------------------------------------------------------
TYPED_TEST(MatrixTyped, NewMatrixIsZeroFilled) {
	Matrix<TypeParam> m(5, 3);
	for (int i = 1; i <= 5; ++i) {
		for (int j = 1; j <= 3; ++j) {
			EXPECT_EQ(m(i, j), TypeParam{}) << "non-zero element at (" << i << ", " << j << ")";
		}
	}
}

TYPED_TEST(MatrixTyped, SpecialMembersAreGeneratedByCompiler) {
	EXPECT_TRUE(std::is_copy_constructible_v<Matrix<TypeParam>>);
	EXPECT_TRUE(std::is_copy_assignable_v<Matrix<TypeParam>>);
	EXPECT_TRUE(std::is_nothrow_move_constructible_v<Matrix<TypeParam>>);
	EXPECT_TRUE(std::is_nothrow_move_assignable_v<Matrix<TypeParam>>);
}

TYPED_TEST(MatrixTyped, CopyConstructorMakesIndependentCopy) {
	auto a = makeMatrix<TypeParam>(2, 2, {1, 2, 3, 4});
	Matrix<TypeParam> b(a);
	b(1, 1) = TypeParam{9};
	expectMatrixEq<TypeParam>(a, 2, 2, {1, 2, 3, 4});   // oryginal bez zmian
	expectMatrixEq<TypeParam>(b, 2, 2, {9, 2, 3, 4});
}

TYPED_TEST(MatrixTyped, CopyConstructorPreservesNonSquareDimensions) {
	auto a = makeMatrix<TypeParam>(2, 3, {1, 2, 3, 4, 5, 6});
	Matrix<TypeParam> b(a);
	expectMatrixEq<TypeParam>(b, 2, 3, {1, 2, 3, 4, 5, 6});
	EXPECT_THROW(b(3, 1), IndexOutOfBoundsException);   // wierszy nadal 2
	EXPECT_THROW(b(1, 4), IndexOutOfBoundsException);   // kolumn nadal 3
}

TEST(MatrixCopy, CopyOfEmptyMatrixIsEmpty) {
	Matrix<int> a;
	Matrix<int> b(a);
	EXPECT_THROW(b(1, 1), IndexOutOfBoundsException);
}

TEST(MatrixCopy, CopyOfConstMatrix) {
	const Matrix<int> a(2, 2, 4);
	Matrix<int> b(a);
	EXPECT_EQ(b(2, 2), 4);
}

TYPED_TEST(MatrixTyped, CopyAssignmentMakesIndependentCopyAndChangesSize) {
	auto a = makeMatrix<TypeParam>(2, 2, {1, 2, 3, 4});
	Matrix<TypeParam> b(3, 3, TypeParam{7});
	b = a;
	expectMatrixEq<TypeParam>(b, 2, 2, {1, 2, 3, 4});
	EXPECT_THROW(b(3, 3), IndexOutOfBoundsException);   // stary rozmiar 3x3 nie obowiazuje
	b(2, 2) = TypeParam{0};
	expectMatrixEq<TypeParam>(a, 2, 2, {1, 2, 3, 4});   // a bez zmian
}

TYPED_TEST(MatrixTyped, SelfAssignmentKeepsData) {
	auto a = makeMatrix<TypeParam>(2, 2, {1, 2, 3, 4});
	Matrix<TypeParam>& alias = a;   // alias, zeby kompilator nie ostrzegal o a = a
	a = alias;
	expectMatrixEq<TypeParam>(a, 2, 2, {1, 2, 3, 4});
}

TYPED_TEST(MatrixTyped, MoveConstructorTransfersData) {
	auto a = makeMatrix<TypeParam>(2, 2, {1, 2, 3, 4});
	Matrix<TypeParam> b(std::move(a));
	expectMatrixEq<TypeParam>(b, 2, 2, {1, 2, 3, 4});
	// obiekt po przeniesieniu mozna ponownie uzyc po przypisaniu nowej wartosci
	a = makeMatrix<TypeParam>(1, 2, {5, 6});
	expectMatrixEq<TypeParam>(a, 1, 2, {5, 6});
}

TYPED_TEST(MatrixTyped, MoveAssignmentTransfersData) {
	auto a = makeMatrix<TypeParam>(2, 2, {1, 2, 3, 4});
	Matrix<TypeParam> b(1, 1, TypeParam{0});
	b = std::move(a);
	expectMatrixEq<TypeParam>(b, 2, 2, {1, 2, 3, 4});
	a = makeMatrix<TypeParam>(1, 2, {5, 6});
	expectMatrixEq<TypeParam>(a, 1, 2, {5, 6});
}

TYPED_TEST(MatrixTyped, SurvivesStoringInGrowingVector) {
	std::vector<Matrix<TypeParam>> matrices;
	for (int n = 1; n <= 10; ++n) {
		matrices.push_back(Matrix<TypeParam>(1, 1, TypeParam(n)));   // realokacje kopiuja/przenosza
	}
	for (int n = 1; n <= 10; ++n) {
		EXPECT_EQ(matrices[static_cast<std::size_t>(n - 1)](1, 1), TypeParam(n));
	}
}

// ---------------------------------------------------------------------------
// Walidacja wymiarow
// ---------------------------------------------------------------------------
static_assert(std::is_base_of_v<MatrixException, InvalidDimensionException>);

TEST(MatrixDimensions, NegativeRowsThrow) {
	EXPECT_THROW(Matrix<int>(-1, 3), InvalidDimensionException);
}

TEST(MatrixDimensions, NegativeColumnsThrow) {
	EXPECT_THROW(Matrix<int>(3, -1), InvalidDimensionException);
}

TEST(MatrixDimensions, NegativeRowsAndColumnsThrow) {
	EXPECT_THROW(Matrix<int>(-2, -2), InvalidDimensionException);
}

TEST(MatrixDimensions, NegativeDimensionsThrowWithInitialValueConstructor) {
	EXPECT_THROW(Matrix<int>(-1, 3, 5), InvalidDimensionException);
	EXPECT_THROW(Matrix<double>(3, -1, 5.0), InvalidDimensionException);
}

TEST(MatrixDimensions, InvalidDimensionIsCatchableAsMatrixException) {
	EXPECT_THROW(Matrix<int>(-1, 1), MatrixException);
}

TEST(MatrixDimensions, ZeroDimensionsAreAllowed) {
	EXPECT_NO_THROW(Matrix<int>(0, 0));
	EXPECT_NO_THROW(Matrix<int>(0, 5));
	EXPECT_NO_THROW(Matrix<int>(5, 0));
	EXPECT_NO_THROW(Matrix<int>(0, 5, 1));
}

TEST(MatrixDimensions, MatrixWithZeroDimensionHasNoElements) {
	Matrix<int> noRows(0, 5);
	Matrix<int> noColumns(5, 0);
	EXPECT_THROW(noRows(1, 1), IndexOutOfBoundsException);
	EXPECT_THROW(noColumns(1, 1), IndexOutOfBoundsException);
}

// ---------------------------------------------------------------------------
// Const-correctness i wyrazenia z obiektami tymczasowymi (krok 12)
// ---------------------------------------------------------------------------
TYPED_TEST(MatrixTyped, OperationsWorkOnConstMatrices) {
	const auto a = makeMatrix<TypeParam>(2, 2, {1, 2, 3, 4});
	const auto b = makeMatrix<TypeParam>(2, 2, {10, 20, 30, 40});
	expectMatrixEq<TypeParam>(a + b, 2, 2, {11, 22, 33, 44});
	expectMatrixEq<TypeParam>(b - a, 2, 2, {9, 18, 27, 36});
	expectMatrixEq<TypeParam>(a * b, 2, 2, {70, 100, 150, 220});
	expectMatrixEq<TypeParam>(a * TypeParam{2}, 2, 2, {2, 4, 6, 8});
	expectMatrixEq<TypeParam>(TypeParam{2} * a, 2, 2, {2, 4, 6, 8});
	expectMatrixEq<TypeParam>(a.transpose(), 2, 2, {1, 3, 2, 4});
	EXPECT_EQ(a.getDet(), TypeParam{-2});
}

TYPED_TEST(MatrixTyped, ConstOperationsDoNotModifyOperands) {
	const auto a = makeMatrix<TypeParam>(2, 2, {1, 2, 3, 4});
	const auto b = makeMatrix<TypeParam>(2, 2, {10, 20, 30, 40});
	static_cast<void>(a + b);
	static_cast<void>(a * b);
	static_cast<void>(a.transpose());
	expectMatrixEq<TypeParam>(a, 2, 2, {1, 2, 3, 4});
	expectMatrixEq<TypeParam>(b, 2, 2, {10, 20, 30, 40});
}

TYPED_TEST(MatrixTyped, ChainedExpressionsWithTemporaries) {
	auto a = makeMatrix<TypeParam>(2, 2, {1, 2, 3, 4});
	auto b = makeMatrix<TypeParam>(2, 2, {10, 20, 30, 40});
	auto c = makeMatrix<TypeParam>(2, 2, {100, 200, 300, 400});
	expectMatrixEq<TypeParam>(a + (b + c), 2, 2, {111, 222, 333, 444});
	expectMatrixEq<TypeParam>(a + b + c, 2, 2, {111, 222, 333, 444});
	expectMatrixEq<TypeParam>(TypeParam{2} * (a + b), 2, 2, {22, 44, 66, 88});
	expectMatrixEq<TypeParam>((a + b).transpose(), 2, 2, {11, 33, 22, 44});
	expectMatrixEq<TypeParam>(a * (b + c), 2, 2, {770, 1100, 1650, 2420});
}
