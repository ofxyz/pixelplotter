#pragma once

#include "generator.h"

class G_plaids : public Generator {
public:
	G_plaids() {
		pixelplotter = (ofApp*)ofGetAppPtr();
		name = "Plaids";
	};

	G_plaids(ofJson& settings) {
		pixelplotter = (ofApp*)ofGetAppPtr();
		name = "Plaids";
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
	int width = 100;
	int height = 100;
	int xcount = 1;
	int ycount = 1;
	ImVec4 c_base = ofColor(0, 255, 255, 155);
	ImVec4 c_base2 = ofColor(255, 0, 255, 155);
};
