/*
 * GuiApp.cpp
 *
 *  Created on: Oct 28, 2014
 *      Author: arturo
 */

#include "GuiApp.h"

inline bool ends_with(std::string const & value, std::string const & ending)
{
	if (ending.size() > value.size()) return false;
	return std::equal(ending.rbegin(), ending.rend(), value.rbegin());
}

void GuiApp::setup(){
	ofSetVerticalSync(true);

	upButton.addListener(this, &GuiApp::upButtonPressed);
	downButton.addListener(this, &GuiApp::downButtonPressed);
	addButton.addListener(this, &GuiApp::addButtonPressed);

	nav.setup();
	nav.add(upButton.setup("up"));
	nav.add(downButton.setup("down"));
	nav.add(addButton.setup("add new"));

	ofBackground(255, 0, 144);

	dir.listDir("videos/");
	dir.allowExt("mov");
	dir.allowExt("mp4");

	dir.sort(); // in linux the file system doesn't return file lists ordered in alphabetical order

				//allocate the vector to have as many ofImages as files

	// you can now iterate through the files and load them into the ofImage vector
	for (int i = 0; i < (int)dir.size(); i++) {
		cout << dir.getPath(i) << endl;
		Thumbnail *t = new Thumbnail();

		thumbnails.push_back(t);
		thumbnails[i]->setup(dir.getPath(i));//, *this);

		thumbnails[i]->set(300, 20 + (i%3) * (thumbnails[0]->thumbnailSize + 10), 
			thumbnails[i]->thumbnailSize, thumbnails[i]->thumbnailSize);
	}

	playButton.addListener(this, &GuiApp::playButtonPressed);
	pauseButton.addListener(this, &GuiApp::pauseButtonPressed);
	stopButton.addListener(this, &GuiApp::stopButtonPressed);

	details.setup();
	details.add(videoName.setup("file", ""));
	details.add(playButton.setup("play"));
	details.add(pauseButton.setup("pause"));
	details.add(stopButton.setup("stop"));

	playVideo(thumbnails[3]->name);

	ofSetVerticalSync(false);
}

void GuiApp::update(){
	for (int i = 0; i < (int)thumbnails.size(); i++) {
		thumbnails[i]->video.update();
		thumbnails[i]->update();
	}
	mainPlayer.update();
}

void GuiApp::draw(){
	nav.draw();
	details.draw();

	if (dir.size() > 0) {
		ofSetColor(ofColor::white);

		for (int i = initialVideo; i < initialVideo + 3; i++) {
			if (i < (int)thumbnails.size() && i >= 0) {
				thumbnails[i]->enabled = true;
				thumbnails[i]->draw(thumbnailsOffset, 
					20 + (i - initialVideo) * (thumbnails[0]->thumbnailSize + 10));
			}
		}

		mainPlayer.draw(thumbnailsOffset + thumbnails[0]->thumbnailSize + 100, 20);
	}
}

void GuiApp::playVideo(string name) {
	mainPlayer.load(name);
	mainPlayer.setLoopState(OF_LOOP_NORMAL);
	mainPlayer.play();

	videoName = thumbnails[3]->name;
	details.setPosition(thumbnailsOffset + thumbnails[0]->thumbnailSize + 100,
		20 + mainPlayer.getHeight() + 10);
}

void GuiApp::downButtonPressed() {
	if (initialVideo + 3 < (int)thumbnails.size()) {
		initialVideo += 3;
		for (int i = 0; i < initialVideo; i++) {
			thumbnails[i]->enabled = false;
		}
	}
}

void GuiApp::upButtonPressed() {
	if (initialVideo - 3 >= 0) {
		for (int i = initialVideo; i < (int)thumbnails.size(); i++) {
			thumbnails[i]->enabled = false;
		}
		initialVideo -= 3;
	}
}

void GuiApp::addButtonPressed() {
	ofFileDialogResult result = ofSystemLoadDialog("Load file");
	if (result.bSuccess) {
		string path = result.getPath();
		if (ends_with(path, "mov") || ends_with(path, "mp4")) {
			Thumbnail *t = new Thumbnail();
			thumbnails.push_back(t);
			thumbnails[(int)thumbnails.size() - 1]->setup(path);

			thumbnails[(int)thumbnails.size() - 1]->set(300, 20 + (((int)thumbnails.size() - 1) % 3)
				* (thumbnails[0]->thumbnailSize + 10),
				thumbnails[0]->thumbnailSize, thumbnails[0]->thumbnailSize);
		}
	}
}

void GuiApp::playButtonPressed() {
	mainPlayer.play();
	mainPlayer.setPaused(false);
}

void GuiApp::pauseButtonPressed() {
	mainPlayer.setPaused(true);
}

void GuiApp::stopButtonPressed() {
	mainPlayer.stop();
}

void GuiApp::thumbnailPressed() {
	cout << "lol" << endl;
}

void GuiApp::exit() {
	upButton.removeListener(this, &GuiApp::upButtonPressed);
	downButton.removeListener(this, &GuiApp::downButtonPressed);
	playButton.removeListener(this, &GuiApp::playButtonPressed);
	pauseButton.removeListener(this, &GuiApp::pauseButtonPressed);
	stopButton.removeListener(this, &GuiApp::stopButtonPressed);
	addButton.removeListener(this, &GuiApp::addButtonPressed);
}