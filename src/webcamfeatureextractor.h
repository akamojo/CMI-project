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
	double getLuminance();
	vector<double> getColors();
	double getRythm();

	void calculate(ofVideoGrabber);
	vector<ofxCvBlob> detectFaces(ofVideoGrabber);

private:
	double calculateDiffBetweenFrames(ofVideoGrabber);

	vector<double> calculateFrame(ofVideoGrabber);

	void convertPixels(ofPixels &inPixels, ofPixels &newPixels, int vidWidth, int vidHeight);
	vector <double> calculatePixel(ofPixels, int i, int j, int vidWidth, int nChannels);

	const double rc = 0.2126;
	const double gc = 0.7152;
	const double bc = 0.0722;

	int skipStep; // 250 sampli per frame
	const int samplesPerFrame = 250;

	int frameCounter;
	int frameStep = 7;		// co 7 frame'ów

	CvHistogram* prevHist = NULL;

	double rythm;
	double rythmThreshold = 200;
	double luminance;
	vector<double> colors;

	ofxCvHaarFinder webcamFaceFinder;
	int faceAreaThreshold = 5000;

};

#endif // WEBCAMFEATUREEXTRACTOR_H
