#pragma once
#include "ofJson.h"
#include "ofxImGui.h"

class ofApp;

class Generator {
public:
	Generator();
	ofApp* pixelplotter{ nullptr };
	bool bAlive = true;
	bool bVisible = true;

	bool moveUp = false;
	bool moveDown = false;

	std::string name;

	int width;
	int height;
	ImVec4 c_bg;

	virtual void setup(int width, int height) = 0;
	virtual void update() = 0;
	virtual void draw() = 0;
	virtual void renderImGuiSettings() = 0;
	virtual void loadSettings(ofJson& settings) = 0;
	virtual ofJson getSettings() = 0;

	bool isFresh();
	void setFresh(bool fresh);
	bool isAlive();
	void setAlive(bool alive);

private:
	bool bFresh = false;
};
