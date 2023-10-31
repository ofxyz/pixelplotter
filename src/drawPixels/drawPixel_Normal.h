#pragma once

#include "drawPixel.h"

class Dp_normal : public DrawPixel {
public:
	Dp_normal() {
		name = "Normal";
	}

	Dp_normal(ofJson settings) {
		name = "Normal";
		loadSettings(settings);
	}

	void draw(ofColor c, glm::vec2 dim, glm::vec2 pos = { 0,0 }, glm::vec2 posNorm = { 0,0 }) override;
	void renderImGuiSettings() override;
	void loadSettings(ofJson settings) override;
	ofJson getSettings() override;

};
