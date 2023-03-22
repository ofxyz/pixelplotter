#pragma once

#include "imageFilter.h"

class If_mirror : public ImageFilter {
public:
	If_mirror() {
		name = "Mirror";
	};

	void apply(ofPixels* pix);
	void renderImGuiSettings();
	void loadSettings(ofxXmlSettings settings);
	ofxXmlSettings getSettings();

private:
	bool hMirror = false;
	bool vMirror = false;
};
