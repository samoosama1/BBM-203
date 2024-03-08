#include <iostream>


#include "ImageMatrix.h"
#include "Convolution.h"
#include "ImageSharpening.h"
#include "EdgeDetector.h"
#include "DecodeMessage.h"

int main(){
    ImageMatrix img(R"(D:\TERM 3\BBM 203\AS1\src\output.txt)");
    ImageSharpening sharpening{};
    ImageMatrix sharpImg = sharpening.sharpen(img, 2);
    EdgeDetector detector{};
    auto list = detector.detectEdges(sharpImg);
    DecodeMessage decode{};
    std::string str = decode.decodeFromImage(sharpImg, list);
    std::cout << str;
}