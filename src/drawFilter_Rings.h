#pragma once

#include "drawFilter.h"
#include "ofxOpenCv.h"

class Df_rings : public DrawFilter {
public:
	ofApp* pixelplotter;
	Df_rings(ofApp* app) {
		pixelplotter = app;
		name = "Rings";
	};

	void draw(ofImage* input, float width = 0, float height = 0, float x = 0, float y = 0);
	void renderImGuiSettings();
	virtual void loadSettings(ofxXmlSettings settings);
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
