/*
 * GuiApp.cpp
 *
 *  Created on: Oct 28, 2014
 *      Author: arturo
 */

#include "GuiApp.h"

inline bool ends_with(string const & value, string const & ending)
{
	if (ending.size() > value.size()) return false;
	return equal(ending.rbegin(), ending.rend(), value.rbegin());
}

void GuiApp::checkMetadatas() {

    for (int i = 0; i < (int)dir.size(); i++) {

        vector<string> spl = ofSplitString(dir.getPath(i), ".");
        string xmlFilePath = spl[0] + ".xml";
        if (xmlHandler.loadFile(xmlFilePath)) {
            ofLog(OF_LOG_NOTICE, xmlFilePath + " loaded");
            double lumi = xmlHandler.getValue("luminance", -1.0);
            if (lumi == -1.0) {
                // ? do something ? or not?
            }
        }
        else {
            ofLog(OF_LOG_NOTICE, xmlFilePath + " does not exist");

            int mainPos = xmlHandler.addTag("metadata");
            xmlHandler.pushTag("metadata");
            xmlHandler.addValue("name", ofSplitString(dir.getName(i), ".")[0]);
            xmlHandler.popTag();

            xmlHandler.saveFile(xmlFilePath);
            xmlHandler.clear();
        }
    }
}

void GuiApp::setupVidGrabber() {
    vidGrabber.setDeviceID(0);
    vidGrabber.setDesiredFrameRate(60);
    vidGrabber.initGrabber(camWidth, camHeight);
}

