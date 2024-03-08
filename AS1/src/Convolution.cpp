#include <iostream>

#include "Convolution.h"

// Default constructor 
Convolution::Convolution() {
    kernelHeight = 3;
    kernelWidth = 3;
    stride = 1;
    padding = true;
    kernel = new double * [kernelHeight];
    for (int i = 0; i < kernelHeight; ++i) {
        kernel[i] = new double [kernelWidth]{0};
    }
}

// Parametrized constructor for custom kernel and other parameters
Convolution::Convolution(double** customKernel, int kh, int kw, int stride_val, bool pad){
    kernelHeight = kh;
    kernelWidth = kw;
    stride = stride_val;
    padding = pad;
    kernel = new double * [kernelHeight];
    for (int i = 0; i < kernelHeight; ++i) {
        kernel[i] = new double [kernelWidth];
        for (int j = 0; j < kernelWidth; ++j) {
            kernel[i][j] = customKernel[i][j];
        }
    }
}

// Destructor
Convolution::~Convolution() {
    for (int i = 0; i < kernelHeight; ++i) {
        delete[] kernel[i];
    }
    delete[] kernel;
}

// Copy constructor
Convolution::Convolution(const Convolution &other){
    stride = other.stride;
    padding = other.padding;
    kernelHeight = other.kernelHeight;
    kernelWidth = other.kernelWidth;
    kernel = new double * [kernelHeight];

    for (int i = 0; i < kernelHeight; ++i) {
        kernel[i] = new double [kernelWidth];
        for (int j = 0; j < kernelWidth; ++j) {
            kernel[i][j] = other.kernel[i][j];
        }
    }
}

// Copy assignment operator
Convolution& Convolution::operator=(const Convolution &other) {
    if(&other != this) {
        for (int i = 0; i < kernelHeight; ++i) {
            delete[] kernel[i];
        }
        delete[] kernel;

        stride = other.stride;
        padding = other.padding;
        kernelHeight = other.kernelHeight;
        kernelWidth = other.kernelWidth;
        kernel = new double * [kernelHeight];

        for (int i = 0; i < kernelHeight; ++i) {
            kernel[i] = new double [kernelWidth];
            for (int j = 0; j < kernelWidth; ++j) {
                kernel[i][j] = other.kernel[i][j];
            }
        }
    }
    return *this;
}


// Convolve Function: Responsible for convolving the input image with a kernel and return the convolved image.
ImageMatrix Convolution::convolve(const ImageMatrix& input_image) const {
    int padSize = padding ? 1 : 0;
    int outHeight = (input_image.get_height() - kernelHeight + 2 * padSize) / stride + 1;
    int outWidth = (input_image.get_width() - kernelWidth + 2 * padSize) / stride + 1;
    int copyHeight;
    int copyWidth;
    double** inputCopy;

    if(padding) {
        copyHeight = input_image.get_height() + 2;
        copyWidth = input_image.get_width() + 2;
        inputCopy = new double * [copyHeight];

        for (int i = 0; i < copyHeight; ++i)
            inputCopy[i] = new double[copyWidth]{0};

        for (int i = 1; i < copyHeight - 1; ++i)
            for (int j = 1; j < copyWidth - 1 ; ++j)
                inputCopy[i][j] = input_image.get_data(i - 1, j - 1);
    } else {
        copyHeight = input_image.get_height();
        copyWidth = input_image.get_width();
        inputCopy = new double * [copyHeight];
        for (int i = 0; i < input_image.get_height(); ++i) {
            inputCopy[i] = new double [copyWidth];
            for (int j = 0; j < input_image.get_width(); ++j)
                inputCopy[i][j] = input_image.get_data(i, j);
        }
    }

    double** convImage = new double * [outHeight];
    for(int i = 0; i < outHeight; i++) {
        convImage[i] = new double[outWidth];
    }

    int k, t;
    double sum;
    k = 0;
    for (int i = 0; i < outHeight; ++i) {
        t = 0;
        for (int j = 0; j < outWidth; ++j) {
            sum = 0;
            for (int l = 0; l < kernelHeight; ++l)
                for (int m = 0; m < kernelWidth; ++m)
                    sum += kernel[l][m] * inputCopy[k + l][t + m];
            convImage[i][j] = sum;
            t += stride;
        }
        k += stride;
    }

    ImageMatrix img{const_cast<const double**>(convImage), outHeight, outWidth};

    for (int i = 0; i < copyHeight; ++i) {
        delete[] inputCopy[i];
    }
    delete[] inputCopy;

    for (int i = 0; i < outHeight; ++i) {
        delete[] convImage[i];
    }
    delete[] convImage;

    return img;
}
