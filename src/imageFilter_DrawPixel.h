#pragma once

#include "imageFilter.h"
#include "drawPixels.h"

class If_drawPixel : public ImageFilter {
public:
	If_drawPixel() {
		name = "DrawPixel";
	};

	virtual void apply(ofImage* img);
	virtual void renderImGuiSettings();
	virtual void loadSettings(ofJson settings);
	virtual ofJson getSettings();

	DrawPixels drawPixels;

private:
	int selectedPixelType = 0;
	int hCount = 1;
	int vCount = 1;
	bool bMirror = true;
	ImVec4 c_col = ofColor(0, 128, 128, 255);
};
