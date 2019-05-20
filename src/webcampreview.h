#ifndef WEBCAMPREVIEW_H
#define WEBCAMPREVIEW_H

#include <ofVideoGrabber.h>
#include "ofMain.h"

class WebCamPreview : ofRectangle {

    ofVideoGrabber vidGrabber;
    int camWidth;
    int camHeight;

public:
    WebCamPreview();
    void update();

    void setup(int cW, int cH);
    void draw(int x, int y);
};

#endif // WEBCAMPREVIEW_H