void GuiApp::setup(){
	ofSetVerticalSync(true);

    // START SCREEN

    vector<ofVideoDevice> devices = vidGrabber.listDevices();

    for (size_t i = 0; i < devices.size(); i++) {
        if (devices[i].bAvailable) {
            ofLogNotice() << devices[i].id << ": " << devices[i].deviceName;
        }
        else {
            ofLogNotice() << devices[i].id << ": " << devices[i].deviceName << " - unavailable ";
        }
    }

    setupVidGrabber();
    ofBackground(0);

    // VIDEO LIBRARY SCREEN

	upButton.addListener(this, &GuiApp::upButtonPressed);
	downButton.addListener(this, &GuiApp::downButtonPressed);
    addButton.addListener(this, &GuiApp::addButtonPressed);
    goBackButton.addListener(this, &GuiApp::goBackButtonPressed);

	nav.setup();
	nav.add(upButton.setup("up"));
	nav.add(downButton.setup("down"));
    nav.add(addButton.setup("add new video"));
    nav.add(goBackButton.setup("BACK"));

    ofBackground(255, 0, 144);

    dir.allowExt("mov");
    dir.allowExt("mp4");
    dir.listDir("videos/");
	dir.sort(); // in linux the file system doesn't return file lists ordered in alphabetical order

    checkMetadatas();

	for (int i = 0; i < (int)dir.size(); i++) {
		cout << dir.getPath(i) << endl;
		Thumbnail *t = new Thumbnail();

		thumbnails.push_back(t);
		thumbnails[i]->setup(dir.getPath(i));
        thumbnails[i]->set(thumbnailsOffset, 20 + 100 + (i%3) * (thumbnails[0]->thumbnailSize + 10),
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

    details.add(videoLuminance.setup("lumi", ""));

    startButton.addListener(this, &GuiApp::startButtonPressed);
    manageButton.addListener(this, &GuiApp::manageButtonPressed);

    startScreenNav.setup();
    startScreenNav.setPosition(camPreviewOffset, camPreviewOffset + camHeight + 30);
    startScreenNav.add(startButton.setup("START"));
    startScreenNav.add(manageButton.setup("MANAGE"));

    playVideo();
	ofSetVerticalSync(false);
}

void GuiApp::update(){

    if (startScreenMode) {
        vidGrabber.update();
    }
    else {
        for (int i = 0; i < (int)thumbnails.size(); i++) {
            thumbnails[i]->video.update();
            thumbnails[i]->update();
        }
        mainPlayer.update();

        if (waitsForLuminance && luminanceExtractor.isReady()) {
            waitsForLuminance = false;
            double readLuminance = luminanceExtractor.getLuminance();
            luminanceExtractor.stopThread();
            videoLuminance = ofToString(readLuminance);
            updateXML(currentVideo, "luminance", readLuminance);
        }
    }
}

void GuiApp::updateXML(int videoIdx, string tag, double value) {

    string xmlFilePath = ofSplitString(dir.getPath(videoIdx), ".")[0] + ".xml";
    if (xmlHandler.loadFile(xmlFilePath)) {

        xmlHandler.pushTag("metadata");

        double getCurrentValue = xmlHandler.getValue(tag, -1.0);
        if (getCurrentValue != -1.0) {
            xmlHandler.setValue(tag, value);
            ofLog(OF_LOG_WARNING, "Replaced tag " + tag + " ...");
        }
        else {
            xmlHandler.addValue(tag, value);
            ofLog(OF_LOG_NOTICE, "Writing " + ofToString(value) + " to XML, " + dir.getPath(videoIdx));
        }

        xmlHandler.popTag();
        xmlHandler.saveFile(xmlFilePath);
    }
}

void GuiApp::draw(){

    if (startScreenMode) {
        vidGrabber.draw(camPreviewOffset, camPreviewOffset);
        startScreenNav.draw();

        ofSetHexColor(0xffffff);
        ofNoFill();
    }
    else {
        nav.draw();
        details.draw();

        if (dir.size() > 0) {
            ofSetColor(ofColor::white);

            for (int i = thumbnailIdxOffset; i < thumbnailIdxOffset + 3; i++) {
                if (i < (int)thumbnails.size() && i >= 0) {
                    thumbnails[i]->enabled = true;
                    thumbnails[i]->draw(thumbnailsOffset,
                        20 + 100 + (i - thumbnailIdxOffset) * (thumbnails[0]->thumbnailSize + 10));
                }
            }

            mainPlayer.draw(thumbnailsOffset + thumbnails[0]->thumbnailSize + 50, 20, mainPlayerWidth, mainPlayerHeight);
        }
    }

}

void GuiApp::playVideo() {

    if (currentVideo < thumbnails.size() && currentVideo >= 0) {
        string currentName = thumbnails[currentVideo]->name;

        mainPlayer.load(currentName);
        mainPlayer.setLoopState(OF_LOOP_NORMAL);
        mainPlayer.play();

        if (xmlHandler.loadFile(ofSplitString(dir.getPath(currentVideo), ".")[0] + ".xml")) {
			
			xmlHandler.pushTag("metadata");
            videoName = xmlHandler.getValue("name", "?");
            double getLumi = xmlHandler.getValue("luminance", -1.0);
            videoLuminance = ofToString(getLumi);
            
			if (getLumi == -1.0) {
                if (!waitsForLuminance) {
                    waitsForLuminance = true;
                    luminanceExtractor.setup(dir.getPath(currentVideo));
                    luminanceExtractor.startThread();
                }
            }
        }

        details.setPosition(thumbnailsOffset + thumbnails[0]->thumbnailSize + 50,
            20 + mainPlayerHeight + 10);

	}
}

void GuiApp::downButtonPressed() {
    if (thumbnailIdxOffset + 3 < (int)thumbnails.size()) {

        thumbnailIdxOffset += 3;
        currentVideo = thumbnailIdxOffset;

        for (int i = 0; i < thumbnailIdxOffset; i++)
			thumbnails[i]->enabled = false;
		
		playVideo();
	}
}

void GuiApp::upButtonPressed() {

    if (thumbnailIdxOffset - 3 >= 0) {

        thumbnailIdxOffset -= 3;
        currentVideo = thumbnailIdxOffset;

        for (int i = thumbnailIdxOffset; i < (int)thumbnails.size(); i++)
			thumbnails[i]->enabled = false;
		
		playVideo();
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

            thumbnails[(int)thumbnails.size() - 1]->set(thumbnailsOffset, 20 + 100 + (((int)thumbnails.size() - 1) % 3)
				* (thumbnails[0]->thumbnailSize + 10),
				thumbnails[0]->thumbnailSize, thumbnails[0]->thumbnailSize);
		}
    }
}

void GuiApp::goBackButtonPressed()
{
    if (!startScreenMode)
        startScreenMode = true;
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

void GuiApp::startButtonPressed()
{

}

void GuiApp::manageButtonPressed()
{
    if (startScreenMode)
        startScreenMode = false;
}

void GuiApp::keyPressed(int key) {

	currentVideo++;

    currentVideo %= 3;
    currentVideo += thumbnailIdxOffset;

    if (currentVideo == dir.size()) {
        currentVideo = thumbnailIdxOffset;
    }

	playVideo();
}

void GuiApp::exit() {
    if (luminanceExtractor.isThreadRunning())
        luminanceExtractor.stopThread();

	upButton.removeListener(this, &GuiApp::upButtonPressed);
	downButton.removeListener(this, &GuiApp::downButtonPressed);
	playButton.removeListener(this, &GuiApp::playButtonPressed);
	pauseButton.removeListener(this, &GuiApp::pauseButtonPressed);
	stopButton.removeListener(this, &GuiApp::stopButtonPressed);
	addButton.removeListener(this, &GuiApp::addButtonPressed);
}
