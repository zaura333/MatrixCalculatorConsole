#include <iostream>
#include "Matrix.h"
#include "MatrixExceptions.h"
#include <complex>

int main()
{
    std::cout << "---KALKULATOR MACIERZY - PRZYKLADY---\n\n";

    // Tworzenie macierzy
    std::cout << "\nTworzenie macierzy o jednakowych wymiarach w celu prezentacji dzialan dodawania i odejmowania.\n";
    std::cout << "Tworzenie macierzy A (2x3)...\n";
    Matrix<double> A(2, 3);
    std::cout << "Wypelnianie macierzy poprzez modyfikacje poszczegolnych elementow, np. A(1, 1) = 3...\n";
    try {
        A(1, 1) = 3;
        A(1, 2) = 6;
        A(1, 3) = 1;
        A(2, 1) = 1;
        A(2, 2) = 4;
        A(2, 3) = 2;
    }
    catch (std::out_of_range e) {
        std::cout << "Blad podczas dostepu do elemetnow macierzy:\n" << e.what();
        return -1;
    }
    std::cout << "Uzupelniona macierz A:\nA\n=\n" << A << '\n';

    std::cout << "\nTworzenie macierzy B (2x3) zainicjalizowej wartoscia 5.2...\n";
    Matrix<double> B(2, 3, 5.2);
    std::cout << "Uzupelniona macierz B:\nB\n=\n" << B << '\n';

    std::cout << "\n**DODAWANIE MACIERZY**";
    Matrix<double> Added;
    try {
        Added = A + B;
    }
    catch (Matrix_size_not_match e) {
        std::cout << "\nBlad podczas dodawania macierzy:\n" << e.what();
        return -1;
    }

    std::cout << "\nA + B\n = \n" << Added << '\n';

    std::cout << "\n**ODEJMOWANIE MACIERZY**";

    Matrix<double> Subtracted;
    try {
        Subtracted = A - B;
    }
    catch (Matrix_size_not_match e) {
        std::cout << "\nBlad podczas odejmowania macierzy:\n" << e.what();
        return -1;
    }
    std::cout << "\nA - B\n = \n" << Subtracted << '\n';
    

    std::cout << "\n**MNOZENIE MACIERZY PRZEZ LICZBE**";
    Matrix<double> NumMultiplied = 4.3 * A;
    std::cout << "\n4.3 * A\n = \n" << NumMultiplied << '\n';

    std::cout << "\nTworzenie macierzy o odpowiednich wymiarach by pomnozyc ja z macierza A...";
    std::cout << "\nTworzenie macierzy C (3, 5) zainicjalizowej wartoscia 2...\n";
    Matrix<double> C(3, 5, 2);
    std::cout << "Uzupelniona macierz C:\nC\n=\n" << C << '\n';

    std::cout << "\n**MNOZENIE MACIERZY PRZEZ MACIERZ**";
    Matrix<double> MtMultiplied;
    try {
        MtMultiplied = A * C;
    }
    catch (Matrix_size_not_match e) {
        std::cout << "\nBlad podczas mnozenia macierzy:\n" << e.what();
        return -1;
    }
    std::cout << "\nA * C\n = \n" << MtMultiplied << '\n';

    std::cout << "\n**TRANSPONOWANIE MACIERZY**";
    Matrix<double> TransposedA = A.transpose();
    std::cout << "\nTransponowana macierz A:\n" << TransposedA;

    std::cout << "\n**KOPIOWANIE MACIERZY KONSTRUKTOREM KOPIUJĄCYM**";
    Matrix<double> CopiedA(TransposedA);
    std::cout << "\nSkopiowana transponowana macierz A:\n" << CopiedA;

    //WYWOŁA BŁĄD, bo dozwolone typy to int, double i complex:
    //Matrix<char> CharMt(2, 4, 'a');
    //std::cout << CharMt;

    std::cout << "\n**PRZYKŁAD MACIERZY Z LICZBAMI ZESPOLONYMI**";
    std::complex<double> c1(4.5, 5.0);
    Matrix<std::complex<double>> Zespolone(2, 3, c1);
    std::cout << "\nMacierz Zespolone = " << Zespolone << '\n';

    Matrix<std::complex<double>> Zespolone2 = Zespolone;
    Matrix<std::complex<double>> Zespolone3 = Zespolone + Zespolone2;
    std::cout << "\nZespolone + Zespolone2\n = \n" << Zespolone3 << '\n';

    std::cout << "\n**OBLICZANIE WYZNACZNIKA MACIERZY**";
    Matrix<int> M(5, 5);
    try {
        M(1, 1) = 3;
        M(1, 2) = 6;
        M(1, 3) = 1;
        M(1, 4) = 5;
        M(1, 5) = 7;

        M(2, 1) = 1;
        M(2, 2) = 4;
        M(2, 3) = 2;
        M(2, 4) = 5;
        M(2, 5) = 9;

        M(3, 1) = 10;
        M(3, 2) = 7;
        M(3, 3) = 12;
        M(3, 4) = 30;
        M(3, 5) = 14;

        M(4, 1) = 21;
        M(4, 2) = 16;
        M(4, 3) = 17;
        M(4, 4) = 43;
        M(4, 5) = 9;

        M(5, 1) = 20;
        M(5, 2) = 21;
        M(5, 3) = 18;
        M(5, 4) = 1;
        M(5, 5) = 24;

    }
    catch (std::out_of_range e) {
        std::cout << "Blad podczas dostepu do elemetnow macierzy:\n" << e.what();
        return -1;
    }

    std::cout << "\nM\n = \n" << M << '\n';
    int wyznacznik;

    try {
        wyznacznik = M.getDet();
    }
    catch (Matrix_size_not_match e) {
        std::cout << "\nBlad podczas obliczania wyznacznika. Macierz nie jest kwadratowa:\n" << e.what();
        return -1;
    }
    std::cout << "\ndet(M) = " << wyznacznik << '\n';

    return 0;
}
