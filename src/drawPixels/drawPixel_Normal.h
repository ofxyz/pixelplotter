#pragma once

#include "drawPixel.h"

class Dp_normal : public DrawPixel {
public:
	Dp_normal() {
		name = "Normal";
	}

	Dp_normal(ofJson settings) {
		Dp_normal();
		loadSettings(settings);
	}

	void draw(ofColor c, float offsetX, float offsetY, float width, float height) override;
	void renderImGuiSettings() override;
	void loadSettings(ofJson settings) override;
	ofJson getSettings() override;

};
