#include "webcamfeatureextractor.h"

WebcamFeatureExtractor::WebcamFeatureExtractor() {
}

void WebcamFeatureExtractor::setup(std::string fileName)
{
	this->frameCounter = 0;
	this->prevHist = NULL;
	this->rythm = 0.0;

	webCamPreviewFaceFinder.setup(fileName);
}

double WebcamFeatureExtractor::getRythm()
{
	return rythm;
}

void WebcamFeatureExtractor::calculate(ofVideoGrabber vidGrabber) {

	this->frameCounter++;
	double diff;

	if (frameCounter % frameStep == 0) {
		diff = this->calculateDiffBetweenFrames(vidGrabber);
		if (diff != 0)
			rythm = (double) 255.0 / diff - 1.0;
	}
}

vector<ofxCvBlob> WebcamFeatureExtractor::detectFaces(ofVideoGrabber vidGrabber) {
	ofImage colorImg; 
	colorImg.setFromPixels(vidGrabber.getPixels());
	webCamPreviewFaceFinder.findHaarObjects(colorImg);
	return webCamPreviewFaceFinder.blobs;
}

double WebcamFeatureExtractor::calculateDiffBetweenFrames(ofVideoGrabber vidGrabber) {

	ofxCvColorImage colorImg;
	ofxCvGrayscaleImage grayImg;

	colorImg.setFromPixels(vidGrabber.getPixels());
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
	cvNormalizeHist(hist, 255);

	double diff = 0;

	if (prevHist != NULL)
		diff = cvCompareHist(hist, prevHist, CV_COMP_INTERSECT);

	prevHist = hist;

	return diff;
}
