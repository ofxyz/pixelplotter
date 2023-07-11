#pragma once
#include "ofxImGui.h"
#include "ofxXmlSettings.h"

class ImageFilter {
public:
	bool active = true;
	bool visible = true;
	std::string name;

	bool bFresh = false;
	bool isFresh() {
		return bFresh;
	}
	void setFresh(bool fresh) {
		bFresh = fresh;
	}

	virtual void apply(ofImage* img) = 0;
	virtual void renderImGuiSettings() = 0;
	virtual void loadSettings(ofxXmlSettings settings) = 0;
	virtual ofxXmlSettings getSettings() = 0;

};
