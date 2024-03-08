#include <iostream>
#include "Counter.h"

Counter::Counter() {
    count = new int{0};
}

Counter::Counter(int value) {
    count = new int{value};
}

Counter::Counter(const Counter& other) {
    std::cout << "Copy constructor called, address of copy: " << this << "\n";
    count = new int{*(other.count)};
}

Counter::~Counter() {
    std::cout << "Destructing the integer in heap\n";
    delete count;
}

void Counter::operator++(int) {
    (*count)++;
}

std::ostream& operator<<(std::ostream& os, const Counter& obj) {
    os << *(obj.count);
    return os;
}

