#include <iostream>
#include "Matrix.h"

int main()
{
    std::cout << "---KALKULATOR MACIERZY - PRZYKLADY---\n\n";

    // Tworzenie macierzy
    std::cout << "\nTworzenie macierzy o jednakowych wymiarach w celu prezentacji dzialan dodawania i odejmowania.\n";
    std::cout << "Tworzenie macierzy A (2x3)...\n";
    Matrix A(2, 3);
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
    Matrix B(2, 3, 5.2);
    std::cout << "Uzupelniona macierz B:\nB\n=\n" << B << '\n';

    std::cout << "\n**DODAWANIE MACIERZY**";
    Matrix Added;
    try {
        Added = A + B;
    }
    catch (std::invalid_argument e) {
        std::cout << "\nBlad podczas dodawania macierzy:\n" << e.what();
        return -1;
    }

    std::cout << "\nA + B\n = \n" << Added << '\n';

    std::cout << "\n**ODEJMOWANIE MACIERZY**";

    Matrix Subtracted;
    try {
        Subtracted = A - B;
    }
    catch (std::invalid_argument e) {
        std::cout << "\nBlad podczas odejmowania macierzy:\n" << e.what();
        return -1;
    }
    std::cout << "\nA - B\n = \n" << Subtracted << '\n';
    

    std::cout << "\n**MNOZENIE MACIERZY PRZEZ LICZBE**";
    Matrix NumMultiplied = 4.3 * A;
    std::cout << "\n4.3 * A\n = \n" << NumMultiplied << '\n';

    std::cout << "\nTworzenie macierzy o odpowiednich wymiarach by pomnozyc ja z macierza A...";
    std::cout << "\nTworzenie macierzy C (3, 5) zainicjalizowej wartoscia 2...\n";
    Matrix C(3, 5, 2);
    std::cout << "Uzupelniona macierz C:\nC\n=\n" << C << '\n';

    std::cout << "\n**MNOZENIE MACIERZY PRZEZ MACIERZ**";
    Matrix MtMultiplied;
    try {
        MtMultiplied = A * C;
    }
    catch (std::invalid_argument e) {
        std::cout << "\nBlad podczas mnozenia macierzy:\n" << e.what();
        return -1;
    }
    std::cout << "\nA * C\n = \n" << MtMultiplied << '\n';

    std::cout << "\n**TRANSPONOWANIE MACIERZY**";
    Matrix TransposedA = A.transpose();
    std::cout << "\nTransponowana macierz A:\n" << TransposedA;

    return 0;
}
