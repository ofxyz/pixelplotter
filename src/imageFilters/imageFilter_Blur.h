#pragma once

#include "imageFilter.h"
#include "ofxOpenCv.h"

class If_blur : public ImageFilter {
public:
	If_blur() {
		name = "Blur";
	};

	void apply(ofImage* img) override;
	void renderImGuiSettings() override;
	void loadSettings(ofJson& settings) override;
	ofJson getSettings() override;

private:
	ofxCvColorImage colorCvImage;
	int cvBlur = 0;
};
