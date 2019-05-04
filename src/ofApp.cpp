#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){

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

	vidGrabber.update();

	if (vidGrabber.isFrameNew())
	{
		colorImg.setFromPixels(vidGrabber.getPixels());
		finder.findHaarObjects(colorImg);
	}

//    ofBackground(100, 100, 100);
    vidGrabber.update();
}

//--------------------------------------------------------------
void ofApp::draw(){

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
