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

        xmlHandler.clear();
    }
}

void VideoBackgroundWorker::updateXMLWithVector(string path, string tag, string subTag, vector<double> values, double missingValue) {
    string xmlFilePath = ofSplitString(path, ".")[0] + ".xml";
    if (xmlHandler.loadFile(xmlFilePath)) {

        xmlHandler.pushTag("metadata");
        xmlHandler.addTag(tag);
        xmlHandler.pushTag(tag);

        double getCurrentValue = xmlHandler.getValue(subTag, missingValue);
        if (getCurrentValue == missingValue) {
            for (size_t i = 0; i < values.size(); ++i) {
                xmlHandler.setValue(subTag, values[i], i);
            }
        }
        else if (getCurrentValue != missingValue) {
            ofLog(OF_LOG_WARNING, "Tag " + tag + " to update is already set ");
        }
        else {
            for (size_t i = 0; i < values.size(); ++i) {
                xmlHandler.addValue(tag, values[i]);
                ofLog(OF_LOG_NOTICE, "Writing " + ofToString(values[i]) + " tag ");
            }
        }

        xmlHandler.popTag();
        xmlHandler.popTag();

        xmlHandler.saveFile(xmlFilePath);

        xmlHandler.clear();
    }
}

void VideoBackgroundWorker::threadedFunction()
{
    for (int i = 0; i < (int)videosDir.size(); i++) {

        string videoName = videosDir.getPath(i);
        vector<string> spl = ofSplitString(videoName, ".");
        string xmlFilePath = spl[0] + ".xml";

        xmlHandler.clear();
        ofLog(OF_LOG_NOTICE, "[BG Worker] Checking video " + videoName);
        if (xmlHandler.loadFile(xmlFilePath)) {

            xmlHandler.pushTag("metadata");
            double getLumi = xmlHandler.getValue("luminance", -1.0);
            double getRed = xmlHandler.getValue("red", -1.0);
			double getRythm = xmlHandler.getValue("rythm", -1.0);

            if (getLumi == -1.0 || getRed == -1.0 || getRythm == -1.0) {

                ofLog(OF_LOG_NOTICE, "[BG Worker] Calculating features ");

                extractor.setup(videoName);
                extractor.calculate();

                getLumi = extractor.getLuminance();
                updateXML(videoName, "luminance", getLumi, -1.0);
                ofLog(OF_LOG_NOTICE, "[BG Worker] Updated XML with " + ofToString(getLumi));

                vector<double> avgColors = extractor.getAvgColors();
                updateXML(videoName, "red", avgColors[0], -1.0);
                updateXML(videoName, "green", avgColors[1], -1.0);
                updateXML(videoName, "blue", avgColors[2], -1.0);
                ofLog(OF_LOG_NOTICE, "[BG Worker] Updated XML with " + ofToString(avgColors[0]) + "," + ofToString(avgColors[1]) + "," + ofToString(avgColors[2]));

				getRythm = extractor.getRythm();
				updateXML(videoName, "rythm", getRythm, -1.0);
				ofLog(OF_LOG_NOTICE, "[BG Worker] Updated XML with " + ofToString(getRythm));

                vector<double> edgeDistribution = extractor.getEdgeDistribution();
                updateXMLWithVector(videoName, "edgeHistogram", "edge", edgeDistribution, -1);
                ofLog(OF_LOG_NOTICE, "[BG Worker] Updated XML with edge distribution");

            }

        }
    }
    this->workFinished = true;
}
