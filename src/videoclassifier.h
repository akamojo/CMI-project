#ifndef VIDEOCLASSIFIER_H
#define VIDEOCLASSIFIER_H

#include "ofMain.h"
#include <iostream>

enum CategoryEnum
{
	PARTY, CALM, OTHER
};

class VideoClassifier
{
private:
	int luminanceThreshold = 100;
	double rythmThreshold = 0.05;

	const char* CategoryToString(CategoryEnum);
public:
	VideoClassifier();
	string classify(double, double);
};

#endif // CVIDEOCLASSIFIER_H
