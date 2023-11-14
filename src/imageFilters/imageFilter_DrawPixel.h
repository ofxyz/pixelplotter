#pragma once

#include "imageFilter.h"
#include "drawPixel_Controller.h"

class If_drawPixel : public ImageFilter {
public:
	If_drawPixel() {
		name = "DrawPixel";
	};

	void apply(ofImage* img) override;
	void renderImGuiSettings() override;
	void loadSettings(ofJson& settings) override;
	ofJson getSettings() override;

	DrawPixelController drawPixels;

private:
	int selectedPixelType = 0;
	int hCount = 1;
	int vCount = 1;
	bool bMirror = true;
	ImVec4 c_col = ofColor(0, 128, 128, 255);
};
