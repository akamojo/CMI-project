#ifndef FEATUREEXTRACTOR_H
#define FEATUREEXTRACTOR_H

#include <ofVideoPlayer.h>
#include "ofMain.h"
#include "ofxCvColorImage.h"
#include "ofxCvGrayscaleImage.h"

#include <iostream>
#include <vector>
#include <math.h>

#include <ofxCvHaarFinder.h>
#include <ofxXmlSettings.h>

class FeatureExtractor
{
public:
    FeatureExtractor();
    void setup(std::string path);
    double getLuminance();
	vector<double> getAvgColors();
	double getRythm();
	int getNumberOfFaces();

    void calculate();

private:
    vector<double> calculateFrame();
	double calculateDiffBetweenFrames();
	int detectFaces();

    void convertPixels(ofPixels &inPixels, ofPixels &newPixels, int vidWidth, int vidHeight);
    vector <double> calculatePixel(ofPixels, int i, int j, int vidWidth, int nChannels);

    const double rc = 0.2126;
    const double gc = 0.7152;
    const double bc = 0.0722;

    int skipStep; // 250 sampli per frame
    const int samplesPerFrame = 250;

    int frameCounter;
    int frameStep; // 25 frame'ów
    const int framesPerVideo = 75;

	CvHistogram* prevHist = NULL;

	ofxCvHaarFinder faceFinder;
	ofImage colorImg;
	int numberOfFaces;
	int faceAreaThreshold = 5000;

	double rythm;
	double rythmThreshold = 200;
    double luminance;
	vector<double> avgColors;

    std::string videoFilePath = "?";
    std::string tempFilename = "videos/temp/temp.mp4";
    ofVideoPlayer videoPlayer;

};

#endif // FEATUREEXTRACTOR_H
