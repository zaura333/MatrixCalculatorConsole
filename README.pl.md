# MatrixCalculatorConsole

*Przeczytaj w: [English](README.md) | **Polski***

![C++17](https://img.shields.io/badge/C%2B%2B-17-blue)
[![License: GPL v3](https://img.shields.io/badge/License-GPLv3-blue.svg)](LICENSE.txt)

Biblioteka macierzy w C++17, w jednym nagłówku: `Matrix<T>` dla `int`, `double` i `std::complex<double>`,
z operatorami, wyznacznikiem, transpozycją, typowaną hierarchią wyjątków, interaktywnym demem konsolowym i
testami GoogleTest.

Projekt zaczął się jako klasa w C++14 z ręcznym zarządzaniem pamięcią (Rule of Three). Został zmodernizowany
do C++17: Rule of Zero, przechowywanie w `std::vector`, CMake, testy i obsługa sanitizerów.
[Notatki projektowe](#notatki-projektowe) wyjaśniają podjęte decyzje i pokazują, jak kod wyglądał wcześniej.

## Funkcje

- `Matrix<T>` z indeksowaniem **od 1**: `A(1, 1)` to lewy górny element.
- Operacje: `+`, `-`, `*` przez skalar (`2.0 * A` i `A * 2.0`), `*` przez macierz, `transpose()`,
  `getDet()` (rozwinięcie Laplace'a, O(n!)), `size()`, `operator<<`.
- Dozwolone typy elementów są sprawdzane w czasie kompilacji (`static_assert`): `int`, `double`,
  `std::complex<double>`.
- Hierarchia wyjątków dziedzicząca po `std::logic_error` (zobacz [Wyjątki](#wyjątki)).
- Interaktywne demo konsolowe (menu) oraz testy jednostkowe i smoke (GoogleTest).
- Opcjonalny build z AddressSanitizer / UndefinedBehaviorSanitizer.

## Szybki start

`Matrix.h` jest w jednym nagłówku; podlinkuj małą bibliotekę `matrix` (zawiera typy wyjątków) i dołącz
nagłówek:

```cpp
#include "Matrix.h"

int main() {
    Matrix<double> A(2, 2, 1.0);          // 2x2, każdy element 1.0
    A(1, 2) = 3.5;                        // indeksy od 1: wiersz 1, kolumna 2

    const auto B = 2.0 * A + A.transpose();
    std::cout << B << "det(B) = " << B.getDet() << '\n';

    try {
        std::cout << A * Matrix<double>(3, 3);   // 2x2 * 3x3 -> niezgodne rozmiary
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

## Wymagania

- Kompilator C++17 (rozwijane na MSVC 2022 / v143; zbudowane i przetestowane też na GCC)
- CMake 3.20+
- Dostęp do internetu przy pierwszej konfiguracji (GoogleTest 1.15.2 jest pobierany przez `FetchContent`)

## Budowanie, uruchamianie, testy

Wszystkie polecenia uruchamiaj w katalogu głównym repozytorium.

```bash
# Konfiguracja + build (Debug)
cmake -S . -B build
cmake --build build --config Debug

# Wszystkie testy
ctest --test-dir build -C Debug --output-on-failure

# Tylko testy smoke (suite "Smoke")
ctest --test-dir build -C Debug -R "^Smoke\." --output-on-failure
```

Miejsce plików wykonywalnych zależy od generatora:

| | Visual Studio (wielokonfiguracyjny) | Makefile / Ninja (jednokonfiguracyjny) |
|---|---|---|
| Demo | `build/Debug/MatrixCalculatorConsole.exe` | `build/MatrixCalculatorConsole` |
| Plik z testami | `build/tests/Debug/matrix_tests.exe` | `build/tests/matrix_tests` |

Plik z testami możesz też uruchomić bezpośrednio i filtrować przez GoogleTest, np. `--gtest_filter=Matrix*`.
`-C Debug` (oraz `--config Debug`) jest potrzebne tylko przy generatorach wielokonfiguracyjnych; Makefile/Ninja
je ignorują.

W **Visual Studio** możesz też otworzyć folder (albo wygenerowany `.sln` w `build/`), wybrać cel
`MatrixCalculatorConsole.exe` i nacisnąć Ctrl+F5.

### Sanitizery (ASan i UBSan)

AddressSanitizer (błędy pamięci: wyjście poza zakres, użycie po zwolnieniu, wycieki) i
UndefinedBehaviorSanitizer (przepełnienie liczb ze znakiem, niepoprawne przesunięcia bitowe itd.) są
dostępne z GCC i Clang przez opcję CMake:

```bash
cmake -S . -B build-san -DCMAKE_BUILD_TYPE=Debug -DENABLE_SANITIZERS=ON
cmake --build build-san
ctest --test-dir build-san --output-on-failure
```

Każdy raport sanitizera powoduje, że test się nie powiedzie. **MSVC** nie ma UBSan, więc opcja wypisuje tam
ostrzeżenie i jest ignorowana; build z sanitizerami uruchom na Linuksie lub w WSL. Workflow CI uruchamia ten
build przy każdym pushu.

## Demo

> **Uwaga:** demo konsolowe jest po **polsku** (menu, nagłówki i komunikaty), tak jak w oryginalnym programie.
> API biblioteki, komunikaty wyjątków i kod są po angielsku. Poniżej tłumaczenie menu na angielski, dla
> czytelników spoza Polski:
>
> | # | Polski | English |
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

Demo to menu; każdy przykład jest samodzielny, więc można je uruchamiać w dowolnej kolejności. Przykładowa
sesja obliczająca wyznacznik (opcja `8`):

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

## Wyjątki

Wszystkie dziedziczą po `MatrixException`, który dziedziczy po `std::logic_error`, więc jeden
`catch (const MatrixException&)` łapie wszystkie:

| Wyjątek | Kiedy jest rzucany |
|---|---|
| `SizeMismatchException` | `+`, `-` lub `*` przy niezgodnych wymiarach |
| `NonSquareMatrixException` | `getDet()` dla macierzy niekwadratowej |
| `IndexOutOfBoundsException` | `operator()` z indeksem poza `1..rows` / `1..columns` |
| `InvalidDimensionException` | tworzenie macierzy z ujemnym wymiarem |

Zerowe wymiary (`0x0`, `0xN`, `Nx0`) są celowo dozwolone; `getDet()` dla `0x0` zwraca `0`.
Łap wyjątki przez referencję do `const` (`catch (const X& e)`), żeby uniknąć slicingu i kopii.

## Ograniczenia

- **`getDet()` ma złożoność O(n!)** (rozwinięcie Laplace'a). Działa dla małych macierzy, do około `n = 10`.
- **Tylko `int`, `double` i `std::complex<double>`** są akceptowane jako typy elementów; wszystko inne nie
  przejdzie `static_assert`.
- **Indeksowanie od 1** różni się od standardowych kontenerów; `operator()` sprawdza zakres i rzuca
  `IndexOutOfBoundsException`.
- **Stan po przeniesieniu:** macierz po przeniesieniu ma pusty wektor, ale `rows`/`columns` pozostają bez
  zmian. Można ją tylko zniszczyć albo jej coś przypisać, co jest typową umową dla obiektów po przeniesieniu.

## Struktura projektu

```
MatrixCalculatorConsole/
  Matrix.h                    Matrix<T> (szablon w jednym nagłówku)
  MatrixExceptions.h/.cpp     hierarchia wyjątków (plik .cpp sprawia, że biblioteka statyczna nie jest pusta)
  MatrixCalculatorConsole.cpp interaktywne demo
tests/
  SmokeTests.cpp              kilka szybkich testów "czy to w ogóle działa"
  MatrixTests.cpp             testy jednostkowe (typowane: int, double, std::complex<double>)
  ExceptionsTests.cpp         typy wyjątków i hierarchia
.github/workflows/ci.yml      GitHub Actions: Linux, Windows, sanitizery
CMakeLists.txt                biblioteka + demo + opcja sanitizerów
```

## Notatki projektowe

### Jak było przed modernizacją

Oryginalna wersja była klasycznie napisaną klasą z ręcznym zarządzaniem pamięcią:

- dane trzymane w **`T**`**: tablicy wskaźników na wiersze, każdy wiersz alokowany osobno przez
  `new T[columns]`;
- **Rule of Three** napisane ręcznie: własny destruktor (`delete[]` w pętli), konstruktor kopiujący i
  operator przypisania kopiującego; brak operacji przenoszenia;
- surowe wskaźniki będące właścicielami, **brak smart pointerów**, brak kontenerów;
- elementy **nie były zerowane** (`new T[n]` zostawia `int`/`double` z nieokreśloną wartością);
- brak testów, brak CMake (tylko solucja Visual Studio).

**Podejście do modernizacji.** Chodziło o zmianę projektu bez przypadkowej zmiany zachowania:

1. commit nietkniętej wersji wyjściowej i przejście na C++17;
2. dodanie CMake i GoogleTest, a potem **testy istniejącego zachowania napisane najpierw** (siatka
   bezpieczeństwa);
3. refaktoryzacja małymi krokami z zielonymi testami po każdym: hierarchia wyjątków, przechowywanie w
   `std::vector`, zerowanie i walidacja wymiarów, const-correctness i `[[nodiscard]]`, sprzątanie, a na końcu
   funkcje C++17 (`if constexpr`, structured bindings, CTAD).

### Dlaczego Rule of Zero

Rule of Three mówi: jeśli klasa potrzebuje własnego destruktora, konstruktora kopiującego albo operatora
przypisania, to prawdopodobnie potrzebuje wszystkich trzech (Rule of Five dodaje konstruktor przenoszący i
przypisanie przenoszące). Rule of Zero idzie krok dalej: **zaprojektuj klasę tak, żeby nie potrzebowała żadnej
z tych funkcji**, trzymając zasoby w składowych, które same się nimi zajmują.

`Matrix` ma teraz tylko `int rows, columns` i `std::vector<T> data`. Dzięki temu:

- destruktor oraz operacje kopiowania i przenoszenia wygenerowane przez kompilator są poprawne, więc nie ma
  wycieku, podwójnego `delete` ani błędu przy samoprzypisaniu, który można by popełnić;
- przenoszenie jest `noexcept` i tanie (przejmowany jest jeden bufor); sprawdzają to testy
  (`std::is_nothrow_move_constructible_v` itd.);
- klasa jest krótsza i nie trzeba niczego synchronizować przy dodawaniu składowych.

Wersja Rule of Five była rozważana, ale dodaje pięć ręcznie pisanych funkcji, które tylko powtarzają to, co
`std::vector` już robi poprawnie, więc została odrzucona.

### Dlaczego płaski `std::vector<T>` i funkcja `index()`

Elementy leżą w **jednym** ciągłym `std::vector<T>` wierszami (row-major); publiczne indeksy `(x, y)` liczone
od 1 są tłumaczone przez jedną funkcję:

```cpp
std::size_t index(int x, int y) const noexcept {
    return static_cast<std::size_t>(x - 1) * columns + (y - 1);
}
```

```
Macierz 2x3, A(wiersz, kolumna):  data (płasko, wiersz po wierszu)

 A(1,1) A(1,2) A(1,3)              indeks:  0      1      2      3      4      5
 A(2,1) A(2,2) A(2,3)              wartość: A(1,1) A(1,2) A(1,3) A(2,1) A(2,2) A(2,3)

 index(x, y) = (x-1)*columns + (y-1)       np. A(2,2) -> (2-1)*3 + (2-1) = 4
```

Dlaczego nie `std::vector<std::vector<T>>`:

- **jedna alokacja zamiast wiersze + 1**, a pamięć jest ciągła, co jest lepsze dla cache;
- kopiowanie i przenoszenie dotyczy jednego bufora;
- **kształt nie może się rozjechać**: przy wektorze wektorów każdy wiersz to osobny obiekt i mógłby mieć inną
  długość, więc klasa musiałaby pilnować niezmiennika, którego w ogóle nie musi mieć;
- utworzenie macierzy z rozmiarem i wartością początkową to jeden konstruktor `std::vector`, więc zerowanie
  dostajemy za darmo.

Ceną jest arytmetyka indeksów, ale mieści się w jednym miejscu i jest pokryta testami.

### Wnioskowanie typu przez CTAD

C++17 potrafi wywnioskować argument szablonu klasy z konstruktora (Class Template Argument Deduction):

```cpp
Matrix a(2, 3, 1.5);   // Matrix<double>
Matrix b(2, 3, 7);     // Matrix<int>
Matrix c(a);           // Matrix<double> (kopia)
```

Działa to dzięki niejawnym deduction guides tworzonym przez kompilator. W `Matrix.h` dodany jest jeden jawny
deduction guide:

```cpp
Matrix(int, int, float) -> Matrix<double>;
```

Bez niego `Matrix(2, 3, 1.5f)` wywnioskowałoby `Matrix<float>`, który `static_assert` odrzuca. Przewodnik
promuje `float` do `double`.

`Matrix m(2, 3)` (bez wartości początkowej) jest **celowo niewnioskowalne**: nie ma z czego wywnioskować `T`, a
ciche przyjęcie `double` ukrywałoby decyzję, która należy do wywołującego. Zamiast tego napisz
`Matrix<double> m(2, 3)`.

### Użyte funkcje C++17

`if constexpr` (liczby zespolone wypisywane jako `a+bi`), structured bindings (`auto [rows, cols] = m.size()`),
wnioskowanie argumentów szablonu klasy z deduction guide, `std::is_same_v` i inne cechy `_v`, `[[nodiscard]]`,
`std::optional`, `std::from_chars` oraz `std::array` z CTAD (menu dema).

## Licencja

Projekt jest udostępniany na licencji GNU General Public License v3.0. Zobacz [LICENSE.txt](LICENSE.txt).
