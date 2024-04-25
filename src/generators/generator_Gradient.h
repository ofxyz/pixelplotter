#pragma once

#include "generator.h"
#include "ofxColorGradient.h"
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

	void setup(int width = 100, int height = 100) override;
	void update() override;
	void draw() override;

	void renderImGuiSettings() override;
	void loadSettings(ofJson& settings) override;
	ofJson getSettings() override;

	ofxColorGradient<ofColor> gradient;

private:
	void drawPattern();
	int numSteps = 25;
	bool bDirFlip = false;
	bool bDrawVertical = false;
	bool bHardMode = false;

	ImVec4 c_base = ofColor(0, 255, 255, 155);
	ImVec4 c_base2 = ofColor(255, 0, 255, 155);

	int32_t stateID = 10;

	ImGradientHDRState state;
	ImGradientHDRTemporaryState tempState;

};
