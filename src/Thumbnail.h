#pragma once

#include "ofMain.h"
#include "GuiApp.h"
#include "ofxMSAInteractiveObject.h"

#define		IDLE_COLOR		0xFFFFFF
#define		OVER_COLOR		0x00FF00
#define		DOWN_COLOR		0xFF0000


class Thumbnail : public ofxMSAInteractiveObject {
public:
	ofVideoPlayer video;
	bool enabled = false;
	int thumbnailSize = 300;
	string name;

	void setup(string path) {
		name = path;

		video.load(path);
		video.setLoopState(OF_LOOP_NORMAL);
		video.play();
		video.setPaused(true);

		enableMouseEvents();
		enableKeyEvents();
	}


	void exit() {
	}


	void update() {
	}


	void draw(int x, int y) {
		/*if (isMousePressed()) ofSetHexColor(DOWN_COLOR);
		else if (isMouseOver()) ofSetHexColor(OVER_COLOR);
		else ofSetHexColor(IDLE_COLOR);

		ofRect(x, y, width, height);*/
		video.draw(x, y, thumbnailSize, thumbnailSize);
	}

	virtual void onRollOver(int x, int y) {
		if (enabled)
			video.setPaused(false);
	}

	virtual void onRollOut() {
		video.setPaused(true);
	}

	virtual void onMouseMove(int x, int y) {
	}

	virtual void onDragOver(int x, int y, int button) {
	}

	virtual void onDragOutside(int x, int y, int button) {
	}

	virtual void onPress(int x, int y, int button) {
		//if (enabled)
		//	app.thumbnailPressed();
	}

	virtual void onRelease(int x, int y, int button) {
	}

	virtual void onReleaseOutside(int x, int y, int button) {
	}

	virtual void keyPressed(int key) {
	}

	virtual void keyReleased(int key) {
	}

};