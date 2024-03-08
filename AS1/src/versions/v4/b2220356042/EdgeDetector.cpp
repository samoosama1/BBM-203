// EdgeDetector.cpp

#include "EdgeDetector.h"
#include <cmath>

void deleteArr(double** arr, int row);
// Default constructor
EdgeDetector::EdgeDetector() {
}

// Destructor
EdgeDetector::~EdgeDetector() {
}

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

    double threshold{};
    double val;
    double sum = 0;
    for (int i = 0; i < gradHeight; ++i) {
        gradient[i] = new double [gradWidth];
        for (int j = 0; j < gradWidth; ++j) {
            val = std::sqrt(std::pow(imgX.get_data(i,j), 2) + std::pow(imgY.get_data(i,j), 2));
            gradient[i][j] = val;
            sum += val;
        }
    }

    threshold = sum / gradHeight * gradWidth;
    std::vector<std::pair<int, int>> edgePixels;

    for (int i = 0; i < gradHeight; ++i) {
        for (int j = 0; j < gradWidth; ++j) {
            if(gradient[i][j] > threshold)
                edgePixels.push_back(std::make_pair(i, j));
        }

    }

    deleteArr(sobelGx, sobOpHeight);
    deleteArr(sobelGy, sobOpHeight);
    deleteArr(gradient, gradHeight);

    return edgePixels;
}

void deleteArr(double** arr, int row) {
    for (int i = 0; i < row; ++i) {
        delete[] arr[i];
    }
    delete[] arr;
}

