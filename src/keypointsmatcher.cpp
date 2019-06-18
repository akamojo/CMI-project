#include "keypointsmatcher.h"

using namespace cv;
using namespace ofxCv;
using namespace std;

KeypointsMatcher::KeypointsMatcher() {

}

void KeypointsMatcher::analyzeObjects(string dirPath) {

    dir.allowExt("png");
    dir.listDir(dirPath);
    dir.sort();

    std::vector<KeyPoint> keypoints_object;
    Mat object, descriptors_object;

    objectsKeypoints.clear();
    objectsDescriptors.clear();
    objectsNames.clear();

    for (int i = 0; i < (int)dir.size(); i++) {
        string path = dir.getAbsolutePath() + "/" + dir.getName(i);
        object = imread(path, CV_LOAD_IMAGE_GRAYSCALE);
        if (!object.data) {
            cout << "KptsMatcher: failed to load " << path << "!" << endl;
            continue;
        }

        detectObject(object, keypoints_object, descriptors_object);

        objectsKeypoints.push_back(keypoints_object);
        objectsDescriptors.push_back(descriptors_object);
        objectsNames.push_back(dir.getPath(i));

    }
}

void KeypointsMatcher::detectObject(Mat &object, std::vector<KeyPoint> &keypoints_object, Mat &descriptors_object) {

    Mat typedObj;
    object.convertTo(typedObj, CV_8U);

    detector = ORB::create();
    detector->detect(typedObj, keypoints_object);

    extractor = ORB::create();
    extractor->compute(typedObj, keypoints_object, descriptors_object);

}

int KeypointsMatcher::getObjectsCount() {
    return objectsNames.size();
}

vector<string> KeypointsMatcher::getObjectsNames() {
    return objectsNames;
}

vector<double> KeypointsMatcher::getBestMatch(Mat scene) {
    vector<double> result(2, -1.0);

    std::vector<KeyPoint> keypoints_scene;
    Mat descriptors_scene;
    detectObject(scene, keypoints_scene, descriptors_scene);

    if (keypoints_scene.size() == 0) {
        return result;
    }

    BFMatcher matcher(NORM_L2);

    vector< DMatch > matches;
    int closest_distance = 10000000.0;
    int closest_match = -1;

    // iterate through objects
    for (int i = 0; i < objectsNames.size(); ++i) {

        matcher.match(objectsDescriptors[i], descriptors_scene, matches);

        double max_dist = 0.0; double min_dist = 1000000.0;
        vector< DMatch > good_matches;

        // iterate through matches' distances
        for (int j = 0; j < matches.size(); j++)
        {
            double dist = matches[j].distance;

            if (dist < closest_distance) {
                closest_distance = dist;
                closest_match = i;
            }
        }
    }

    result[0] = closest_match;
    result[1] = closest_distance;

    return result;
}

vector<size_t> KeypointsMatcher::countObjects(Mat scene) {

    vector<size_t> result(objectsNames.size(), 0);

    std::vector<KeyPoint> keypoints_scene;
    Mat descriptors_scene;
    detectObject(scene, keypoints_scene, descriptors_scene);

    if (keypoints_scene.size() == 0) {
        return result;
    }

    cout << " analyzed scene ..." << scene.rows << " x " << scene.cols << endl;

    BFMatcher matcher(NORM_L2);

    vector< DMatch > matches;
    int closest_distance = 10000000.0;
    int closest_match = -1;

    // iterate through objects
    for (int i = 0; i < objectsNames.size(); ++i) {

        matcher.match(objectsDescriptors[i], descriptors_scene, matches);

        double max_dist = 0.0; double min_dist = 1000000.0;
        vector< DMatch > good_matches;

        // iterate through matches' distances
        for (int j = 0; j < matches.size(); j++)
        {
            double dist = matches[j].distance;
            if (dist < min_dist)
                min_dist = dist;
            if (dist > max_dist) max_dist = dist;

            if (dist < closest_distance) {
                closest_distance = dist;
                closest_match = i;
            }

            if (matches[j].distance < this->maxAbsoluteDistance)
                good_matches.push_back(matches[j]);

        }
        result[i] = good_matches.size();
    }
    return result;
}
