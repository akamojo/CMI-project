#pragma once

#include "ofMain.h"
#include "GuiApp.h"
#include "ofxMSAInteractiveObject.h"

class Thumbnail : public ofxMSAInteractiveObject {
public:
	ofVideoPlayer video;
	bool enabled = false;
    int thumbnailSize = 200;
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
