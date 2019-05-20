#include "videobackgroundworker.h"

VideoBackgroundWorker::VideoBackgroundWorker()
{

}

void VideoBackgroundWorker::setup(ofDirectory dir)
{
    this->videosDir = dir;
    this->workFinished = false;
}

bool VideoBackgroundWorker::isWorking()
{
    return !this->workFinished;
}

void VideoBackgroundWorker::updateXML(string path, string tag, double value, double missingValue) {

    string xmlFilePath = ofSplitString(path, ".")[0] + ".xml";
    if (xmlHandler.loadFile(xmlFilePath)) {

        xmlHandler.pushTag("metadata");

        double getCurrentValue = xmlHandler.getValue(tag, missingValue);
        if (getCurrentValue == missingValue) {
            xmlHandler.setValue(tag, value);
            ofLog(OF_LOG_WARNING, "Replaced missing tag " + tag + " ...");
        }
        else if (getCurrentValue != missingValue) {
            ofLog(OF_LOG_WARNING, "Tag " + tag + " to update is already set ");
        }
        else {
            xmlHandler.addValue(tag, value);
            ofLog(OF_LOG_NOTICE, "Writing " + ofToString(value) + " tag ");
        }

        xmlHandler.popTag();
        xmlHandler.saveFile(xmlFilePath);
    }
}

void VideoBackgroundWorker::threadedFunction()
{
    for (int i = 0; i < (int)videosDir.size(); i++) {

        string videoName = videosDir.getPath(i);
        vector<string> spl = ofSplitString(videoName, ".");
        string xmlFilePath = spl[0] + ".xml";

        ofLog(OF_LOG_NOTICE, "[BG Worker] Checking video " + videoName);
        if (xmlHandler.loadFile(xmlFilePath)) {

            xmlHandler.pushTag("metadata");
            double getLumi = xmlHandler.getValue("luminance", -1.0);

            if (getLumi == -1.0) {

                ofLog(OF_LOG_NOTICE, "[BG Worker] Calculating Luminance ");

                extractor.setup(videoName);
                extractor.calculate();
                getLumi = extractor.getLuminance();
                updateXML(videoName, "luminance", getLumi, -1.0);

                ofLog(OF_LOG_NOTICE, "[BG Worker] Updated XML with " + ofToString(getLumi));

            }

        }
    }
    this->workFinished = true;
}
