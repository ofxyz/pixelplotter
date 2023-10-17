#pragma once

#include "generator.h"

class G_plaids : public Generator {
public:
	ofApp* pixelplotter;
	G_plaids(ofApp* app);;

	virtual void setup(int width, int height);
	virtual void update();
	virtual void draw();
	virtual void renderImGuiSettings();
	virtual void loadSettings(ofJson settings);
	virtual ofJson getSettings();
private:
	int width, height;
	int xcount = 1;
	int ycount = 1;
	ImVec4 c_base = ofColor(0, 255, 255, 255);
};
