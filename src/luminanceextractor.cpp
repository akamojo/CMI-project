#include "luminanceextractor.h"

LuminanceExtractor::LuminanceExtractor() {
}

void LuminanceExtractor::setup(std::string path)
{
    this->videoFilePath = path;
    this->frameCounter = 0;
    this->luminance = -1.0;

	this->avgColors.clear();

	for (int i = 0; i < 3; i++) {
		this->avgColors.push_back(-1.0);
	}

    this->ready = false;
}

bool LuminanceExtractor::isReady()
{
    return this->ready;
}

double LuminanceExtractor::getLuminance()
{
    return isReady() ? this->luminance : 1.0;
}

vector<double> LuminanceExtractor::getAvgColors()
{
	return avgColors;
}

void LuminanceExtractor::threadedFunction() {
    videoPlayer.load(videoFilePath);
    videoPlayer.setLoopState(OF_LOOP_NONE);
    videoPlayer.play();
    ofLog(OF_LOG_NOTICE, "LumExtractor starts for " + videoFilePath + "...");
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

				for (int i = 0; i < 3; i++) {
					avgColors[i] += currentColors[i];
				}

            }
        }
    }

    luminance /= (double)(frameCounter / frameStep);

	for (int i = 0; i < 3; i++) {
		avgColors[i] /= (double)(frameCounter / frameStep);
	}

    ofLog(OF_LOG_NOTICE, "[thread] LUMI = " + ofToString(luminance));
    ready = true;
}

vector<double> LuminanceExtractor::calculatePixel(ofPixels pixels, int i, int j, int vidWidth, int nChannels) {
	vector<double> colors;

	for (int k = 0; k < 3; k++) {
		colors.push_back((double)pixels[(j * vidWidth + i) * nChannels + k]);
	}

	return colors;
}

vector<double> LuminanceExtractor::calculateFrame() {
	vector<double> result;
	
	for (int i = 0; i < 4; i++) {
		result.push_back(0.0);
	}


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

	return result;
}

void LuminanceExtractor::convertPixels(ofPixels &inPixels, ofPixels &newPixels, int vidWidth, int vidHeight) {

    double currentLumi;
    for (int i = 0; i < vidWidth; ++i) {
        for (int j = 0; j < vidHeight; ++j) {
			vector<double> colors = calculatePixel(inPixels, i, j, vidWidth, 3);
            currentLumi = rc * colors[0] + gc * colors[1] + bc * colors[2];
            newPixels[j * vidWidth + i] = (int) currentLumi;
        }
    }
}
