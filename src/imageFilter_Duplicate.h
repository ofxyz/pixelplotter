#pragma once

#include "imageFilter.h"

class If_duplicate : public ImageFilter {
public:
	If_duplicate() {
		name = "Duplicate";
	};

	virtual void apply(ofImage* img);
	virtual void renderImGuiSettings();
	virtual void loadSettings(ofxXmlSettings settings);
	virtual ofxXmlSettings getSettings();

private:
	int hCount = 1;
	int vCount = 1;
	bool bMirror = true;
};