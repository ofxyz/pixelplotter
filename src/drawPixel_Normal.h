#pragma once

#include "drawPixel.h"

class Dp_normal : public DrawPixel {
public:
	Dp_normal() {
		name = "Normal";
	}

	void draw(ofColor c, float offsetX, float offsetY, float width, float height);
	void renderImGuiSettings();
	void loadSettings(ofxXmlSettings settings);
	ofxXmlSettings getSettings();

};
