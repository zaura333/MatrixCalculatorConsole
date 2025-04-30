#include <iostream>
#include "Matrix.h"

int main()
{
    Matrix Test2(4,5,8932);
    Matrix Test(4, 5, 7390);
    std::cout << Test(1, 4);
    Test(3, 2) = 4;
    std::cout << Test;
    Matrix Test4(Test2);
    Matrix Test3 = Test + Test2;
    std::cout << Test3;
    Matrix Test5 = Test2 - Test;
    std::cout << Test5;

    std::cout << "\nMnozenie macierzy: ";
    Matrix Skladnik1(2, 3);
    Skladnik1(1, 1) = 3;
    Skladnik1(1, 2) = 6;
    Skladnik1(1, 3) = 1;
    Skladnik1(2, 1) = 1;
    Skladnik1(2, 2) = 4;
    Skladnik1(2, 3) = 2;
    std::cout << Skladnik1 << '\n';

    std::cout << " * \n";

    Matrix Skladnik2(3, 2);
    Skladnik2(1, 1) = 1;
    Skladnik2(1, 2) = 5;
    Skladnik2(2, 1) = 2;
    Skladnik2(2, 2) = 4;
    Skladnik2(3, 1) = 1;
    Skladnik2(3, 2) = 3;
    std::cout << Skladnik2;

    Matrix Iloczyn = Skladnik1 * Skladnik2;
    std::cout << "\n=\n" << Iloczyn;

    std::cout << "\nMnozenie macierzy przez liczbe: ";
    Matrix Iloczyn2 = 2 * Iloczyn;

    std::cout << "\n" << Iloczyn2;

    return 0;
}
