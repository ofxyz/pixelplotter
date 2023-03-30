#pragma once

#include "generator.h"

class G_plaids : public Generator {
public:
	G_plaids() {
		name = "Plaids";
	};

	void setup(int width, int height);
	void update();
	void draw();
	void renderImGuiSettings();
	void loadSettings(ofxXmlSettings settings);
	ofxXmlSettings getSettings();
private:
	int w,h;
};