#include <iostream>
#include "Matrix.h"

int main(int argc, char* argv[])
{
    /*QApplication a(argc, argv);
    MatrixCalculator w;
    w.show();
    return a.exec();*/

    Matrix Test(4, 5, 2);
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 5; j++) {
            std::cout << Test(i, j);
        }
        std::cout << "\n";
    }


    std::cin;

    return 0;
}
