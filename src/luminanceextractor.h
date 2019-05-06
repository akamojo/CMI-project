#ifndef LUMINANCEEXTRACTOR_H
#define LUMINANCEEXTRACTOR_H

#include <ofVideoPlayer.h>

class LuminanceExtractor : public ofThread
{
public:
    LuminanceExtractor();
    void setup(std::string path);
    bool isReady();
    double getLuminance();

private:

    void threadedFunction();

    double calculateFrame();

    void convertPixels(ofPixels &inPixels, ofPixels &newPixels, int vidWidth, int vidHeight);
    double calculatePixel(ofPixels, int i, int j, int vidWidth, int nChannels);

    const double rc = 0.2126;
    const double gc = 0.7152;
    const double bc = 0.0722;

    const int skipStep = 50;

    int frameCounter;
    const int frameStep = 50;

    double luminance;
    bool ready;

    std::string videoFilePath;
    ofVideoPlayer videoPlayer;

};

#endif // LUMINANCEEXTRACTOR_H
