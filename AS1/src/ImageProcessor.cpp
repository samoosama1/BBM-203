#include "ImageProcessor.h"

ImageProcessor::ImageProcessor() {

}

ImageProcessor::~ImageProcessor() {

}


std::string ImageProcessor::decodeHiddenMessage(const ImageMatrix &img) {
    ImageSharpening sharpening{};
    ImageMatrix sharpImg = sharpening.sharpen(img, 2);
    EdgeDetector detector{};
    auto list = detector.detectEdges(sharpImg);
    DecodeMessage decode{};
    std::string str = decode.decodeFromImage(sharpImg, list);
    return str;
}

ImageMatrix ImageProcessor::encodeHiddenMessage(const ImageMatrix &img, const std::string &message) {
    ImageSharpening sharpening{};
    ImageMatrix sharpImg = sharpening.sharpen(img, 2);
    EdgeDetector detector{};
    auto list = detector.detectEdges(sharpImg);
    EncodeMessage encodeMessage{};
    ImageMatrix output = encodeMessage.encodeMessageToImage(img, message, list);
    return output;
}
