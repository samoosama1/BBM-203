#include "EncodeMessage.h"
#include <cmath>
#include <iostream>
#include <iomanip>


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
    std::vector<int> binaryStr{};
    int binaryArr[7]{};

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

    int lsb;
    for (int i = 0; i < end.size(); ++i) {
        charVal = (int) end.at(i);
        for (int j = 0; j < 7; ++j) {
            lsb = charVal % 2;
            binaryStr.push_back(lsb);
            charVal /= 2;
        }
    }

    if (binaryStr.size() > positions.size())
        binaryStr.resize(positions.size());

    int row;
    int col;
    int pixelVal;
    for (int i = 0; i < binaryStr.size(); ++i) {
        row = positions[i].first;
        col = positions[i].second;
        lsb = binaryStr.at(i);
        pixelVal = (int) encodeMatrix[row][col];
        if (lsb != pixelVal % 2) {
            if(pixelVal % 2 == 1)
                pixelVal -= 1;
            else
                pixelVal += 1;
        }
        encodeMatrix[row][col] = pixelVal;
    }

    ImageMatrix result{const_cast<const double**>(encodeMatrix), img.get_height(), img.get_width()};

    for (int i = 0; i < img.get_height(); ++i) {
        delete[] encodeMatrix[i];
    }
    delete[] encodeMatrix;

    return result;
}

int fib(int pos) {
    if(pos == 0)
        return 0;
    if(pos == 1)
        return 1;
    return fib(pos - 1) + fib(pos - 2);
}

bool isPrime(int n) {
    if(n == 0)
        return false;
    if(n == 1)
        return false;
    if(n == 2) {
        return true;
    }
    for (int i = 2; i <= (int) (std::sqrt(n)); ++i) {
        if(n % i == 0)
            return false;
    }
    return true;
}
