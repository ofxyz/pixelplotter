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

	void draw(ofColor c, glm::vec2 dim, glm::vec2 pos = { 0,0 }, glm::vec2 posNorm = { 0,0 }) override;
	void renderImGuiSettings() override;
	void loadSettings(ofJson settings) override;
	ofJson getSettings() override;
};
