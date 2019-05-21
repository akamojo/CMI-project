#ifndef FEATUREEXTRACTOR_H
#define FEATUREEXTRACTOR_H

#include <ofVideoPlayer.h>
#include "ofMain.h"
#include "ofxCvColorImage.h"
#include "ofxCvGrayscaleImage.h"

#include <iostream>
#include <vector>

class FeatureExtractor
{
public:
    FeatureExtractor();
    void setup(std::string path);
    double getLuminance();
	vector<double> getAvgColors();
	double getRythm();

    void calculate();

private:
    vector<double> calculateFrame();
	double calculateDiffBetweenFrames();

    void convertPixels(ofPixels &inPixels, ofPixels &newPixels, int vidWidth, int vidHeight);
    vector <double> calculatePixel(ofPixels, int i, int j, int vidWidth, int nChannels);

    const double rc = 0.2126;
    const double gc = 0.7152;
    const double bc = 0.0722;

    const int skipStep = 100;

    int frameCounter;
    const int frameStep = 100;

	CvHistogram* prevHist = NULL;

	double rythm;
    double luminance;
	vector<double> avgColors;

    std::string videoFilePath = "?";
    std::string tempFilename = "videos/temp.mp4";
    ofVideoPlayer videoPlayer;


};

#endif // FEATUREEXTRACTOR_H
