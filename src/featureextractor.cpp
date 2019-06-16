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

    this->skipStep = -1;
    this->frameStep = -1;

	this->avgColors.clear();
	for (int i = 0; i < 3; i++) {
		this->avgColors.push_back(-1.0);
	}

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

vector<double> FeatureExtractor::getEdgeDistribution()
{
    return edgesHistogram;
}

std::string FeatureExtractor::getVideoResolution() {
    return resolutionStr;
}

vector<double> FeatureExtractor::getTextureMoments() {
    return textureMoments;
}

void FeatureExtractor::calculate() {

    if (videoFilePath == "?") return;

    videoPlayer.load(tempFilename);
    videoPlayer.setLoopState(OF_LOOP_NONE);
    videoPlayer.setVolume(0.0);
    videoPlayer.play();

    skipStep = int( sqrt(((videoPlayer.getWidth() * videoPlayer.getHeight()) / (float) samplesPerFrame)) );
    frameStep = int( videoPlayer.getTotalNumFrames() / framesPerVideo );

    ofLog(OF_LOG_NOTICE, "[FeatureExtractor] starts for " + videoFilePath + "...");

    double currentLumi;
	vector<double> currentColors;
    vector<vector<double>> framesEdgeHistograms;

    ofxCvColorImage colorImg;
    ofxCvGrayscaleImage grayImg;

    while (true) {

        videoPlayer.update();
        if (videoPlayer.getPosition() >= 0.999) {
            break;
        }

        if (videoPlayer.isFrameNew()) {
            if (frameCounter % frameStep == 0) {

                colorImg.setFromPixels(videoPlayer.getPixels());
                grayImg = colorImg;

                if (frameCounter == 0) {
                    // first frame
                    resolutionStr = ofToString(videoPlayer.getWidth()) + " x " + ofToString(videoPlayer.getHeight());
                    textureMoments = this->calculateTextures(grayImg);
                }

                currentColors = this->calculateFrame();
                luminance += currentColors[3];


                if (this->calculateDiffBetweenFrames(grayImg) > this->rythmThreshold)
					rythm += 1;

                vector<double> currentEdgeDistribution = this->calculateEdgeDistribution(grayImg);
                framesEdgeHistograms.push_back(currentEdgeDistribution);

				for (int i = 0; i < 3; i++) {
					avgColors[i] += currentColors[i];
				}

            }
            this->frameCounter++;
        }
    }

	rythm = (double)(frameCounter / frameStep) / rythm - 1.0;
	ofLog(OF_LOG_NOTICE, "[RythmExtractor] RYTHM = " + ofToString(rythm));

    luminance = luminance / (double)(frameCounter / frameStep);
    ofLog(OF_LOG_NOTICE, "[LumExtractor] LUMI = " + ofToString(luminance));

	for (int i = 0; i < 3; i++) {
		avgColors[i] /= (double)(frameCounter / frameStep);
	}

    edgesHistogram = this->avgEdgeDistribution(framesEdgeHistograms);

    videoPlayer.close();    
}

vector<double> FeatureExtractor::calculateTextures(ofxCvGrayscaleImage grayImg) {
    // to openCV
    cv::Mat src = ofxCv::toCv(grayImg.getPixels());
    cv::Mat gaborKernel, dst;

    vector<double> result;
    cv::Mat m, stdv;

    for (int i = 0; i < sizeof(sigmas)/sizeof(double); ++i) {
        for (int j = 0; j < sizeof(thetas)/sizeof(double); ++j) {
            gaborKernel = cv::getGaborKernel(cv::Size(3, 3), sigmas[i], thetas[j], 10.0, 0.5);
            cv::filter2D(src, dst, -1, gaborKernel);
            cv::meanStdDev(dst, m, stdv);
            result.push_back(m.at<double>(0));
            result.push_back(stdv.at<double>(0));
        }
    }

    return result;
}

vector<double> FeatureExtractor::avgEdgeDistribution(vector<vector<double>> framesHistograms) {

    vector<double> result(framesHistograms[0].size(), 0.0);


    // for each frame
    for (size_t i = 0; i < framesHistograms.size(); ++i) {
        // for each edge type
        for (size_t j = 0; j < framesHistograms[i].size(); ++j) {
            result[j] += framesHistograms[i][j];
        }
    }

    for (size_t i = 0; i < result.size(); ++i) {
        result[i] /= (double) framesHistograms.size();
    }

    return result;
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

vector<double> FeatureExtractor::calculateEdgeDistribution(ofxCvGrayscaleImage grayImg) {

    // to openCV
    cv::Mat src = ofxCv::toCv(grayImg.getPixels());
    cv::Mat blurred, edges, binarized, kernel;

    // blur
    GaussianBlur(src, blurred, cv::Size(5, 5), 0);

    vector<double> result;
    double edges_ratio;

    // apply different edge filters
    for (int i = 0; i < kernelsNum; i++) {
        kernel = cv::Mat(2, 2, CV_32F, kernels[i]);

        cv::filter2D(blurred, edges, CV_32F, kernel); // ? CV_32F - > -1?
        cv::threshold(edges, binarized, threshold_value, max_binary_value, cv::THRESH_BINARY);

        cv::Size s = binarized.size();
        edges_ratio = (double)cv::countNonZero(binarized) / (double)(s.height * s.width);

        result.push_back(edges_ratio);
    }

    return result;
}

double FeatureExtractor::calculateDiffBetweenFrames(ofxCvGrayscaleImage grayImg) {

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
