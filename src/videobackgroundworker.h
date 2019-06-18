#ifndef VIDEOBACKGROUNDWORKER_H
#define VIDEOBACKGROUNDWORKER_H

#include "featureextractor.h"
#include "ofMain.h"
#include <ofFileUtils.h>
#include <ofThread.h>
#include <ofxXmlSettings.h>

class VideoBackgroundWorker : public ofThread {

public:
    VideoBackgroundWorker();
    void setup(ofDirectory dir);
    bool isWorking();

protected:
    void threadedFunction();

private:
    ofDirectory videosDir;
    ofxXmlSettings xmlHandler;
    FeatureExtractor extractor;
    bool workFinished = false;

    void updateXML(string path, string tag, double value, double missingValue);
    void updateXML(string path, string tag, string value, string missingValue);

    void updateXMLWithVector(string path, string tag, string subTag, vector<double> values, double missingValue);
    void updateXMLWithObjects(string path, string tag, string subTag, vector<double> objectsCounts, vector<string> objectsNames, double missingValue);
};

#endif // VIDEOBACKGROUNDWORKER_H
