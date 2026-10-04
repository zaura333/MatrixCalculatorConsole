# MatrixCalculatorConsole

*Read this in: **English** | [Polski](README.pl.md)*

![C++17](https://img.shields.io/badge/C%2B%2B-17-blue)
[![License: GPL v3](https://img.shields.io/badge/License-GPLv3-blue.svg)](LICENSE.txt)

A C++17 matrix library: `Matrix<T>` for `int`, `double` and `std::complex<double>`, with
operators, determinant, transpose, a typed exception hierarchy, an interactive console demo and a GoogleTest
suite.

The project started as a C++14 class with hand-written memory management (Rule of Three). It was modernised to
C++17 with the Rule of Zero, `std::vector` storage, CMake, tests and sanitizer support. The
[design notes](#design-notes) explain the choices and show what the code looked like before.

## Features

- `Matrix<T>` with **1-based** indexing: `A(1, 1)` is the top-left element.
- Operations: `+`, `-`, `*` by scalar (both `2.0 * A` and `A * 2.0`), `*` by matrix, `transpose()`,
  `getDet()` (Laplace expansion, O(n!)), `size()`, `operator<<`.
- Allowed element types are enforced at compile time (`static_assert`): `int`, `double`, `std::complex<double>`.
- Exception hierarchy derived from `std::logic_error` (see [Exceptions](#exceptions)).
- Interactive console demo (menu) and a unit/smoke test suite (GoogleTest).
- Optional AddressSanitizer / UndefinedBehaviorSanitizer build.

## Quick start

`Matrix.h` is header-only; link the small `matrix` library target (it contains the exception types) and include
the header:

```cpp
#include "Matrix.h"

int main() {
    Matrix<double> A(2, 2, 1.0);          // 2x2, every element 1.0
    A(1, 2) = 3.5;                        // 1-based: row 1, column 2

    const auto B = 2.0 * A + A.transpose();
    std::cout << B << "det(B) = " << B.getDet() << '\n';

    try {
        std::cout << A * Matrix<double>(3, 3);   // 2x2 * 3x3 -> sizes do not match
    } catch (const MatrixException& e) {
        std::cerr << e.what() << '\n';
    }
}
```

```
[
        3       8
        5.5     3
]
det(B) = -35
Error: Number of rows of the first matrix must be equal to the number of columns of the second matrix.
```

## Requirements

- A C++17 compiler (developed with MSVC 2022 / v143; also built and tested with GCC)
- CMake 3.20+
- Internet access at the first configure step (GoogleTest 1.15.2 is downloaded with `FetchContent`)

## Build, run, test

All commands are run from the repository root.

```bash
# Configure + build (Debug)
cmake -S . -B build
cmake --build build --config Debug

# Run all tests
ctest --test-dir build -C Debug --output-on-failure

# Only the smoke tests (suite "Smoke")
ctest --test-dir build -C Debug -R "^Smoke\." --output-on-failure
```

Where the binaries end up depends on the generator:

| | Visual Studio (multi-config) | Makefiles / Ninja (single-config) |
|---|---|---|
| Demo | `build/Debug/MatrixCalculatorConsole.exe` | `build/MatrixCalculatorConsole` |
| Test binary | `build/tests/Debug/matrix_tests.exe` | `build/tests/matrix_tests` |

You can also run the test binary directly and filter with GoogleTest, e.g. `--gtest_filter=Matrix*`.
`-C Debug` (and `--config Debug`) is only needed with multi-config generators; Makefiles/Ninja ignore it.

In **Visual Studio** you can also open the folder (or the generated `.sln` in `build/`), choose the
`MatrixCalculatorConsole.exe` target and press Ctrl+F5.

### Sanitizers (ASan and UBSan)

AddressSanitizer (memory errors: out-of-bounds, use-after-free, leaks) and UndefinedBehaviorSanitizer
(signed overflow, invalid shifts, ...) are available with GCC and Clang through a CMake option:

```bash
cmake -S . -B build-san -DCMAKE_BUILD_TYPE=Debug -DENABLE_SANITIZERS=ON
cmake --build build-san
ctest --test-dir build-san --output-on-failure
```

Any sanitizer report makes the test fail. **MSVC** has no UBSan, so the option prints a warning and is
ignored there; run the sanitizer build on Linux or in WSL. The CI workflow runs this build on every push.

## Demo

> **Note:** the console demo is in **Polish** (menu, headings and messages), as in the original program.
> The library API, the exception messages and the code are in English. A translation of the menu:
>
> | # | Polish | English |
> |---|---|---|
> | 1 | Dodawanie macierzy | Matrix addition |
> | 2 | Odejmowanie macierzy | Matrix subtraction |
> | 3 | Mnozenie macierzy przez liczbe | Multiplying a matrix by a number |
> | 4 | Mnozenie macierzy przez macierz | Matrix multiplication |
> | 5 | Transponowanie macierzy | Transposing a matrix |
> | 6 | Kopiowanie macierzy | Copying a matrix |
> | 7 | Macierze z liczbami zespolonymi | Matrices of complex numbers |
> | 8 | Wyznacznik macierzy | Matrix determinant |
> | 9 | CTAD (wnioskowanie typu) | CTAD (type deduction) |
> | 10 | Obsluga wyjatkow | Exception handling |
> | a | Uruchom wszystkie po kolei | Run all in turn |
> | 0 | Wyjscie | Exit |

The demo is a menu; each example is self-contained, so they can be run in any order. A session that
computes a determinant (option `8`):

```
---KALKULATOR MACIERZY - PRZYKLADY---

===== KALKULATOR MACIERZY - MENU =====
  1. Dodawanie macierzy
  2. Odejmowanie macierzy
  3. Mnozenie macierzy przez liczbe
  4. Mnozenie macierzy przez macierz
  5. Transponowanie macierzy
  6. Kopiowanie macierzy
  7. Macierze z liczbami zespolonymi
  8. Wyznacznik macierzy
  9. CTAD (wnioskowanie typu)
  10. Obsluga wyjatkow
  a. Uruchom wszystkie po kolei
  0. Wyjscie
Wybor: 8

**OBLICZANIE WYZNACZNIKA MACIERZY**

M
 =

[
        3       6       1       5       7
        1       4       2       5       9
        10      7       12      30      14
        21      16      17      43      9
        20      21      18      1       24
]

Wymiary M: 5x5

det(M) = -45112
```

## Exceptions

All derive from `MatrixException`, which derives from `std::logic_error`, so a single
`catch (const MatrixException&)` handles all of them:

| Exception | Thrown when |
|---|---|
| `SizeMismatchException` | `+`, `-` or `*` with incompatible dimensions |
| `NonSquareMatrixException` | `getDet()` on a non-square matrix |
| `IndexOutOfBoundsException` | `operator()` with an index outside `1..rows` / `1..columns` |
| `InvalidDimensionException` | constructing a matrix with a negative dimension |

Zero dimensions (`0x0`, `0xN`, `Nx0`) are allowed on purpose; `getDet()` of `0x0` returns `0`.
Catch by `const` reference (`catch (const X& e)`) to avoid slicing and copies.

## Limitations

- **`getDet()` is O(n!)** (Laplace expansion). It is fine for small matrices up to around n = 10.
- **Only `int`, `double` and `std::complex<double>`** are accepted as element types; anything else fails the
  `static_assert`.
- **1-based indexing** differs from the standard containers; `operator()` is bounds-checked and throws
  `IndexOutOfBoundsException`.
- **Moved-from state:** a moved-from `Matrix` has an empty vector but unchanged `rows`/`columns`. It may only
  be destroyed or assigned to, which is the usual contract for moved-from objects.

## Project layout

```
MatrixCalculatorConsole/
  Matrix.h                    Matrix<T> (header-only template)
  MatrixExceptions.h/.cpp     exception hierarchy (the .cpp keeps the static library non-empty)
  MatrixCalculatorConsole.cpp interactive demo
tests/
  SmokeTests.cpp              a few fast "does it basically work" checks
  MatrixTests.cpp             unit tests (typed over int, double, std::complex<double>)
  ExceptionsTests.cpp         exception types and hierarchy
.github/workflows/ci.yml      GitHub Actions: Linux, Windows, sanitizers
CMakeLists.txt                library + demo + sanitizer option
```

## Design notes

### Before the modernisation

The original version was a classic hand-managed-memory class:

- storage was a **`T**`**: an array of row pointers, each row allocated separately with `new T[columns]`;
- **Rule of Three** by hand: a user-written destructor (`delete[]` in a loop), copy constructor and copy
  assignment operator; no move operations;
- raw owning pointers, **no smart pointers**, no containers;
- elements were **not zero-initialised** (`new T[n]` leaves `int`/`double` indeterminate);
- no tests, no CMake (a Visual Studio solution only).

**Approach to modernisation.** The point was to change the design without changing the behaviour by accident:

1. commit the untouched baseline and switch to C++17;
2. add CMake and GoogleTest, then write **tests for the existing behaviour first** (the safety net);
3. refactor in small steps, keeping the tests green after each one: exception hierarchy, `std::vector`
   storage, zero-initialisation and dimension validation, const-correctness and `[[nodiscard]]`, cleanup,
   then C++17 features (`if constexpr`, structured bindings, CTAD).

### Why Rule of Zero

The Rule of Three says: if a class needs a custom destructor, copy constructor or copy assignment, it
probably needs all three (Rule of Five adds the move constructor and move assignment). The Rule of Zero goes
one step further: **design the class so that it needs none of them**, by holding resources in members that
already manage themselves.

`Matrix` now owns only `int rows, columns` and a `std::vector<T> data`. Because of that:

- the compiler-generated destructor, copy and move operations are correct, so there is no leak,
  double-delete or self-assignment bug to get wrong;
- move construction and assignment are `noexcept` and cheap (one buffer is stolen); this is checked by tests
  (`std::is_nothrow_move_constructible_v`, ...);
- the class is shorter and there is nothing to keep in sync when members are added.

A Rule of Five version was considered, but it adds five hand-written functions
that only re-implement what `std::vector` already does correctly, so it was rejected.

### Why a flat `std::vector<T>` and an `index()` function

Elements are stored in **one** contiguous `std::vector<T>` in row-major order; the public 1-based `(x, y)` is
translated by one function:

```cpp
std::size_t index(int x, int y) const noexcept {
    return static_cast<std::size_t>(x - 1) * columns + (y - 1);
}
```

```
Matrix 2x3, A(row, col):         data (flat, row-major)

 A(1,1) A(1,2) A(1,3)             index:   0      1      2      3      4      5
 A(2,1) A(2,2) A(2,3)             value: A(1,1) A(1,2) A(1,3) A(2,1) A(2,2) A(2,3)

 index(x, y) = (x-1)*columns + (y-1)      e.g. A(2,2) -> (2-1)*3 + (2-1) = 4
```

Why not `std::vector<std::vector<T>>`:

- **one allocation instead of rows + 1**, and the memory is contiguous, which is friendlier to the cache;
- copying and moving touches one buffer;
- **the shape cannot become inconsistent**: with a vector of vectors every row is an independent object and
  could end up with a different length, so the class would have to defend an invariant it does not need to
  have;
- construction with a size and an initial value is a single `std::vector` constructor, so zero-initialisation
  is free.

The price is the index arithmetic, but it lives in exactly one place and is covered by tests.

### Type deduction with CTAD

C++17 can deduce the template argument of a class from its constructor (Class Template Argument Deduction):

```cpp
Matrix a(2, 3, 1.5);   // Matrix<double>
Matrix b(2, 3, 7);     // Matrix<int>
Matrix c(a);           // Matrix<double> (copy)
```

This works through the compiler's implicit deduction guides. One explicit deduction guide is added in
`Matrix.h`:

```cpp
Matrix(int, int, float) -> Matrix<double>;
```

Without it `Matrix(2, 3, 1.5f)` would deduce `Matrix<float>`, which the `static_assert` rejects. The guide
promotes `float` to `double`.

`Matrix m(2, 3)` (no initial value) is **deliberately not deducible**: there is nothing to deduce `T` from, and
silently defaulting to `double` would hide a decision that belongs to the caller. Write `Matrix<double> m(2, 3)`
instead.

### C++17 features used

`if constexpr` (complex numbers printed as `a+bi`), structured bindings (`auto [rows, cols] = m.size()`),
class template argument deduction with a deduction guide, `std::is_same_v` and other `_v` traits,
`[[nodiscard]]`, `std::optional`, `std::from_chars` and `std::array` with CTAD (demo menu).

## License

Released under the GNU General Public License v3.0. See [LICENSE.txt](LICENSE.txt).
