#pragma once

#include "imageFilter.h"

class If_mirror : public ImageFilter {
public:
	If_mirror() {
		name = "Mirror";
	};

	virtual void apply(ofImage* img);
	virtual void renderImGuiSettings();
	virtual void loadSettings(ofxXmlSettings settings);
	virtual ofxXmlSettings getSettings();

private:
	bool hMirror = false;
	bool vMirror = false;
};
