#include "luminanceextractor.h"

LuminanceExtractor::LuminanceExtractor() {
}

void LuminanceExtractor::setup(std::string path)
{
    this->videoFilePath = path;
    this->frameCounter = 0;
    this->luminance = -1.0;
}

double LuminanceExtractor::getLuminance()
{
    return this->luminance;
}

void LuminanceExtractor::calculate() {

    videoPlayer.load(videoFilePath);
    videoPlayer.setLoopState(OF_LOOP_NONE);

    videoPlayer.play();
    ofLog(OF_LOG_NOTICE, "[LumExtractor] starts for " + videoFilePath + "...");

    double currentLumi;
    while (true) {

        videoPlayer.update();
        if (videoPlayer.getPosition() >= 0.999) {
            break;
        }

        if (videoPlayer.isFrameNew()) {
            this->frameCounter++;
            if (frameCounter % frameStep == 0) {
                currentLumi = this->calculateFrame();
                luminance += currentLumi;
            }
        }
    }

    luminance = luminance / (double)(frameCounter / frameStep);
    ofLog(OF_LOG_NOTICE, "[LumExtractor] LUMI = " + ofToString(luminance));
}

double LuminanceExtractor::calculatePixel(ofPixels pixels, int i, int j, int vidWidth, int nChannels) {
    double red = (double) pixels[(j * vidWidth + i) * nChannels    ];
    double green =  (double) pixels[(j * vidWidth + i) * nChannels + 1];
    double blue = (double) pixels[(j * vidWidth + i) * nChannels + 2];

    return rc * red + gc * green + bc * blue;
}

double LuminanceExtractor::calculateFrame() {
    double luminanceSum = 0.0;

    ofPixels & pixels = videoPlayer.getPixels();
    int vidWidth = pixels.getWidth();
    int vidHeight = pixels.getHeight();
    int nChannels = pixels.getNumChannels();

    double currentLumi;
    for (int i = 0; i < vidWidth; i += skipStep) {
        for (int j = 0; j < vidHeight; j += skipStep) {
            currentLumi = calculatePixel(pixels, i, j, vidWidth, nChannels);
            luminanceSum += currentLumi;
        }
    }
    return luminanceSum / (double)(vidWidth/skipStep * vidHeight/skipStep);
}

void LuminanceExtractor::convertPixels(ofPixels &inPixels, ofPixels &newPixels, int vidWidth, int vidHeight) {

    double currentLumi;
    for (int i = 0; i < vidWidth; ++i) {
        for (int j = 0; j < vidHeight; ++j) {
            currentLumi = calculatePixel(inPixels, i, j, vidWidth, 3);
            newPixels[j * vidWidth + i] = (int) currentLumi;
        }
    }
}
