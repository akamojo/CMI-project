#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){

    ofBackground(ofColor::whiteSmoke);

    this->setupWebcam();

	dir.listDir("videos/");
	dir.allowExt("mov");
	dir.sort(); // in linux the file system doesn't return file lists ordered in alphabetical order

	//allocate the vector to have as many ofImages as files
	if( dir.size() ){
        videoPlayers.assign(dir.size(), ofVideoPlayer());
	}

	// you can now iterate through the files and load them into the ofImage vector
	for(int i = 0; i < (int)dir.size(); i++){
		cout << dir.getPath(i) << endl;
        videoPlayers[i].load(dir.getPath(i));
        videoPlayers[i].setLoopState(OF_LOOP_NORMAL);
        videoPlayers[i].play();
        videoPlayers[i].setPaused(true);
	}
    currentVideo = 0;
    videoPlayers[currentVideo].setPaused(false);

}

//--------------------------------------------------------------
void ofApp::setupWebcam() {
    camWidth = 200;  // try to grab at this size.
    camHeight = 200;

    //get back a list of devices.
    vector<ofVideoDevice> devices = vidGrabber.listDevices();

    for (size_t i = 0; i < devices.size(); i++){
        if (devices[i].bAvailable){
            //log the device
            ofLogNotice() << devices[i].id << ": " << devices[i].deviceName;
        } else {
            //log the device and note it as unavailable
            ofLogNotice() << devices[i].id << ": " << devices[i].deviceName << " - unavailable ";
        }
    }

    vidGrabber.setDeviceID(0);
    vidGrabber.setDesiredFrameRate(60);
    vidGrabber.initGrabber(camWidth, camHeight);

    ofSetVerticalSync(true);
}

//--------------------------------------------------------------
void ofApp::update(){
    for (int i = 0; i < (int)videoPlayers.size(); i++) {
        videoPlayers[i].update();
	}

//    ofBackground(100, 100, 100);
    vidGrabber.update();
}

//--------------------------------------------------------------
void ofApp::draw(){

    int vWidth = 100;
    int vHeight = 100;

    vidGrabber.draw(200, 200);

//	if (dir.size() > 0) {
//		ofSetColor(ofColor::white);
		
//        for (int i = 0; i < (int)videoPlayers.size(); i++) {
////			videoPlayers[i].draw(20, 20 + i * (videoPlayers[0].getHeight() + 10) );
//            videoPlayers[i].draw(20, 20 + i * (vHeight + 10), vWidth, vHeight );

//			/*vidGrabber.draw(20, 20);
//			videoTexture.draw(20 + camWidth, 20, camWidth, camHeight);

//			fingerMovie.draw(20 + 2 * camWidth, 20, 2 * camWidth, 2 * camHeight);*/
//		}
		
//	}

}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	if (dir.size() > 0){
        videoPlayers[currentVideo].setPaused(true);
        currentVideo++;
        currentVideo %= dir.size();

        videoPlayers[currentVideo].setPaused(false);
	}
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
