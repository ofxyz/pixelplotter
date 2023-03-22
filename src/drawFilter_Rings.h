#pragma once

#include "drawFilter.h"
#include "ofxOpenCv.h"

class Df_rings : public DrawFilter {
public:
	Df_rings() {
		name = "Rings";
	};
	void draw(ofImage* input);
	void renderImGuiSettings();
	void loadSettings(ofxXmlSettings settings);
	ofxXmlSettings getSettings();

private:
	int cvThresh = 128;
	int cvBlur = 150;
	int cvSteps = 10;
	ImVec4 c_blob = ofColor(0, 0, 0, 255);

	ofxCvContourFinder contourFinder;

	ofxCvColorImage colorCvImage;
	ofxCvGrayscaleImage greyCvImage;
	ofxCvGrayscaleImage greyCvBlur;
	ofxCvGrayscaleImage greyCvThresh;
};
