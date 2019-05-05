#include "luminanceextractor.h"

LuminanceExtractor::LuminanceExtractor() {
}

double LuminanceExtractor::calculatePixel(ofPixels pixels, int i, int j, int vidWidth, int nChannels) {
    double red = (double) pixels[(j * vidWidth + i) * nChannels    ];
    double green =  (double) pixels[(j * vidWidth + i) * nChannels + 1];
    double blue = (double) pixels[(j * vidWidth + i) * nChannels + 2];

    return rc * red + gc * green + bc * blue;
}

double LuminanceExtractor::calculate(ofVideoPlayer vidPlayer) {
    double luminanceSum = 0.0;

    ofPixels & pixels = vidPlayer.getPixels();
    int vidWidth = pixels.getWidth();
    int vidHeight = pixels.getHeight();
    int nChannels = pixels.getNumChannels();

    ofLog(OF_LOG_NOTICE, ofToString(vidWidth));
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
