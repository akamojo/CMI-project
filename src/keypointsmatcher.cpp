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

//        cout << dir.getPath(i) << ": kpts count = " << keypoints_object.size() << endl;
    }
}

void KeypointsMatcher::detectObject(Mat &object, std::vector<KeyPoint> &keypoints_object, Mat &descriptors_object) {

    detector = ORB::create(); // use 200 to get max 200 features ?
    detector->detect(object, keypoints_object);

    extractor = ORB::create();
    extractor->compute(object, keypoints_object, descriptors_object);

}

int KeypointsMatcher::getObjectsCount() {
    return objectsNames.size();
}

vector<string> KeypointsMatcher::getObjectsNames() {
    return objectsNames;
}

vector<size_t> KeypointsMatcher::countObjects(Mat scene) {

    vector<size_t> result;

    std::vector<KeyPoint> keypoints_scene;
    Mat descriptors_scene;
    detectObject(scene, keypoints_scene, descriptors_scene);

    BFMatcher matcher(NORM_L2);

//    matcher.match()
//    matcher = DescriptorMatcher::create("BruteForce");

    vector< DMatch > matches;

    for (int i = 0; i < objectsNames.size(); ++i) {
        matcher.match(objectsDescriptors[i], descriptors_scene, matches);

        double max_dist = 0.0; double min_dist = 1000000.0;

        for (int i = 0; i < objectsDescriptors[i].rows; i++)
        {
            double dist = matches[i].distance;
            if (dist < min_dist) min_dist = dist;
            if (dist > max_dist) max_dist = dist;
        }

        vector< DMatch > good_matches;

        for (int i = 0; i < objectsDescriptors[i].rows; i++)
        {

/*          if (matches[i].distance <= goodMatchCoeff * min_dist)
            {
                good_matches.push_back(matches[i]);
                cout << matches[i].distance << endl;
            }
*/

            if (matches[i].distance < this->maxAbsoluteDistance)
                good_matches.push_back(matches[i]);
        }

        result.push_back(good_matches.size());
    }
    return result;
}
