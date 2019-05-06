/*
 * GuiApp.cpp
 *
 *  Created on: Oct 28, 2014
 *      Author: arturo
 */

#include "GuiApp.h"
#include <exception>
#include <filesystem>
#include <iostream>

inline bool endsWith(string const & value, string const & ending)
{
	if (ending.size() > value.size()) return false;
	return equal(ending.rbegin(), ending.rend(), value.rbegin());
}

void copyFile(string const & SRC, string const & DEST)
{
	filesystem::path sourceFile = SRC;
	filesystem::path targetParent = DEST;
	auto target = targetParent / sourceFile.filename(); 

	try 
	{
		filesystem::copy_file(sourceFile, target, filesystem::copy_option::overwrite_if_exists);
	}
	catch (std::exception& e) 
	{
		cout << e.what();
	}
}

string splitFilename(const std::string& str)
{
	size_t found = str.find_last_of("/\\");
	return str.substr(found + 1);
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

	playVideo();

	ofSetVerticalSync(false);
}

void GuiApp::update(){
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

void GuiApp::playVideo() {

    if (currentVideo < thumbnails.size() && currentVideo >= 0) {
        string currentName = thumbnails[currentVideo]->name;

		cout << "tu: " << currentName << endl;

        mainPlayer.load(currentName);
        mainPlayer.setLoopState(OF_LOOP_NORMAL);
        mainPlayer.play();

        if (xmlHandler.loadFile(ofSplitString(currentName, ".")[0] + ".xml")) {
			
			xmlHandler.pushTag("metadata");
            videoName = xmlHandler.getValue("name", "?");
            double getLumi = xmlHandler.getValue("luminance", -1.0);
            videoLuminance = ofToString(getLumi);
            
			if (getLumi == -1.0) {
                if (!waitsForLuminance) {
                    waitsForLuminance = true;
                    luminanceExtractor.setup(currentName);
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
		if (endsWith(path, "mov") || endsWith(path, "mp4")) {
			Thumbnail *t = new Thumbnail();
			thumbnails.push_back(t);

			copyFile(path, dir.getAbsolutePath());
			thumbnails[(int)thumbnails.size() - 1]->setup(dir.path() + splitFilename(path));

            thumbnails[(int)thumbnails.size() - 1]->set(thumbnailsOffset, 20 + 100 + (((int)thumbnails.size() - 1) % 3)
				* (thumbnails[0]->thumbnailSize + 10),
				thumbnails[0]->thumbnailSize, thumbnails[0]->thumbnailSize);

			dir.allowExt("mov");
			dir.allowExt("mp4");
			dir.listDir("videos/");

			checkMetadatas();
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
