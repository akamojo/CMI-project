#pragma once
#include "ofMain.h"
#include "ofxGui.h"
#include "Thumbnail.h"

#include "featureextractor.h"
#include "videobackgroundworker.h"
#include "webcampreview.h"
#include "webcamfeatureextractor.h"

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
    float detailsWidth = 400.0, detailsHeight = 400.0;

    ofxPanel startScreenNav;
    ofxButton startButton;
    ofxButton manageButton;
	ofxLabel numberOfFaces;
	ofxLabel capturedLuminance;
	ofxLabel capturedR;
	ofxLabel capturedG;
	ofxLabel capturedB;
	ofxLabel capturedRythm;

    ofxLabel videoName;
    ofxLabel videoResolution;
	ofxLabel videoCategory;

    ofxLabel videoLuminance;
    ofxLabel videoColors;
	ofxLabel videoRythm;
    ofxLabel edgeHist;

    ofxLabel texMoments;
    string texMomentsString = "";
    string objDetectedString = "";

	ofxButton playButton;
	ofxButton pauseButton;
	ofxButton stopButton;

	ofDirectory dir;
    ofDirectory objDir;
	vector<Thumbnail*> thumbnails;

    ofxXmlSettings xmlHandler;

    VideoBackgroundWorker worker;

    int thumbnailIdxOffset = 0;
    int thumbnailsOffset = 10;
	int currentVideo = 0;

    WebCamPreview webCamPreview;
    bool startScreenMode = false;
	WebcamFeatureExtractor webcamFeatureExtractor;
	vector<ofxCvBlob> faces;

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

	void playVideo(int);
	void keyPressed(int);

    void exit();
    //void updateXML(int videoIdx, string tag, double value);
private:
	bool clickedStart = false;
    void setupVidGrabber();
    void checkVidGrabberDevices();
    void readXML(string videoXMLPath);

    vector<double> calculateDifferences();

    void loadObjectNames(string dirPath);
    void loadVideosNames(string dirPath);
};

