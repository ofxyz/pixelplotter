#pragma once

#include "drawPixel.h"

class Dp_separateCMYK_Bars01 : public DrawPixel {
public:
	Dp_separateCMYK_Bars01() {
		name = "Separate CMYK Bars 01";
	}

	Dp_separateCMYK_Bars01(ofxXmlSettings settings) {
		Dp_separateCMYK_Bars01();
		loadSettings(settings);
	}

	virtual void draw(ofColor c, float offsetX, float offsetY, float width, float height);
	virtual void renderImGuiSettings();
	virtual void loadSettings(ofxXmlSettings settings);
	virtual ofxXmlSettings getSettings();

};
