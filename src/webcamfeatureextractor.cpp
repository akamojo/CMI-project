#include "webcamfeatureextractor.h"

WebcamFeatureExtractor::WebcamFeatureExtractor() {
}

void WebcamFeatureExtractor::setup()
{
	this->frameCounter = 0;
	//this->luminance = -1.0;
	this->prevHist = NULL;
	this->rythm = 0.0;

	//this->skipStep = -1;
	//this->frameStep = -1;

	/*this->avgColors.clear();
	for (int i = 0; i < 3; i++) {
		this->avgColors.push_back(-1.0);
	}*/

	webCamPreviewFaceFinder.setup("haarcascade_frontalface_default.xml");
}


/*double WebcamFeatureExtractor::getLuminance()
{
	return this->luminance;
}

vector<double> WebcamFeatureExtractor::getAvgColors()
{
	return avgColors;
}*/

double WebcamFeatureExtractor::getRythm()
{
	return rythm / (double)(frameCounter / frameStep);
}

void WebcamFeatureExtractor::calculate(ofVideoGrabber vidGrabber) {

	this->frameCounter++;
	if (frameCounter % frameStep == 0) {
		if (this->calculateDiffBetweenFrames(vidGrabber) > this->rythmThreshold)
			rythm += 1;
	}
}

vector<ofxCvBlob> WebcamFeatureExtractor::detectFaces(ofVideoGrabber vidGrabber) {
	colorImg.setFromPixels(vidGrabber.getPixels());
	webCamPreviewFaceFinder.findHaarObjects(colorImg);
	return webCamPreviewFaceFinder.blobs;
}

/*vector<double> WebcamFeatureExtractor::calculatePixel(ofPixels pixels, int i, int j, int vidWidth, int nChannels) {
	vector<double> colors;

	for (int k = 0; k < 3; k++) {
		colors.push_back((double)pixels[(j * vidWidth + i) * nChannels + k]);
	}

	return colors;
}

vector<double> WebcamFeatureExtractor::calculateFrame() {
	double result[4] = { 0.0 };

	ofPixels & pixels = videoPlayer.getPixels();
	int vidWidth = pixels.getWidth();
	int vidHeight = pixels.getHeight();
	int nChannels = pixels.getNumChannels();

	double currentLumi;
	for (int i = 0; i < vidWidth; i += skipStep) {
		for (int j = 0; j < vidHeight; j += skipStep) {
			vector<double> currentColors = calculatePixel(pixels, i, j, vidWidth, nChannels);
			currentLumi = rc * currentColors[0] + gc * currentColors[1] + bc * currentColors[2];

			for (int k = 0; k < 3; k++) {
				result[k] += currentColors[k];
			}
			result[3] += currentLumi;
		}
	}

	for (int k = 0; k < 4; k++) {
		result[k] /= (double)(vidWidth / skipStep * vidHeight / skipStep);
	}

	int n = sizeof(result) / sizeof(result[0]);
	vector<double> vresult(result, result + n);
	return vresult;
}*/

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

/*void WebcamFeatureExtractor::convertPixels(ofPixels &inPixels, ofPixels &newPixels, int vidWidth, int vidHeight) {

	double currentLumi;
	for (int i = 0; i < vidWidth; ++i) {
		for (int j = 0; j < vidHeight; ++j) {
			vector<double> colors = calculatePixel(inPixels, i, j, vidWidth, 3);
			currentLumi = rc * colors[0] + gc * colors[1] + bc * colors[2];
			newPixels[j * vidWidth + i] = (int)currentLumi;
		}
	}
}*/
