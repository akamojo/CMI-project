/*
 * GuiApp.cpp
 *
 *  Created on: Oct 28, 2014
 *      Author: arturo
 */

#include "GuiApp.h"

void GuiApp::setup(){
	parameters.setName("parameters");
	parameters.add(radius.set("radius",50,1,100));
	parameters.add(color.set("color",100,ofColor(0,0),255));
	gui.setup(parameters);
	ofBackground(255, 0, 144);

	dir.listDir("videos/");
	dir.allowExt("mov");
	dir.sort(); // in linux the file system doesn't return file lists ordered in alphabetical order

				//allocate the vector to have as many ofImages as files
	if (dir.size()) {
		videos.assign(dir.size(), ofVideoPlayer());
	}

	// you can now iterate through the files and load them into the ofImage vector
	for (int i = 0; i < (int)dir.size(); i++) {
		cout << dir.getPath(i) << endl;
		videos[i].load(dir.getPath(i));
		videos[i].setLoopState(OF_LOOP_NORMAL);
		videos[i].play();
		videos[i].setPaused(true);
	}
	currentVideo = 0;
	videos[currentVideo].setPaused(false);

	ofSetVerticalSync(false);
}

void GuiApp::update(){
	for (int i = 0; i < (int)videos.size(); i++) {
		videos[i].update();
	}
}

void GuiApp::draw(){
	gui.draw();
	if (dir.size() > 0) {
		ofSetColor(ofColor::white);

		for (int i = 0; i < (int)videos.size(); i++) {
			videos[i].draw(300, 20 + i * (videos[0].getHeight() + 10));
		}
	}
}

void GuiApp::keyPressed(int key)
{
	if (dir.size() > 0) {
		videos[currentVideo].setPaused(true);
		currentVideo++;
		currentVideo %= dir.size();

		videos[currentVideo].setPaused(false);
	}
}
