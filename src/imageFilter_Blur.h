#pragma once

#include "imageFilter.h"
#include "ofxOpenCv.h"

class If_blur : public ImageFilter {
public:
	If_blur() {
		name = "Blur";
	};

	virtual void apply(ofImage* img);
	virtual void renderImGuiSettings();
	virtual void loadSettings(ofJson settings);
	virtual ofJson getSettings();

private:
	ofxCvColorImage colorCvImage;
	int cvBlur = 0;
};
