#pragma once

#include "generator.h"

class G_plaids : public Generator {
public:
	// TODO: Finish
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
	int width, height;
	int xcount = 1;
	int ycount = 1;
	ImVec4 c_base = ofColor(0, 255, 255, 255);
};
