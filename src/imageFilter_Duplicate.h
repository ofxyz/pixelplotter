#pragma once

#include "imageFilter.h"

void apply(ofImage* img);
void renderImGuiSettings();
void loadSetti
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
};