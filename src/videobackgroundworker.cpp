#include "videobackgroundworker.h"

void VideoBackgroundWorker::setup(ofDirectory dir)
{
    this->metadatasDir = dir;
}

void VideoBackgroundWorker::threadedFunction()
{
    for (int i = 0; i < (int)metadatasDir.size(); i++) {

        vector<string> spl = ofSplitString(metadatasDir.getPath(i), ".");
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

            xmlHandler.popTag();

            xmlHandler.saveFile(xmlFilePath);
            xmlHandler.clear();

            ofLog(OF_LOG_NOTICE, xmlFilePath + " metadata file created");
        }
    }
}
