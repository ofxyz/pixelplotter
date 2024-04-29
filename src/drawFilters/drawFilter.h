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
	bool _isOpen = true;

	bool moveUp = false;
	bool moveDown = false;
	bool duplicate = false;

	int currentBlendModeIndex = 0;
	bool useMask = false;
	int maskMargin = 0;
	
	std::string name;

	// Draw always draws from home position (0,0) 
	virtual void draw(ofImage* input, float width = 0, float height = 0) = 0;
	virtual void renderImGuiSettings() = 0;
	virtual void loadSettings(ofJson& settings) = 0;
	virtual ofJson getSettings() = 0;

	void renderUpDownButtons();

	// -------------------------------------------------
	// TODO: Set BlendMode should be ofSetBlendMode(BLENDMODES[currBlendmodeIndex])
	std::vector<std::string> v_BlendModes;
	void setBlendMode();

	// -------------------------------------------------
	// TODO: Finish implementing mask
	ImVec4 c_mask = ofColor(255, 255, 255, 255);
	void renderImGuiColourSettings(bool colors, bool mask);

	float lineWidth = 8;

	bool isFresh();
	void setFresh(bool fresh);
	bool isAlive();
	void setAlive(bool alive);

private:
	bool bFresh = false;
};
