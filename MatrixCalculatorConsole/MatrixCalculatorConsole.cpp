#include <iostream>
#include "Matrix.h"

int main()
{
    std::cout << "---KALKULATOR MACIERZY - PRZYKLADY---\n\n";

    // Tworzenie macierzy
    std::cout << "Tworzenie macierzy o jednakowych wymiarach w celu prezentacji dzialan dodawania i odejmowania.\n";
    std::cout << "Tworzenie macierzy A (3x2)...\n";
    Matrix A(3, 2);
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

    std::cout << "Tworzenie macierzy B (2x4) zainicjalizowej wartoscia 5.2...\n";
    Matrix B(3, 2, 5.2);
    std::cout << "Uzupelniona macierz B:\nB\n=\n" << B << '\n';

    std::cout << "Dodawanie macierzy A i B:";
    Matrix Added;
    try {
        Matrix Added = A + B;
    }
    catch (std::invalid_argument e) {
        std::cout << "Blad podczas dodawania macierzy:\n" << e.what();
        return -1;
    }

    std::cout << "\nA + B\n = \n" << Added;

    std::cout << "Odejmowanie macierzy B od A:";

    Matrix Subtracted;
    try {
        Matrix Subtracted = A - B;
    }
    catch (std::invalid_argument e) {
        std::cout << "Blad podczas odejmowania macierzy:\n" << e.what();
        return -1;
    }
    std::cout << "\nA - B\n = \n" << Subtracted;
    

    std::cout << "\nMnozenie macierzy przez liczbe: ";

    std::cout << "\nTransponowanie macierzy: ";

    return 0;
}
