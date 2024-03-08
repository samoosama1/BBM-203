#include <iostream>

class Counter {
public:
    Counter();
    Counter(int value);
    Counter(const Counter& other);
    ~Counter();
    void operator++(int);
    friend std::ostream& operator<<(std::ostream& os, const Counter& obj);
private:
    int* count;
};
