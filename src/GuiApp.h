#pragma once

#include "ofMain.h"
#include "ofxGui.h"
#include "Thumbnail.h"

class GuiApp: public ofBaseApp {
public:
	void setup();
	void update();
	void draw();

	ofVideoPlayer mainPlayer;

	ofxButton upButton;
	ofxButton downButton;
	ofxButton addButton;

	ofxPanel nav;
	ofxPanel details;

	ofxLabel videoName;

	ofxButton playButton;
	ofxButton pauseButton;
	ofxButton stopButton;

	ofDirectory dir;
	vector<Thumbnail> thumbnails;

	int initialVideo = 0;
	int thumbnailsOffset = 250;

	void upButtonPressed();
	void downButtonPressed();

	void playButtonPressed();
	void pauseButtonPressed();
	void stopButtonPressed();
	void addButtonPressed();

	void playVideo(string name);
	void thumbnailPressed();

	void exit();
};

