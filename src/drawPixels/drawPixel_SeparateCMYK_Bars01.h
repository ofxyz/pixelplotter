#pragma once

#include "drawPixel.h"

class Dp_separateCMYK_Bars01 : public DrawPixel {
public:
	Dp_separateCMYK_Bars01() {
		name = "Separate CMYK Bars 01";
	}

	Dp_separateCMYK_Bars01(ofJson settings) {
		name = "Separate CMYK Bars 01";
		loadSettings(settings);
	}

	void draw(ofColor c, float offsetX, float offsetY, float width, float height) override;
	void renderImGuiSettings() override;
	void loadSettings(ofJson settings) override;
	ofJson getSettings() override;

};
