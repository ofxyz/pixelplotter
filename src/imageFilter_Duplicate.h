#pragma once

#include "imageFilter.h"

class If_duplicate : public ImageFilter {
public:
	If_duplicate() {
		name = "Duplicate";
	};

	void apply(ofImage* img);
	void renderImGuiSettings();
	void loadSettings(ofxXmlSettings settings);
	ofxXmlSettings getSettings();

private:
	int hCount = 1;
	int vCount = 1;
	bool bMirror = true;
	ImVec4 c_BG = ofColor(255, 255, 255, 255);
};