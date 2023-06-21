#pragma once

#include "drawPixel.h"

class Dp_separateCMYK : public DrawPixel {
public:
	Dp_separateCMYK() {
		name = "Separate CMYK";
	}

	void draw(ofColor c, float offsetX, float offsetY, float width, float height);
	void renderImGuiSettings();
	void loadSettings(ofxXmlSettings settings);
	ofxXmlSettings getSettings();
	
	int currentSubType = 0;
	std::vector<std::string> v_pixelType {
			"Soft Bars",
			"Soft Squares",
			"Soft Triangles"
	};
};
