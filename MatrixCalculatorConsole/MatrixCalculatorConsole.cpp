#include <array>
#include <charconv>
#include <complex>
#include <cstddef>
#include <iostream>
#include <optional>
#include <string>
#include <system_error>
#include "Matrix.h"
#include "MatrixExceptions.h"

namespace {

// ---------------------------------------------------------------------------
// Dane przykladowe
// ---------------------------------------------------------------------------
Matrix<double> makeA()
{
    Matrix<double> A(2, 3);
    const double values[] = { 3, 6, 1,
                              1, 4, 2 };
    std::size_t k = 0;
    for (int i = 1; i <= 2; ++i) {
        for (int j = 1; j <= 3; ++j) {
            A(i, j) = values[k++];
        }
    }
    return A;
}

Matrix<int> makeM()
{
    Matrix<int> M(5, 5);
    const int values[] = {  3,  6,  1,  5,  7,
                            1,  4,  2,  5,  9,
                           10,  7, 12, 30, 14,
                           21, 16, 17, 43,  9,
                           20, 21, 18,  1, 24 };
    std::size_t k = 0;
    for (int i = 1; i <= 5; ++i) {
        for (int j = 1; j <= 5; ++j) {
            M(i, j) = values[k++];
        }
    }
    return M;
}

// ---------------------------------------------------------------------------
// Przyklady (mozna uruchamiac w dowolnej kolejnosci)
// ---------------------------------------------------------------------------
void demoAddition()
{
    std::cout << "\n**DODAWANIE MACIERZY**\n";
    const Matrix<double> A = makeA();
    const Matrix<double> B(2, 3, 5.2);
    std::cout << "Macierz A (2x3):\nA\n=\n" << A << '\n';
    std::cout << "Macierz B (2x3) zainicjalizowana wartoscia 5.2:\nB\n=\n" << B << '\n';

    try {
        const Matrix<double> Added = A + B;
        std::cout << "\nA + B\n = \n" << Added << '\n';
    }
    catch (const SizeMismatchException& e) {
        std::cout << "\nBlad podczas dodawania macierzy:\n" << e.what() << '\n';
    }
}

void demoSubtraction()
{
    std::cout << "\n**ODEJMOWANIE MACIERZY**\n";
    const Matrix<double> A = makeA();
    const Matrix<double> B(2, 3, 5.2);
    std::cout << "Macierz A (2x3):\nA\n=\n" << A << '\n';
    std::cout << "Macierz B (2x3) zainicjalizowana wartoscia 5.2:\nB\n=\n" << B << '\n';

    try {
        const Matrix<double> Subtracted = A - B;
        std::cout << "\nA - B\n = \n" << Subtracted << '\n';
    }
    catch (const SizeMismatchException& e) {
        std::cout << "\nBlad podczas odejmowania macierzy:\n" << e.what() << '\n';
    }
}

void demoScalarMultiplication()
{
    std::cout << "\n**MNOZENIE MACIERZY PRZEZ LICZBE**\n";
    const Matrix<double> A = makeA();
    std::cout << "Macierz A (2x3):\nA\n=\n" << A << '\n';

    const Matrix<double> NumMultiplied = 4.3 * A;
    std::cout << "\n4.3 * A\n = \n" << NumMultiplied << '\n';
}

void demoMatrixMultiplication()
{
    std::cout << "\n**MNOZENIE MACIERZY PRZEZ MACIERZ**\n";
    const Matrix<double> A = makeA();
    const Matrix<double> C(3, 5, 2);
    std::cout << "Macierz A (2x3):\nA\n=\n" << A << '\n';
    std::cout << "Macierz C (3x5) zainicjalizowana wartoscia 2:\nC\n=\n" << C << '\n';

    try {
        const Matrix<double> MtMultiplied = A * C;
        std::cout << "\nA * C\n = \n" << MtMultiplied << '\n';
    }
    catch (const SizeMismatchException& e) {
        std::cout << "\nBlad podczas mnozenia macierzy:\n" << e.what() << '\n';
    }
}

void demoTranspose()
{
    std::cout << "\n**TRANSPONOWANIE MACIERZY**\n";
    const Matrix<double> A = makeA();
    std::cout << "Macierz A (2x3):\nA\n=\n" << A << '\n';
    std::cout << "\nTransponowana macierz A:\n" << A.transpose();
}

void demoCopy()
{
    std::cout << "\n**KOPIOWANIE MACIERZY KONSTRUKTOREM KOPIUJACYM**\n";
    const Matrix<double> TransposedA = makeA().transpose();
    const Matrix<double> CopiedA(TransposedA);
    std::cout << "\nSkopiowana transponowana macierz A:\n" << CopiedA;
}

void demoComplex()
{
    std::cout << "\n**PRZYKLAD MACIERZY Z LICZBAMI ZESPOLONYMI**\n";

    const std::complex<double> c1(4.5, 5.0);
    const Matrix<std::complex<double>> Zespolone(2, 3, c1);
    std::cout << "\nMacierz Zespolone = " << Zespolone << '\n';

    const Matrix<std::complex<double>> Zespolone2 = Zespolone;
    const Matrix<std::complex<double>> Zespolone3 = Zespolone + Zespolone2;
    std::cout << "\nZespolone + Zespolone2\n = \n" << Zespolone3 << '\n';
}

void demoDeterminant()
{
    std::cout << "\n**OBLICZANIE WYZNACZNIKA MACIERZY**\n";
    const Matrix<int> M = makeM();
    std::cout << "\nM\n = \n" << M << '\n';

    const auto [mRows, mCols] = M.size();
    std::cout << "Wymiary M: " << mRows << "x" << mCols << '\n';

    try {
        std::cout << "\ndet(M) = " << M.getDet() << '\n';
    }
    catch (const NonSquareMatrixException& e) {
        std::cout << "\nBlad podczas obliczania wyznacznika. Macierz nie jest kwadratowa:\n"
                  << e.what() << '\n';
    }
}

void demoDeduction()
{
    std::cout << "\n**CTAD - TYP ELEMENTOW WYWNIOSKOWANY PRZEZ KOMPILATOR**\n";
    const Matrix Wnioskowana(2, 2, 1.5);   // Matrix<double>
    const Matrix Calkowita(2, 2, 7);       // Matrix<int>
    std::cout << "\nMatrix(2, 2, 1.5):\n" << Wnioskowana
              << "\nMatrix(2, 2, 7):\n" << Calkowita << '\n';
}

void demoExceptions()
{
    std::cout << "\n**OBSLUGA WYJATKOW**\n";
    const Matrix<double> A = makeA();
    const Matrix<double> C(3, 5, 2);

    try {
        static_cast<void>(A + C);
    }
    catch (const SizeMismatchException& e) {
        std::cout << "\nA + C (2x3 + 3x5): " << e.what() << '\n';
    }

    try {
        static_cast<void>(A(3, 1));
    }
    catch (const IndexOutOfBoundsException& e) {
        std::cout << "\nA(3, 1): " << e.what() << '\n';
    }

    try {
        Matrix<double> bad(-1, 2);
    }
    catch (const InvalidDimensionException& e) {
        std::cout << "\nMatrix(-1, 2): " << e.what() << '\n';
    }

    // Wszystkie wyjatki macierzy dziedzicza po MatrixException, wiec mozna je zlapac razem:
    try {
        static_cast<void>(A.getDet());
    }
    catch (const MatrixException& e) {
        std::cout << "\nA.getDet() (macierz 2x3, przechwycona jako MatrixException): " << e.what() << '\n';
    }
}

// ---------------------------------------------------------------------------
// Menu
// ---------------------------------------------------------------------------
struct Demo {
    const char* name;
    void (*run)();
};

constexpr std::array demos{
    Demo{ "Dodawanie macierzy",               demoAddition },
    Demo{ "Odejmowanie macierzy",             demoSubtraction },
    Demo{ "Mnozenie macierzy przez liczbe",   demoScalarMultiplication },
    Demo{ "Mnozenie macierzy przez macierz",  demoMatrixMultiplication },
    Demo{ "Transponowanie macierzy",          demoTranspose },
    Demo{ "Kopiowanie macierzy",              demoCopy },
    Demo{ "Macierze z liczbami zespolonymi",  demoComplex },
    Demo{ "Wyznacznik macierzy",              demoDeterminant },
    Demo{ "CTAD (wnioskowanie typu)",         demoDeduction },
    Demo{ "Obsluga wyjatkow",                 demoExceptions },
};

void printMenu()
{
    std::cout << "\n===== KALKULATOR MACIERZY - MENU =====\n";
    for (std::size_t i = 0; i < demos.size(); ++i) {
        const auto& [name, run] = demos[i];
        std::cout << "  " << (i + 1) << ". " << name << '\n';
    }
    std::cout << "  a. Uruchom wszystkie po kolei\n"
              << "  0. Wyjscie\n"
              << "Wybor: ";
}

void runAll()
{
    for (const auto& demo : demos) {
        demo.run();
    }
}

std::optional<std::size_t> parseChoice(std::string text)
{
    const char* whitespace = " \t\r\n";
    text.erase(text.find_last_not_of(whitespace) + 1);
    text.erase(0, text.find_first_not_of(whitespace));

    std::size_t value = 0;
    const char* first = text.data();
    const char* last = first + text.size();
    const auto [ptr, ec] = std::from_chars(first, last, value);
    if (ec != std::errc{} || ptr != last) {
        return std::nullopt;
    }
    return value;
}

} // namespace

int main()
{
    std::cout << "---KALKULATOR MACIERZY - PRZYKLADY---\n";

    std::string line;
    while (true) {
        printMenu();
        if (!std::getline(std::cin, line)) {
            std::cout << '\n';
            break;   // koniec wejscia (EOF)
        }

        if (line == "a" || line == "A") {
            runAll();
            continue;
        }

        const auto choice = parseChoice(line);
        if (!choice) {
            std::cout << "Nieprawidlowy wybor. Podaj numer z menu, 'a' lub 0.\n";
        }
        else if (*choice == 0) {
            break;
        }
        else if (*choice <= demos.size()) {
            demos[*choice - 1].run();
        }
        else {
            std::cout << "Nie ma takiej pozycji w menu.\n";
        }
    }

    std::cout << "Do widzenia!\n";
    return 0;
}
