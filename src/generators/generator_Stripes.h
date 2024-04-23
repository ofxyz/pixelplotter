#pragma once

#include "generator.h"

class G_stripes : public Generator {
public:
	G_stripes() {
		pixelplotter = (ofApp*)ofGetAppPtr();
		name = "Stripes";
		setup(100, 100);
	};

	G_stripes(ofJson& settings) {
		pixelplotter = (ofApp*)ofGetAppPtr();
		name = "Stripes";
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
	int xcount = 1;
	int ycount = 1;
	ImVec4 c_base = ofColor(0, 255, 255, 155);
	ImVec4 c_base2 = ofColor(255, 0, 255, 155);
};
