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

void GuiApp::createMetadatasFiles() {

    for (int i = 0; i < (int)dir.size(); i++) {

        xmlHandler.clear();

        vector<string> spl = ofSplitString(dir.getPath(i), ".");
        string xmlFilePath = spl[0] + ".xml";
        if (xmlHandler.loadFile(xmlFilePath)) {
            ofLog(OF_LOG_NOTICE, xmlFilePath + " metadata file loaded");
        }
        else {
            ofLog(OF_LOG_NOTICE, xmlFilePath + " does not exist");

            int mainPos = xmlHandler.addTag("metadata");
            xmlHandler.pushTag("metadata");

            xmlHandler.addValue("name", ofSplitString(dir.getName(i), ".")[0]);
            xmlHandler.addValue("luminance", -1.0);
            xmlHandler.addValue("red", -1.0);
            xmlHandler.addValue("green", -1.0);
            xmlHandler.addValue("blue", -1.0);
			xmlHandler.addValue("rythm", -1.0);

            xmlHandler.popTag();
            xmlHandler.saveFile(xmlFilePath);

            ofLog(OF_LOG_NOTICE, xmlFilePath + " metadata file created");
        }
    }
    xmlHandler.clear();
}

void GuiApp::setupVidGrabber() {
    vidGrabber.setDeviceID(0);
    vidGrabber.setDesiredFrameRate(60);
    vidGrabber.initGrabber(camWidth, camHeight);
}

void GuiApp::checkVidGrabberDevices() {
    vector<ofVideoDevice> devices = vidGrabber.listDevices();

    for (size_t i = 0; i < devices.size(); i++) {
        if (devices[i].bAvailable) {
            ofLogNotice() << devices[i].id << ": " << devices[i].deviceName;
        }
        else {
            ofLogNotice() << devices[i].id << ": " << devices[i].deviceName << " - unavailable ";
        }
    }
}


void GuiApp::setup(){
	ofSetVerticalSync(true);

    // START SCREEN

    checkVidGrabberDevices();
	webcamFeatureExtractor.setup();
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

    createMetadatasFiles();
    worker.setup(dir);
    worker.startThread();

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
	details.add(videoR.setup("red", ""));
	details.add(videoG.setup("green", ""));
	details.add(videoB.setup("blue", ""));
	details.add(videoRythm.setup("rythm", ""));

    startButton.addListener(this, &GuiApp::startButtonPressed);
    manageButton.addListener(this, &GuiApp::manageButtonPressed);

    startScreenNav.setup();
    startScreenNav.setPosition(camPreviewOffset, camPreviewOffset + camHeight + 30);
    startScreenNav.add(startButton.setup("START"));
    startScreenNav.add(manageButton.setup("MANAGE"));
	startScreenNav.add(numberOfFaces.setup("num of faces", ""));
	startScreenNav.add(rythmOfCaptured.setup("rythm", ""));

    playVideo();
	ofSetVerticalSync(false);
}

void GuiApp::update(){

    if (startScreenMode) {
        vidGrabber.update();

        if (vidGrabber.isFrameNew())
        {
			faces = webcamFeatureExtractor.detectFaces(vidGrabber);
			numberOfFaces = ofToString(faces.size());

			webcamFeatureExtractor.calculate(vidGrabber);
			rythmOfCaptured = ofToString(webcamFeatureExtractor.getRythm());
        }
    }
    else {
        for (int i = 0; i < (int)thumbnails.size(); i++) {
            thumbnails[i]->video.update();
            thumbnails[i]->update();
        }
        mainPlayer.update();    
    }

}

void GuiApp::draw(){

    if (startScreenMode) {
        vidGrabber.draw(camPreviewOffset, camPreviewOffset);
        startScreenNav.draw();

        for (unsigned int i = 0; i < faces.size(); i++)
        {
            ofRectangle cur = faces[i].boundingRect;
            ofRect(cur.x + camPreviewOffset, cur.y + camPreviewOffset, cur.width, cur.height);
        }

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
		mainPlayer.setVolume(0.0);

        if (xmlHandler.loadFile(ofSplitString(dir.getPath(currentVideo), ".")[0] + ".xml")) {
			
			xmlHandler.pushTag("metadata");
            videoName = xmlHandler.getValue("name", "?");

            double getLumi = xmlHandler.getValue("luminance", -1.0);
            videoLuminance = ofToString(getLumi);

			double getR = xmlHandler.getValue("red", -1.0);
			double getG = xmlHandler.getValue("green", -1.0);
			double getB = xmlHandler.getValue("blue", -1.0);

			videoR = ofToString(getR);
			videoG = ofToString(getG);
			videoB = ofToString(getB);

			double getRythm = xmlHandler.getValue("rythm", -1.0);
			videoRythm = ofToString(getRythm);

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
    if (worker.isThreadRunning())
        worker.stopThread();

	upButton.removeListener(this, &GuiApp::upButtonPressed);
	downButton.removeListener(this, &GuiApp::downButtonPressed);
	playButton.removeListener(this, &GuiApp::playButtonPressed);
	pauseButton.removeListener(this, &GuiApp::pauseButtonPressed);
	stopButton.removeListener(this, &GuiApp::stopButtonPressed);
	addButton.removeListener(this, &GuiApp::addButtonPressed);
}
