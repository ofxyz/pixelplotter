#pragma once

#include "imageFilter.h"
#include "ofxOpenCv.h"

class If_blur : public ImageFilter {
public:
	If_blur() {
		name = "Blur";
	};

	void apply(ofImage* img);
	void renderImGuiSettings();
	void loadSettings(ofxXmlSettings settings);
	ofxXmlSettings getSettings();

private:
	ofxCvColorImage colorCvImage;
	int cvBlur = 0;
};
