#pragma once
#include "ofMain.h"
#include "ofxImGui.h"

class ImageFilter {
public:
	bool active = true;
	bool visible = true;
	std::string name;

	bool bFresh = false;
	bool isFresh();
	void setFresh(bool fresh);

	virtual void apply(ofImage* img) = 0;
	virtual void renderImGuiSettings() = 0;
	virtual void loadSettings(ofJson settings) = 0;
	virtual ofJson getSettings() = 0;

};
