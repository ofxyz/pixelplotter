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

	Dp_separateRGB_Bars01(ofxXmlSettings settings) {
		Dp_separateRGB_Bars01();
		loadSettings(settings);
	}

	virtual void draw(ofColor c, float offsetX, float offsetY, float width, float height);
	virtual void renderImGuiSettings();
	virtual void loadSettings(ofxXmlSettings settings);
	virtual ofxXmlSettings getSettings();
};
