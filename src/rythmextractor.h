#ifndef RYTHMEXTRACTOR_H
#define RYTHMEXTRACTOR_H

#include <ofVideoPlayer.h>
#include "ofMain.h"
#include "ofxCvColorImage.h"
#include "ofxCvGrayscaleImage.h"

class RythmExtractor : public ofThread
{
public:
	RythmExtractor();
	void setup(std::string path);
	bool isReady();
	double getRythm();

private:

	void threadedFunction();

	double calculateFrame();

	const int skipStep = 100;

	int frameCounter;
	const int frameStep = 100;

	CvHistogram* prevHist = NULL;

	double rythm;
	bool ready;

	std::string videoFilePath;
	ofVideoPlayer videoPlayer;

};

#endif // RYTHMEXTRACTOR_H
