#pragma once
#include <string>
#include <vector>
#include "ofxImGui.h"
#include "ofJson.h"
#include "ofx2d.h"

class ofApp;

class swatchController {
public:
	std::vector<ofColor*> palette;
	std::vector<std::string> v_ColorNames;
	std::vector<ImVec4> v_ColorValues;

	void addColor(ofColor color);
	void addColor(ofColor color, std::string name);

	ofJson getSettings();
	void loadSettings(ofJson settings);

	void renderImGuiSettings();

	bool isFresh();
	void setFresh(bool fresh);

private:
	bool _bFresh = false;
};
