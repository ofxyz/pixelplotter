#pragma once

#include "generator.h"

class G_plaids : public Generator {
public:
	ofApp* pixelplotter;
	G_plaids(ofApp* app) {
		pixelplotter = app;
		name = "Rings";
	};

	void setup(int width, int height);
	void update();
	void draw();
	void renderImGuiSettings();
	void loadSettings(ofxXmlSettings settings);
	ofxXmlSettings getSettings();
private:
	int width,height;
	int xcount = 1;
	int ycount = 1;
	ImVec4 c_base = ofColor(0, 255, 255, 255);
};