#pragma once
#include "ofMain.h"
#include "ofxGui.h"
#include "Thumbnail.h"

#include "featureextractor.h"
#include "videobackgroundworker.h"
#include "webcampreview.h"

#include <ofxCvHaarFinder.h>
#include <ofxXmlSettings.h>

class GuiApp: public ofBaseApp {
public:
	void setup();
	void update();
	void draw();

	ofVideoPlayer mainPlayer;
    int mainPlayerWidth = 500;
    int mainPlayerHeight = 400;

	ofxButton upButton;
	ofxButton downButton;
	ofxButton addButton;
    ofxButton goBackButton;

	ofxPanel nav;
	ofxPanel details;

    ofxPanel startScreenNav;
    ofxButton startButton;
    ofxButton manageButton;
	ofxLabel numberOfFaces;
	ofxLabel rythmOfCaptured;

    ofxLabel videoName;
    ofxLabel videoLuminance;
	ofxLabel videoR;
	ofxLabel videoG;
	ofxLabel videoB;
	ofxLabel videoRythm;
	ofxLabel videoNumOfFaces;

	ofxButton playButton;
	ofxButton pauseButton;
	ofxButton stopButton;

	ofDirectory dir;
	vector<Thumbnail*> thumbnails;

    ofxXmlSettings xmlHandler;

    VideoBackgroundWorker worker;

    int thumbnailIdxOffset = 0;
    int thumbnailsOffset = 10;
	int currentVideo = 0;

    WebCamPreview webCamPreview;
    bool startScreenMode = false;
    ofxCvHaarFinder webCamPreviewFaceFinder;
    ofImage colorImg;

    ofVideoGrabber vidGrabber;
    int camWidth = 320;
    int camHeight = 240;
    int camPreviewOffset = 50;

    void createMetadatasFiles();

	void upButtonPressed();
	void downButtonPressed();
    void addButtonPressed();
    void goBackButtonPressed();

	void playButtonPressed();
	void pauseButtonPressed();
	void stopButtonPressed();

    void startButtonPressed();
    void manageButtonPressed();

	void playVideo();
	void keyPressed(int);

    void exit();
    void updateXML(int videoIdx, string tag, double value);
private:
    void setupVidGrabber();
    void checkVidGrabberDevices();
};

