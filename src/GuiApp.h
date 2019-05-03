#pragma once

#include "ofMain.h"
#include "ofxGui.h"
#include "Thumbnail.h"

class GuiApp: public ofBaseApp {
public:
	void setup();
	void update();
	void draw();

	ofParameterGroup parameters;
	ofParameter<float> radius;
	ofParameter<ofColor> color;
	ofxPanel gui;

	ofDirectory dir;
	vector<Thumbnail> thumbnails;

	int currentVideo;
};

