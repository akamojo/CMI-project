#ifndef VIDEOBACKGROUNDWORKER_H
#define VIDEOBACKGROUNDWORKER_H

#include "luminanceextractor.h"
#include "ofMain.h"
#include <ofFileUtils.h>
#include <ofThread.h>
#include <ofxXmlSettings.h>

class VideoBackgroundWorker : public ofThread {

public:
    VideoBackgroundWorker();
    void setup(ofDirectory dir);
    bool isWorking();

    // ofThread interface
protected:
    void threadedFunction();

private:
    ofDirectory videosDir;
    ofxXmlSettings xmlHandler;
    LuminanceExtractor extractor;
    bool workFinished = false;

    void updateXML(string path, string tag, double value, double missingValue);
};

#endif // VIDEOBACKGROUNDWORKER_H
