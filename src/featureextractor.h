#ifndef FEATUREEXTRACTOR_H
#define FEATUREEXTRACTOR_H

#include <ofVideoPlayer.h>
#include "ofMain.h"
#include "ofxCv.h"

#include "keypointsmatcher.h"
#include "ofxCvColorImage.h"
#include "ofxCvGrayscaleImage.h"

#include <iostream>
#include <vector>
#include <math.h>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace ofxCv;
using namespace std;

class FeatureExtractor
{
public:
    FeatureExtractor();
    void setup(string path);
    double getLuminance();
	vector<double> getAvgColors();
	double getRythm();
    vector<double> getEdgeDistribution();
    string getVideoResolution();
    vector<double> getTextureMoments();

    void calculate();
    vector<double> getObjectsCount();
    vector<string> getObjectsNames();

private:
    vector<double> calculateFrame();
    double calculateDiffBetweenFrames(ofxCvGrayscaleImage grayImg);
    vector<double> avgEdgeDistribution(vector<vector<double>> framesHistograms);

    vector<double> calculateEdgeDistribution(Mat src);
    vector<double> calculateTextures(Mat src);

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

	double rythm;
	double rythmThreshold = 200;
    double luminance;
	vector<double> avgColors;
    vector<double> edgesHistogram;

    string videoFilePath = "?";
    string tempFilename = "videos/temp/temp.mp4";
    string resolutionStr = "?";
    ofVideoPlayer videoPlayer;

    // Edge Distribution variables
    const int kernelsNum = 5;
    float kernels[5][4] = {
        {1, 1, -1, -1},
        {1, -1, 1, -1},
        {1.41f, 0.0f, 0.0f, -1.41f},
        {0.0f, 1.41f, -1.41f, 0.0f},
        {2, -2, -2, 2}
    };
    const double threshold_value = 1.0;
    const double max_binary_value = 255.0;

    // Texture features variables
    const double sigmas[4] = {3.0, 4.0, 5.0, 6.0};
    const double thetas[7] = {22.5, 45.0, 67.5, 90.0, 123.75, 157.5, 180.0};
    vector<double> textureMoments;

    // Keypoints
    KeypointsMatcher keypointsMatcher;
    vector<double> objectsCount;

};

#endif // FEATUREEXTRACTOR_H
