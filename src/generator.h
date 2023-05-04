#pragma once
#include "ofxImGui.h"
#include "ofxXmlSettings.h"

class ofApp;

class Generator {
public:
	std::string name;
	int width;
	int height;

	//ofFbo genFbo;
	//ofPixels genPix;
	ImVec4 c_bg = ofColor(255, 255, 255, 255);

	bool active = true;
	bool visible = true;
	bool moveUp = false;
	bool moveDown = false;
	bool bFresh = false;
	bool isFresh() {
		return bFresh;
	}
	void setFresh(bool fresh) {
		bFresh = fresh;
	}

	virtual void setup(int width, int height) = 0;
	virtual void update() = 0;
	virtual void draw() = 0;
	virtual void renderImGuiSettings() = 0;
	virtual void loadSettings(ofxXmlSettings settings) = 0;
	virtual ofxXmlSettings getSettings() = 0;

};
