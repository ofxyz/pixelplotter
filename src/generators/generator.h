#pragma once
#include "ofApp.h" // TODO: Only import ImGui?

class Generator {
public:
	Generator();
	std::string name;
	int width;
	int height;
	ImVec4 c_bg;

	bool active = true;
	bool visible = true;
	bool moveUp = false;
	bool moveDown = false;
	bool bFresh = false;
	bool isFresh();
	void setFresh(bool fresh);

	virtual void setup(int width, int height) = 0;
	virtual void update() = 0;
	virtual void draw() = 0;
	virtual void renderImGuiSettings() = 0;
	virtual void loadSettings(ofJson& settings) = 0;
	virtual ofJson getSettings() = 0;

};
