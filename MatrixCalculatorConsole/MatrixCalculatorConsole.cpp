#include <iostream>
#include "Matrix.h"

int main()
{
    Matrix Test2(4,5,3);
    Matrix Test(4, 5, 2);
    std::cout << Test(1, 4);
    Test(3, 2) = 4;
    std::cout << Test;
    Matrix Test4(Test2);
    Matrix Test3 = Test + Test2;
    std::cout << Test3;

    return 0;
}
