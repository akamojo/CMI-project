#ifndef VIDEOBACKGROUNDWORKER_H
#define VIDEOBACKGROUNDWORKER_H

#include "ofMain.h"
#include <ofFileUtils.h>
#include <ofThread.h>
#include <ofxXmlSettings.h>

class VideoBackgroundWorker : public ofThread {

public:
    VideoBackgroundWorker();
    void setup(ofDirectory dir);

    // ofThread interface
protected:
    void threadedFunction();

private:
    ofDirectory metadatasDir;
    ofxXmlSettings xmlHandler;
};

#endif // VIDEOBACKGROUNDWORKER_H
