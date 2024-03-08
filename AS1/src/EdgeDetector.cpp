// EdgeDetector.cpp

#include "EdgeDetector.h"
#include <cmath>
#include <iostream>

// Default constructor
EdgeDetector::EdgeDetector() {
    sobOpHeight = 3;
    sobOpWidth = 3;
}

// Destructor
EdgeDetector::~EdgeDetector() = default;

// Detect Edges using the given algorithm
std::vector<std::pair<int, int>> EdgeDetector::detectEdges(const ImageMatrix& input_image) {
    double** sobelGx = new double * [sobOpHeight];
    sobelGx[0] = new double [sobOpWidth]{-1, 0, 1};
    sobelGx[1] = new double [sobOpWidth]{-2, 0, 2};
    sobelGx[2] = new double [sobOpWidth]{-1, 0, 1};

    double** sobelGy = new double * [sobOpHeight];
    sobelGy[0] = new double [sobOpWidth]{-1, -2, -1};
    sobelGy[1] = new double [sobOpWidth]{0, 0, 0};
    sobelGy[2] = new double [sobOpWidth]{1, 2, 1};

    Convolution convX{sobelGx, sobOpHeight, sobOpWidth, 1, true};
    Convolution convY{sobelGy, sobOpHeight, sobOpWidth, 1, true};

    ImageMatrix imgX = convX.convolve(input_image);
    ImageMatrix imgY = convY.convolve(input_image);

    int gradHeight = imgX.get_height();
    int gradWidth = imgX.get_width();

    double** gradient = new double * [gradHeight];

    for (int i = 0; i < gradHeight; ++i) {
        gradient[i] = new double [gradWidth];
    }

    double threshold;
    double val;
    double sum = 0;

    for (int i = 0; i < gradHeight; ++i) {
        for (int j = 0; j < gradWidth; ++j) {
            val = std::sqrt(std::pow(imgX.get_data(i,j), 2) + std::pow(imgY.get_data(i,j), 2));
            sum += val;
            gradient[i][j] = val;
        }
    }

    threshold = sum / (gradHeight * gradWidth);
    std::vector<std::pair<int, int>> edgePixels;
    std::pair<int, int> pair;

    for (int i = 0; i < gradHeight; ++i) {
        for (int j = 0; j < gradWidth; ++j) {
            if(gradient[i][j] > threshold) {
                pair = {i, j};
                edgePixels.push_back(pair);
            }
        }
    }

    for (int i = 0; i < gradHeight; ++i) {
        delete[] gradient[i];
    }
    delete[] gradient;

    for (int i = 0; i < sobOpHeight; ++i) {
        delete[] sobelGx[i];
        delete[] sobelGy[i];
    }
    delete[] sobelGx;
    delete[] sobelGy;

    return edgePixels;
}
