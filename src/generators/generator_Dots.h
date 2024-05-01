#pragma once

#include "generator.h"

class G_dots : public Generator {
public:
	G_dots() {
		pixelplotter = (ofApp*)ofGetAppPtr();
		name = "Dots";
		setup(100, 100);
	};

	G_dots(ofJson& settings) {
		pixelplotter = (ofApp*)ofGetAppPtr();
		name = "Dots";
		loadSettings(settings);
	};

	void setup(int width, int height) override;
	void update() override;
	void draw() override;

	void renderImGuiSettings() override;
	void loadSettings(ofJson& settings) override;
	ofJson getSettings() override;

private:
	void drawPattern();
	int circleRes = 6;
	float diameter = 50;
	float rotation = 0;
	int countX = 1;
	int countY = 1;
	ImVec4 c_Dots = ofColor(0, 0, 0, 255);
};
