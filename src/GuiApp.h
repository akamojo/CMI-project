#pragma once

#include "ofMain.h"
#include "ofxGui.h"
#include "Thumbnail.h"

class GuiApp: public ofBaseApp {
public:
	void setup();
	void update();
	void draw();

	ofxButton upButton;
	ofxButton downButton;

	ofxPanel gui;

	ofDirectory dir;
	vector<Thumbnail> thumbnails;

	int initialVideo = 0;
	int thumbnailsOffset = 250;

	void upButtonPressed();
	void downButtonPressed();
	void exit();
};

