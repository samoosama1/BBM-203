#include "ImageMatrix.h"
#include <iostream>


// Default constructor
ImageMatrix::ImageMatrix(){
    height = 3;
    width = 3;
    data = new double * [height];

    for (int i = 0; i < height; ++i) {
        data[i] = new double [width]{0};
    }
}


// Parameterized constructor for creating a blank image of given size
ImageMatrix::ImageMatrix(int imgHeight, int imgWidth) : height(imgHeight), width(imgWidth) {
    data = new double * [imgHeight];
    for (int i = 0; i < imgHeight; ++i) {
        data[i] = new double [imgWidth]{0};
    }
}

// Parameterized constructor for loading image from file. PROVIDED FOR YOUR CONVENIENCE
ImageMatrix::ImageMatrix(const std::string &filepath) {
    // Create an ImageLoader object and load the image
    ImageLoader imageLoader(filepath);

    // Get the dimensions of the loaded image
    height = imageLoader.getHeight();
    width = imageLoader.getWidth();

    // Allocate memory for the matrix
    data = new double*[height];
    for (int i = 0; i < height; ++i) {
        data[i] = new double[width];
    }

    // Copy data from imageLoader to data
    double** imageData = imageLoader.getImageData();
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; j++) {
            data[i][j] = imageData[i][j];
        }
    }
}



// Destructor
ImageMatrix::~ImageMatrix() {
    for (int i = 0; i < height; ++i) {
        delete[] data[i];
    }
    delete[] data;
}

// Parameterized constructor - direct initialization with 2D matrix
ImageMatrix::ImageMatrix(const double** inputMatrix, int imgHeight, int imgWidth){
    height = imgHeight;
    width = imgWidth;
    data = new double * [height];
    for (int i = 0; i < height; ++i) {
        data[i] = new double [width];
        for (int j = 0; j < width; ++j) {
            data[i][j] = inputMatrix[i][j];
        }
    }
}

// Copy constructor
ImageMatrix::ImageMatrix(const ImageMatrix &other) {
    height = other.height;
    width = other.width;
    data = new double * [height];
    for (int i = 0; i < height; ++i) {
        data[i] = new double[width];
        for (int j = 0; j < width; ++j) {
            data[i][j] = other.data[i][j];
        }
    }
}

// Copy assignment operator
ImageMatrix& ImageMatrix::operator=(const ImageMatrix &other) {
    if (&other != this) {
        for (int i = 0; i < height; ++i) {
            delete[] data[i];
        }
        delete[] data;

        height = other.height;
        width = other.width;

        data = new double * [height];
        for (int i = 0; i < height; ++i) {
            data[i] = new double[width];
            for (int j = 0; j < width; ++j) {
                data[i][j] = other.data[i][j];
            }
        }
    }
    return *this;
}



// Overloaded operators

// Overloaded operator + to add two matrices
ImageMatrix ImageMatrix::operator+(const ImageMatrix &other) const {
    ImageMatrix img{height, width};
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            img.data[i][j] = data[i][j] + other.data[i][j];
        }
    }
    return img;
}

// Overloaded operator - to subtract two matrices
ImageMatrix ImageMatrix::operator-(const ImageMatrix &other) const {
    ImageMatrix img{height, width};
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            img.data[i][j] = data[i][j] - other.data[i][j];
        }
    }
    return img;
}

// Overloaded operator * to multiply a matrix with a scalar
ImageMatrix ImageMatrix::operator*(const double &scalar) const {
    ImageMatrix img{height, width};
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            img.data[i][j] = data[i][j] * scalar;
        }
    }
    return img;
}


// Getter function to access the data in the matrix
double** ImageMatrix::get_data() const {
    return data;
}

// Getter function to access the data at the index (i, j)
double ImageMatrix::get_data(int i, int j) const {
    return data[i][j];
}

int ImageMatrix::get_height() const {
    return height;
}

int ImageMatrix::get_width() const {
    return width;
}

void ImageMatrix::printData() const {
    std::cout << "Location of data: " << &data << "\n";
    std::cout << "Location of object: " << this << "\n";
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            std::cout << data[i][j] << " ";
        }
        std::cout << "\n";
    }
    std::cout << "\n";
}

void ImageMatrix::clipData() {
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            if(data[i][j] > 255)
                data[i][j] = 255;
            else if(data[i][j] < 0)
                data[i][j] = 0;
        }
    }
}

