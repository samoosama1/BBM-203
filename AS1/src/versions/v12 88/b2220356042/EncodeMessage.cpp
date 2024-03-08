#include "EncodeMessage.h"
#include <cmath>


int fib(int pos);
bool isPrime(int n);
// Default Constructor
EncodeMessage::EncodeMessage() {

}

// Destructor
EncodeMessage::~EncodeMessage() {
    
}

// Function to encode a message into an image matrix
ImageMatrix EncodeMessage::encodeMessageToImage(const ImageMatrix &img, const std::string &message, const std::vector<std::pair<int, int>>& positions) {
    std::string newStr{};
    std::string end{};
    std::string front{};
    double** encodeMatrix = new double * [img.get_height()];
    for (int i = 0; i < img.get_height(); ++i) {
        encodeMatrix[i] = new double [img.get_width()];
        for (int j = 0; j < img.get_width(); ++j) {
            encodeMatrix[i][j] = img.get_data(i, j);
        }
    }

    int charVal{};
    for (int i = 0; i < message.size(); ++i) {
        if(!isPrime(i)) {
            newStr.push_back(message.at(i));
        } else {
            charVal = ((int) message.at(i)) + fib(i);
            if(charVal <= 32)
                charVal += 33;
            else if(charVal >= 127)
                charVal = 126;
            newStr.push_back((char) charVal);
        }
    }

    int size = newStr.size() / 2;
    front = newStr.substr(0, newStr.size() - size);
    end = newStr.substr(newStr.size() - size, size);
    end += front;

    int row;
    int col;
    int pixelVal;
    for (int i = 0; i < end.size(); ++i) {
        row = positions[i].first;
        col = positions[i].second;
        pixelVal = (int) img.get_data(row, col);
        encodeMatrix[row][col] = pixelVal + ((int) end.at(i) % 2);
    }

    ImageMatrix result{const_cast<const double** >(encodeMatrix), img.get_height(), img.get_width()};

    for (int i = 0; i < img.get_height(); ++i) {
        delete[] encodeMatrix[i];
    }
    delete[] encodeMatrix;

    return result;
}

int fib(int pos) {
    if(pos == 1)
        return 0;
    if(pos == 2)
        return 1;
    return fib(pos - 1) + fib(pos - 2);
}

bool isPrime(int n) {
    if(n == 0)
        return false;
    if(n == 1)
        return false;
    if(n == 2)
        return true;
    for (int i = 2; i < std::sqrt(n); ++i) {
        if(n % i == 0)
            return false;
    }
    return true;
}
