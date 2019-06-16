#ifndef WEBCAMFEATUREEXTRACTOR_H
#define WEBCAMFEATUREEXTRACTOR_H

#include <ofVideoPlayer.h>
#include "ofMain.h"
#include "ofxCvColorImage.h"
#include "ofxCvGrayscaleImage.h"
#include <ofxCvHaarFinder.h>

#include <iostream>
#include <vector>
#include <math.h>

class WebcamFeatureExtractor
{
public:
	WebcamFeatureExtractor();
	void setup();
	//double getLuminance();
	//vector<double> getAvgColors();
	double getRythm();

	void calculate(ofVideoGrabber);
	vector<ofxCvBlob> detectFaces(ofVideoGrabber);

private:
	//vector<double> calculateFrame();
	double calculateDiffBetweenFrames(ofVideoGrabber);

	/*void convertPixels(ofPixels &inPixels, ofPixels &newPixels, int vidWidth, int vidHeight);
	vector <double> calculatePixel(ofPixels, int i, int j, int vidWidth, int nChannels);

	const double rc = 0.2126;
	const double gc = 0.7152;
	const double bc = 0.0722;*/

	//int skipStep; // 250 sampli per frame
	//const int samplesPerFrame = 250;

	int frameCounter;
	int frameStep = 7; // 25 frame'ów
	//const int framesPerVideo = 75;

	CvHistogram* prevHist = NULL;

	double rythm = 0.0;
	double rythmThreshold = 200;
	//double luminance;
	//vector<double> avgColors;

	ofxCvHaarFinder webCamPreviewFaceFinder;
	ofImage colorImg;

};

#endif // WEBCAMFEATUREEXTRACTOR_H
