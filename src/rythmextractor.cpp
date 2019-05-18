#include "rythmextractor.h"

RythmExtractor::RythmExtractor() {
}

void RythmExtractor::setup(std::string path)
{
	this->videoFilePath = path;
	this->frameCounter = 0;
	this->rythm = -1.0;
	this->prevHist = NULL;
	this->ready = false;
}

bool RythmExtractor::isReady()
{
	return this->ready;
}

double RythmExtractor::getRythm()
{
	return isReady() ? this->rythm : 1.0;
}

void RythmExtractor::threadedFunction() {
	videoPlayer.load(videoFilePath);
	videoPlayer.setLoopState(OF_LOOP_NONE);
	videoPlayer.play();
	ofLog(OF_LOG_NOTICE, "LumExtractor starts for " + videoFilePath + "...");

	double currentDiff;
	while (true) {

		videoPlayer.update();
		if (videoPlayer.getPosition() >= 0.999) {
			break;
		}

		if (videoPlayer.isFrameNew()) {
			this->frameCounter++;
			if (frameCounter % frameStep == 0) {
				currentDiff = this->calculateFrame();
				rythm += currentDiff;
			}
		}
	}

	rythm = rythm / (double)(frameCounter / frameStep);
	ofLog(OF_LOG_NOTICE, "[thread] LUMI = " + ofToString(rythm));
	ready = true;
}

double RythmExtractor::calculateFrame() {

	ofxCvColorImage colorImg;
	ofxCvGrayscaleImage grayImg;

	colorImg.setFromPixels(videoPlayer.getPixels());
	grayImg = colorImg;

	IplImage* grayCVImg = grayImg.getCvImage();
	int g_bins = 32;
	CvHistogram* hist;
	{
		int hist_size[] = { g_bins };
		float g_ranges[] = { 0, 255 };
		float* ranges[] = { g_ranges };
		hist = cvCreateHist(
			1,
			hist_size,
			CV_HIST_ARRAY,
			ranges,
			1
		);
	}
	cvCalcHist(&grayCVImg, hist);

	double diff = 0;

	if (prevHist != NULL)
		diff = cvCompareHist(hist, prevHist, CV_COMP_INTERSECT);

	prevHist = hist;

	return diff;
}

