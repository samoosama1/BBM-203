#include "ImageSharpening.h"

// Default constructor
ImageSharpening::ImageSharpening() {
    double blurConst = 1 / (double) 9;
    blurKernel = new double*[kernelRow];
    for(int i = 0; i < kernelRow; i++) {
        blurKernel[i] = new double[kernelCol];
        for(int j = 0; j < kernelCol; j++) {
            blurKernel[i][j] = blurConst;
        }
    }
}

ImageSharpening::~ImageSharpening(){
    for (int i = 0; i < kernelRow; ++i) {
        delete[] blurKernel[i];
    }
    delete[] blurKernel;
}

ImageMatrix ImageSharpening::sharpen(const ImageMatrix& input_image, double k) {
    Convolution conv(blurKernel, kernelRow, kernelCol, 1, true);
    ImageMatrix blurredImg = conv.convolve(input_image);
    ImageMatrix sharpImg = input_image + (input_image - blurredImg) * k;
    sharpImg.clipData();
    return sharpImg;
}
