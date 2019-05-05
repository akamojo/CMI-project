#ifndef LUMINANCEEXTRACTOR_H
#define LUMINANCEEXTRACTOR_H

#include <ofVideoPlayer.h>

class LuminanceExtractor
{
public:
    double calculate(ofVideoPlayer vidPlayer);

    LuminanceExtractor();
    void convertPixels(ofPixels &inPixels, ofPixels &newPixels, int vidWidth, int vidHeight);

private:
    const double rc = 0.2126;
    const double gc = 0.7152;
    const double bc = 0.0722;

    const int skipStep = 30;

    double calculatePixel(ofPixels, int i, int j, int vidWidth, int nChannels);
};

#endif // LUMINANCEEXTRACTOR_H
