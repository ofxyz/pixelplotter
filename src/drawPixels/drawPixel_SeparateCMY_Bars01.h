#pragma once

#include "drawPixel.h"

class Dp_separateCMY_Bars01 : public DrawPixel {
public:
	Dp_separateCMY_Bars01() {
		name = "Separate CMY Bars 01";
	}

	Dp_separateCMY_Bars01(ofJson& settings) {
		name = "Separate CMY Bars 01";
		loadSettings(settings);
	}

	void draw(ofColor c, glm::vec2 dim, glm::vec2 pos = { 0,0 }, glm::vec2 posNorm = { 0,0 }) override;
	void renderImGuiSettings() override;
	void loadSettings(ofJson& settings) override;
	ofJson getSettings() override;

};
