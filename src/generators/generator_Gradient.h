#pragma once

#include "generator.h"
#include "ImGradientHDR.h"

class G_gradient : public Generator {
public:
	G_gradient() {
		pixelplotter = (ofApp*)ofGetAppPtr();
		name = "Gradient";
	};

	G_gradient(ofJson& settings) {
		pixelplotter = (ofApp*)ofGetAppPtr();
		name = "Gradient";
		loadSettings(settings);
	};

	void setup(int width = 255, int height = 255) override;
	void update() override;
	void draw() override;

	void renderImGuiSettings() override;
	void loadSettings(ofJson& settings) override;
	ofJson getSettings() override;

private:
	void drawPattern();
	int numSteps = 25;
	bool bDirFlip = false;
	bool bDrawVertical = false;

	int32_t stateID = 1;
	ImGradientHDRState state;
	ImGradientHDRTemporaryState tempState;

};
