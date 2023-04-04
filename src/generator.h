#pragma once
#include "ofxImGui.h"
#include "ofxXmlSettings.h"

class Generator {
public:
	std::string name;
	bool active = true;
	bool visible = true;
	bool bFresh = false;
	bool isFresh() {
		return bFresh;
	}

	virtual void setup(int width, int height) = 0;
	virtual void update() = 0;
	virtual void draw() = 0;
	virtual void renderImGuiSettings() = 0;
	virtual void loadSettings(ofxXmlSettings settings) = 0;
	virtual ofxXmlSettings getSettings() = 0;
private:
	int w;
	int h;
};