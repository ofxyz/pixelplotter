#pragma once
#include "ofJson.h"
#include "ofColor.h"
#include "ofxImGui.h"

class ofApp;

class DrawFilter {
public:
	DrawFilter();
	ofApp* pixelplotter{ nullptr };
	bool bAlive = true;
	bool bVisible = true;

	bool moveUp = false;
	bool moveDown = false;

	bool useMask = false;
	int maskMargin = 0;

	std::string name;

	virtual void draw(ofImage* input, float width = 0, float height = 0, float x = 0, float y = 0) = 0;
	virtual void renderImGuiSettings() = 0;
	virtual void loadSettings(ofJson& settings) = 0;
	virtual ofJson getSettings() = 0;

	void renderUpDownButtons();

	// -------------------------------------------------
	// TODO: Delete after implementing drawPixel class ...
	glm::vec4 getCMYK(ofColor rgb);

	int currentBlendModeIndex = 0;
	std::vector<std::string> v_BlendModes;

	// -------------------------------------------------
	// TODO: Delete after implementing drawPixel class ...
	void gui_setGrey();

	void gui_setRGB();

	void gui_setCMYK();

	void gui_setRYB();

	void renderImGuiColourSettings(bool colors, bool mask);

	void setBlendMode();

	float lineWidth = 8;

	ImVec4 c_cyanBlue    = ofColor(0, 174, 239, 255);
	ImVec4 c_magentaRed  = ofColor(236, 0, 140, 255);
	ImVec4 c_yellowGreen = ofColor(255, 242, 0, 255);
	ImVec4 c_black = ofColor(0, 0, 0, 255);
	ImVec4 c_white = ofColor(255, 255, 255, 255);
	ImVec4 c_mask  = ofColor(255, 255, 255, 255);

	bool isFresh();
	void setFresh(bool fresh);
	bool isAlive();
	void setAlive(bool alive);

private:
	bool bFresh = false;
};
