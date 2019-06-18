#include "videoclassifier.h"

VideoClassifier::VideoClassifier() {
}

const char* VideoClassifier::CategoryToString(CategoryEnum v)
{
	switch (v)
	{
	case PARTY:   return "party";
	case CALM:   return "calm";
	case OTHER: return "other";
	default:      return "[Unknown]";
	}
}

std::string VideoClassifier::classify(double lumi, double rythm) {
	if (lumi < luminanceThreshold && rythm > rythmThreshold)
		return CategoryToString(PARTY);
	else if (lumi > luminanceThreshold && rythm < rythmThreshold)
		return CategoryToString(CALM);
	else
		return CategoryToString(OTHER);
}