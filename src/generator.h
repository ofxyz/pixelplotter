#pragma once
#include "ofxImGui.h"
#include "ofxXmlSettings.h"

class ImageGenerator {
public:
	bool active = true;
	bool visible = true;
	
	std::string name;

	bool bFresh = false;
	bool isFresh() {
		return bFresh;
	}

	virtual void setup(ofImage* input) = 0;
	virtual void update() = 0;
	virtual void draw() = 0;
	virtual void renderImGuiSettings() = 0;
	virtual void loadSettings(ofxXmlSettings settings) = 0;
	virtual ofxXmlSettings getSettings() = 0;

};
