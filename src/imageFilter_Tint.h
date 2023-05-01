#pragma once

#include "imageFilter.h"

class If_tint : public ImageFilter {
public:
	If_tint() {
		name = "Tint";
	};

	void apply(ofImage* img);
	void renderImGuiSettings();
	void loadSettings(ofxXmlSettings settings);
	ofxXmlSettings getSettings();

private:
	ImVec4 c_Tint = ofColor(255, 255, 255, 255);
	bool bGrey = false;
};
