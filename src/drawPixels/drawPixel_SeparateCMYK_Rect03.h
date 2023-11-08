#pragma once

#include "drawPixel.h"

class Dp_separateCMYK_Rect03 : public DrawPixel {
public:
	Dp_separateCMYK_Rect03() {
		name = "Separate CMYK Rect 03";
	}

	Dp_separateCMYK_Rect03(ofJson settings) {
		name = "Separate CMYK Rect 03";
		loadSettings(settings);
	}

	void draw(ofColor c, glm::vec2 dim, glm::vec2 pos = { 0,0 }, glm::vec2 posNorm = { 0,0 }) override;
	void renderImGuiSettings() override;
	void loadSettings(ofJson settings) override;
	ofJson getSettings() override;

};
