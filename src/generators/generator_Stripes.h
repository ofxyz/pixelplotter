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
	bool bHorizontal = false;
	int count = 1;
	float offset = 0;
	float offsetBounds = 100;
	ImVec4 c_x = ofColor(0, 0, 0, 255);
};
