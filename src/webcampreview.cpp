#include "webcampreview.h"

WebCamPreview::WebCamPreview() {

}

void WebCamPreview::setup(int cW, int cH){

    camWidth = cW;  // try to grab at this size.
    camHeight = cH;

    vector<ofVideoDevice> devices = vidGrabber.listDevices();

    for (size_t i = 0; i < devices.size(); i++) {
        if (devices[i].bAvailable) {
            ofLogNotice() << devices[i].id << ": " << devices[i].deviceName;
        }
        else {
            ofLogNotice() << devices[i].id << ": " << devices[i].deviceName << " - unavailable ";
        }
    }

    vidGrabber.setDeviceID(0);
    vidGrabber.setDesiredFrameRate(60);
    vidGrabber.initGrabber(camWidth, camHeight);

}

void WebCamPreview::update() {
    vidGrabber.update();
}

void WebCamPreview::draw(int x, int y) {
    vidGrabber.draw(x, y);
}
