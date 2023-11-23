#pragma once
#include "ofJson.h"
#include "ofxImGui.h"

class ofApp;

class ImageFilter {
public:
	ImageFilter();
	ofApp* pixelplotter{ nullptr };
	bool bAlive = true;
	bool bVisible = true;

	bool moveUp = false;
	bool moveDown = false;

	int currentBlendModeIndex = 0;

	std::string name;

	virtual void apply(ofImage* img) = 0;
	virtual void renderImGuiSettings() = 0;
	virtual void loadSettings(ofJson& settings) = 0;
	virtual ofJson getSettings() = 0;

	void renderUpDownButtons();

	bool isFresh();
	void setFresh(bool fresh);
	bool isAlive();
	void setAlive(bool alive);

private:
	bool bFresh = false;
};
