#pragma once
#include "ofJson.h"
#include "ofxImGui.h"

class ImageFilter {
public:
	bool active = true;
	bool visible = true;

	std::string name;

	virtual void apply(ofImage* img) = 0;
	virtual void renderImGuiSettings() = 0;
	virtual void loadSettings(ofJson& settings) = 0;
	virtual ofJson getSettings() = 0;

	bool isFresh();
	void setFresh(bool fresh);

private:
	bool bFresh = false;
};
