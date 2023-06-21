#pragma once

#include "imageFilter.h"
#include "drawPixels.h"

class If_drawPixel : public ImageFilter {
public:
	If_drawPixel() {
		name = "DrawPixel";
	};

	void apply(ofImage* img);
	void renderImGuiSettings();
	void loadSettings(ofxXmlSettings settings);
	ofxXmlSettings getSettings();
	DrawPixels drawPixels;

private:
	int selectedPixelType = 0;
	int hCount = 1;
	int vCount = 1;
	ImVec4 c_col = ofColor(0, 128, 128, 255);
};
