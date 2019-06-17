#ifndef WEBCAMFEATUREEXTRACTOR_H
#define WEBCAMFEATUREEXTRACTOR_H

#include "ofMain.h"
#include "ofxCvColorImage.h"
#include "ofxCvGrayscaleImage.h"
#include <ofxCvHaarFinder.h>

class WebcamFeatureExtractor
{
public:
	WebcamFeatureExtractor();
	void setup(std::string);
	double getRythm();

	void calculate(ofVideoGrabber);
	vector<ofxCvBlob> detectFaces(ofVideoGrabber);

private:
	double calculateDiffBetweenFrames(ofVideoGrabber);

	int frameCounter;
	int frameStep = 7;		// co 7 frame'ów

	CvHistogram* prevHist = NULL;

	double rythm = 0.0;
	double rythmThreshold = 200;

	ofxCvHaarFinder webcamFaceFinder;
	int faceAreaThreshold = 5000;

};

#endif // WEBCAMFEATUREEXTRACTOR_H
