#pragma once

#include "drawPixel.h"

class Dp_separateCMYK_Bars01 : public DrawPixel {
public:
	Dp_separateCMYK_Bars01() {
		name = "Separate CMYK Bars 01";
	}

	void draw(ofColor c, float offsetX, float offsetY, float width, float height);
	void renderImGuiSettings();
	void loadSettings(ofxXmlSettings settings);
	ofxXmlSettings getSettings();

};
