#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){

	/*dir.listDir("videos/");
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
	videos[currentVideo].setPaused(false);*/

	camWidth = 320;  // try to grab at this size.
	camHeight = 240;

	//get back a list of devices.
	vector<ofVideoDevice> devices = vidGrabber.listDevices();

	for (size_t i = 0; i < devices.size(); i++) {
		if (devices[i].bAvailable) {
			//log the device
			ofLogNotice() << devices[i].id << ": " << devices[i].deviceName;
		}
		else {
			//log the device and note it as unavailable
			ofLogNotice() << devices[i].id << ": " << devices[i].deviceName << " - unavailable ";
		}
	}

	vidGrabber.setDeviceID(0);
	vidGrabber.setDesiredFrameRate(60);
	vidGrabber.initGrabber(camWidth, camHeight);

	finder.setup("haarcascade_frontalface_default.xml");

	ofBackground(0);

}

//--------------------------------------------------------------
void ofApp::update(){

	/*for (int i = 0; i < (int)videos.size(); i++) {
		videos[i].update();
	}*/

	vidGrabber.update();

	if (vidGrabber.isFrameNew())
	{
		colorImg.setFromPixels(vidGrabber.getPixels());
		finder.findHaarObjects(colorImg);
	}
}

//--------------------------------------------------------------
void ofApp::draw(){

	/*if (dir.size() > 0) {
		ofSetColor(ofColor::white);

		for (int i = 0; i < (int)videos.size(); i++) {
			videos[i].draw(20, 20 + i * (videos[0].getHeight() + 10));
		}
	}*/

	//vidGrabber.draw(videos[0].getWidth() + 40, 20);
	vidGrabber.draw(0, 0);

	ofSetHexColor(0xffffff);
	ofNoFill();

	for (unsigned int i = 0; i < finder.blobs.size(); i++)
	{
		ofRectangle cur = finder.blobs[i].boundingRect;
		ofRect(cur.x, cur.y, cur.width, cur.height);
	}
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	/*if (dir.size() > 0) {
		videos[currentVideo].setPaused(true);
		currentVideo++;
		currentVideo %= dir.size();

		videos[currentVideo].setPaused(false);
	}*/
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
