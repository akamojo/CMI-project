#include "featureextractor.h"

FeatureExtractor::FeatureExtractor() {
}

void FeatureExtractor::setup(std::string path)
{
    this->videoFilePath = path;
    this->frameCounter = 0;
    this->luminance = -1.0;
	this->prevHist = NULL;
	this->rythm = -1.0;
	this->numberOfFaces = 0;

    this->skipStep = -1;
    this->frameStep = -1;

	this->avgColors.clear();
	for (int i = 0; i < 3; i++) {
		this->avgColors.push_back(-1.0);
	}

	this->faceFinder.setup("haarcascade_frontalface_default.xml");

    ofFile fileToRead(path);
    fileToRead.copyTo(tempFilename, true, true);
}


double FeatureExtractor::getLuminance()
{
    return this->luminance;
}

vector<double> FeatureExtractor::getAvgColors()
{
	return avgColors;
}

double FeatureExtractor::getRythm()
{
	return rythm;
}

int FeatureExtractor::getNumberOfFaces()
{
	return numberOfFaces;
}

void FeatureExtractor::calculate() {

    if (videoFilePath == "?") return;

    videoPlayer.load(tempFilename);
    videoPlayer.setLoopState(OF_LOOP_NONE);
    videoPlayer.setVolume(0.0);
    videoPlayer.play();

    skipStep = int( sqrt(((videoPlayer.getWidth() * videoPlayer.getHeight()) / (float) samplesPerFrame)) );
    frameStep = int( videoPlayer.getTotalNumFrames() / framesPerVideo );

    ofLog(OF_LOG_NOTICE, "[LumExtractor] starts for " + videoFilePath + "...");

    ofLog(OF_LOG_NOTICE, ofToString(skipStep) + ", frameStep: " + ofToString(frameStep));
    ofLog(OF_LOG_NOTICE, ofToString(videoPlayer.getWidth()) + " " + ofToString(videoPlayer.getHeight()) + " "
          + ofToString(videoPlayer.getTotalNumFrames()));

    double currentLumi;
	vector<double> currentColors;

    while (true) {

        videoPlayer.update();
        if (videoPlayer.getPosition() >= 0.999) {
            break;
        }

        if (videoPlayer.isFrameNew()) {
            this->frameCounter++;
            if (frameCounter % frameStep == 0) {
                currentColors = this->calculateFrame();
                luminance += currentColors[3];

				if (this->calculateDiffBetweenFrames() > this->rythmThreshold)
					rythm += 1;

				for (int i = 0; i < 3; i++) {
					avgColors[i] += currentColors[i];
				}

            }
			numberOfFaces = max(numberOfFaces, detectFaces());
        }
    }

	rythm = rythm / (double)(frameCounter / frameStep);
	ofLog(OF_LOG_NOTICE, "[RythmExtractor] RYTHM = " + ofToString(rythm));

    luminance = luminance / (double)(frameCounter / frameStep);
    ofLog(OF_LOG_NOTICE, "[LumExtractor] LUMI = " + ofToString(luminance));

	ofLog(OF_LOG_NOTICE, "[NumberOfFaces] NUMOFFAC = " + ofToString(numberOfFaces));

	for (int i = 0; i < 3; i++) {
		avgColors[i] /= (double)(frameCounter / frameStep);
	}

    videoPlayer.close();    
}

int FeatureExtractor::detectFaces()
{
	colorImg.setFromPixels(videoPlayer.getPixels());
	faceFinder.findHaarObjects(colorImg);
	return faceFinder.blobs.size();
}

vector<double> FeatureExtractor::calculatePixel(ofPixels pixels, int i, int j, int vidWidth, int nChannels) {
	vector<double> colors;

	for (int k = 0; k < 3; k++) {
		colors.push_back((double)pixels[(j * vidWidth + i) * nChannels + k]);
	}

	return colors;
}

vector<double> FeatureExtractor::calculateFrame() {
    double result[4] = {0.0};

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
    vector<double> vresult(result, result+n);
    return vresult;
}

double FeatureExtractor::calculateDiffBetweenFrames() {

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
    cvNormalizeHist(hist, 255);

	double diff = 0;

	if (prevHist != NULL)
		diff = cvCompareHist(hist, prevHist, CV_COMP_INTERSECT);

	prevHist = hist;

	return diff;
}

void FeatureExtractor::convertPixels(ofPixels &inPixels, ofPixels &newPixels, int vidWidth, int vidHeight) {

    double currentLumi;
    for (int i = 0; i < vidWidth; ++i) {
        for (int j = 0; j < vidHeight; ++j) {
			vector<double> colors = calculatePixel(inPixels, i, j, vidWidth, 3);
            currentLumi = rc * colors[0] + gc * colors[1] + bc * colors[2];
            newPixels[j * vidWidth + i] = (int) currentLumi;
        }
    }
}
