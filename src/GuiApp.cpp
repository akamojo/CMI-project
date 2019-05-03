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
		thumbnails.assign(dir.size(), Thumbnail());
	}

	// you can now iterate through the files and load them into the ofImage vector
	for (int i = 0; i < (int)dir.size(); i++) {
		cout << dir.getPath(i) << endl;
		thumbnails[i].video.load(dir.getPath(i));
		thumbnails[i].video.setLoopState(OF_LOOP_NORMAL);
		thumbnails[i].video.play();
		thumbnails[i].video.setPaused(true);

		thumbnails[i].set(300, 20 + i * (thumbnails[0].video.getHeight() + 10), 320, 240);
		thumbnails[i].setup();
	}

	ofSetVerticalSync(false);
}

void GuiApp::update(){
	for (int i = 0; i < (int)thumbnails.size(); i++) {
		thumbnails[i].video.update();
		thumbnails[i].update();
	}
}

void GuiApp::draw(){
	gui.draw();

	if (dir.size() > 0) {
		ofSetColor(ofColor::white);

		for (int i = 0; i < (int)thumbnails.size(); i++) {
			thumbnails[i].video.draw(300, 20 + i * (thumbnails[0].video.getHeight() + 10));
		}
	}
}
