#include <iostream>
#include "Counter.h"

int main() {
    Counter c1{10};
    Counter c2;
    Counter c3 = c1;

    std::cout << "Address of c1: " << &c1 << "\t" << "Value of c1: " << c1 << "\n";
    std::cout << "Address of c2: " << &c2 << "\t" << "Value of c2: " << c2 << "\n";
    std::cout << "Address of c3: " << &c3 << "\t" << "Value of c3: " << c3 << "\n";

    c1++;
    c2++;
    c3++;

    std::cout << "Values after incrementation" << "\n";
    std::cout << "Value of c1: " << c1 << "\n";
    std::cout << "Value of c2: " << c2 << "\n";
    std::cout << "Value of c3: " << c3 << "\n";
}
