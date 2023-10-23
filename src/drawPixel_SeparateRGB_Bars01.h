#pragma once

#include "drawPixel.h"

class Dp_separateRGB_Bars01 : public DrawPixel {
public:
	Dp_separateRGB_Bars01() {
		name = "Separate RGB Bars 01";
		c_magentaRed = ofColor(255, 0, 0, 255);
		c_yellowGreen = ofColor(0, 255, 0, 255);
		c_cyanBlue = ofColor(0, 0, 255, 255);
	}

	Dp_separateRGB_Bars01(ofJson settings) {
		name = "Separate RGB Bars 01";
		c_magentaRed = ofColor(255, 0, 0, 255);
		c_yellowGreen = ofColor(0, 255, 0, 255);
		c_cyanBlue = ofColor(0, 0, 255, 255);
		loadSettings(settings);
	}

	void draw(ofColor c, float offsetX, float offsetY, float width, float height) override;
	void renderImGuiSettings() override;
	void loadSettings(ofJson settings) override;
	ofJson getSettings() override;
};
