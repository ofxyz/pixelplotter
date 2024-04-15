#pragma once

#include "drawPixel.h"

class Dp_separateCMYK_Para01 : public DrawPixel {
public:
	Dp_separateCMYK_Para01() {
		name = "Separate CMYK Para 01";
	}

	Dp_separateCMYK_Para01(ofJson& settings) {
		name = "Separate CMYK Para 01";
		loadSettings(settings);
	}

	void draw(ofColor c, glm::vec2 dim, glm::vec2 pos = { 0,0 }, glm::vec2 posNorm = { 0,0 }) override;
	void renderImGuiSettings() override;
	void loadSettings(ofJson& settings) override;
	ofJson getSettings() override;

	bool sortDescending(float i, float j)
	{
		return (j < i);
	}
};
