#pragma once
#include "ofMain.h"
#include "ofxGui.h"
#include "Thumbnail.h"
#include "luminanceextractor.h"

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

	ofxPanel nav;
	ofxPanel details;

    ofxLabel videoName;
    ofxLabel videoLuminance;

	ofxButton playButton;
	ofxButton pauseButton;
	ofxButton stopButton;

	ofDirectory dir;
	vector<Thumbnail*> thumbnails;

    ofxXmlSettings xmlHandler;
    LuminanceExtractor luminanceExtractor;
    bool waitsForLuminance = false;

	int initialVideo = 0;
    int thumbnailsOffset = 10;
	int currentVideo = 0;

    void checkMetadatas();

	void upButtonPressed();
	void downButtonPressed();

	void playButtonPressed();
	void pauseButtonPressed();
	void stopButtonPressed();
	void addButtonPressed();

	void playVideo();
	void keyPressed(int);

    void exit();
    void updateXML(int videoIdx, string tag, double value);
};

