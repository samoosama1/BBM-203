// DecodeMessage.cpp

#include "DecodeMessage.h"
#include <cmath>
#include <iostream>

// Default constructor
DecodeMessage::DecodeMessage() {
    // Nothing specific to initialize here
}

// Destructor
DecodeMessage::~DecodeMessage() {
    // Nothing specific to clean up
}


std::string DecodeMessage::decodeFromImage(const ImageMatrix& image, const std::vector<std::pair<int, int>>& edgePixels) {
    std::string message{};
    int charSize = 7;
    int row;
    int col;
    int charVal;
    char ch;
    std::vector<int> lsb{};

    for (const auto & edgePixel : edgePixels) {
        row = edgePixel.first;
        col = edgePixel.second;
        lsb.push_back(((int) image.get_data(row, col)) % 2);
    }

    int remainder = lsb.size() % 7;

    if(remainder != 0) {
        for (int i = 0; i < 7 - remainder; ++i) {
            lsb.insert(lsb.begin(), 0);
        }
    }

    for (int i = 0; i < lsb.size(); ++i) {
        std::cout << lsb[i] << " ";
    }

    for (int i = 0; i < lsb.size(); i += charSize) {
        charVal = 0;
        for (int j = 0; j < charSize; ++j) {
            charVal += lsb.at(i + j) * ((int) std::pow(2, 6 - j));
        }
        if(charVal <= 32)
            charVal += 33;
        else if (charVal >= 127)
            charVal = 126;
        ch = (char) charVal;
        message.push_back(ch);
    }
    return message;
}

