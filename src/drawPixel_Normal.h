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

	virtual void draw(ofColor c, float offsetX, float offsetY, float width, float height);
	virtual void renderImGuiSettings();
	virtual void loadSettings(ofJson settings);
	virtual ofJson getSettings();

};
