#pragma once

#include "drawFilter.h"
#include "ofxOpenCv.h"

class Df_rings : public DrawFilter {
public:
	Df_rings() {
		pixelplotter = (ofApp*)ofGetAppPtr();
		name = "Rings";
	};

	Df_rings(ofJson settings) {
		Df_rings();
		loadSettings(settings);
	};

	void draw(ofImage* input, float width = 0, float height = 0, float x = 0, float y = 0) override;
	void renderImGuiSettings() override;
	void loadSettings(ofJson settings) override;
	ofJson getSettings() override;

private:
	int cvThresh = 128;
	int cvBlur = 150;
	int cvSteps = 10;
	float sBlurMapTo = 1;
	float sStepMapTo = 1;

	ImVec4 c_blob = ofColor(0, 0, 0, 255);

	ofxCvContourFinder contourFinder;

	ofxCvColorImage colorCvImage;
	ofxCvGrayscaleImage greyCvImage;
	ofxCvGrayscaleImage greyCvBlur;
	ofxCvGrayscaleImage greyCvThresh;
};
