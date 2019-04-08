#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){

	dir.listDir("videos/");
	dir.allowExt("mov");
	dir.sort(); // in linux the file system doesn't return file lists ordered in alphabetical order

	//allocate the vector to have as many ofImages as files
	if( dir.size() ){
		images.assign(dir.size(), ofVideoPlayer());
	}

	// you can now iterate through the files and load them into the ofImage vector
	for(int i = 0; i < (int)dir.size(); i++){
		cout << dir.getPath(i) << endl;
		images[i].load(dir.getPath(i));
		images[i].setLoopState(OF_LOOP_NORMAL);
		images[i].play();
		images[i].setPaused(true);
	}
	currentImage = 0;
	images[currentImage].setPaused(false);

	ofBackground(ofColor::white);
}

//--------------------------------------------------------------
void ofApp::update(){
	for (int i = 0; i < (int)images.size(); i++) {
		images[i].update();
	}
}

//--------------------------------------------------------------
void ofApp::draw(){

	/*if (dir.size() > 0){
		ofSetColor(ofColor::white);
		images[currentImage].draw(300,50);
		//images[currentImage].setPaused(false);

		ofSetColor(ofColor::gray);
		string pathInfo = dir.getName(currentImage) + " " + dir.getPath(currentImage) + "\n\n" +
			"press any key to advance current image\n\n" +
			"many thanks to hikaru furuhashi for the OFs";
		ofDrawBitmapString(pathInfo, 300, images[currentImage].getHeight() + 80);
	}

	ofSetColor(ofColor::gray);
	for(int i = 0; i < (int)dir.size(); i++){
		if(i == currentImage) {
			ofSetColor(ofColor::red);
		}	else {
			ofSetColor(ofColor::black);
		}
		string fileInfo = "file " + ofToString(i + 1) + " = " + dir.getName(i);
		ofDrawBitmapString(fileInfo, 50,i * 20 + 50);
	}*/

	if (dir.size() > 0) {
		ofSetColor(ofColor::white);
		
		for (int i = 0; i < (int)images.size(); i++) {
			images[i].draw(20, 20 + i * (images[0].getHeight() + 10));
			
			/*vidGrabber.draw(20, 20);
			videoTexture.draw(20 + camWidth, 20, camWidth, camHeight);

			fingerMovie.draw(20 + 2 * camWidth, 20, 2 * camWidth, 2 * camHeight);*/
		}
		
	}

}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	if (dir.size() > 0){
		images[currentImage].setPaused(true);
		currentImage++;
		currentImage %= dir.size();

		images[currentImage].setPaused(false);
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
