#ifndef KEYPOINTSMATCHER_H
#define KEYPOINTSMATCHER_H

#include <ofVideoPlayer.h>
#include "ofMain.h"
#include "ofxCv.h"

#include "ofxCvColorImage.h"
#include "ofxCvGrayscaleImage.h"

#include <iostream>
#include <vector>
#include <math.h>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace ofxCv;
using namespace std;

class KeypointsMatcher {

public:
    KeypointsMatcher();
    void analyzeObjects(string dirPath);
    vector<size_t> countObjects(ofxCvGrayscaleImage &grayImg, int goodMatchCoeff = 1);

    int getObjectsCount();
    vector<string> getObjectsNames();
private:

    ofDirectory dir;

     Ptr<FeatureDetector> detector;
     Ptr<DescriptorExtractor> extractor;
     Ptr<DescriptorMatcher> matcher;

     vector<vector<KeyPoint>> objectsKeypoints;
     vector<Mat> objectsDescriptors;
     vector<string> objectsNames;

     float maxAbsoluteDistance = 200.0;

     void detectObject(Mat &object, std::vector<KeyPoint> &keypoints_object, Mat &descriptors_object);
};

#endif // KEYPOINTSMATCHER_H
