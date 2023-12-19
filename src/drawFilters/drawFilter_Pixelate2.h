#pragma once

#include "drawFilter.h"
#include "drawPixel_Controller.h"

class Df_pixelate2 : public DrawFilter {
public:
	Df_pixelate2();
	Df_pixelate2(ofJson& settings);

	void InitDefaults();

	void draw(ofImage* input, float width = 0, float height = 0) override;
	void renderImGuiSettings() override;
	void loadSettings(ofJson& settings) override;
	ofJson getSettings() override;

	DrawPixelController drawPixels;
	float xBezier[8] = { 0, 0, 0.390f, 0.575f, 0.565f, 1.000f, 1, 1 };
	float yBezier[8] = { 0, 0, 0.390f, 0.575f, 0.565f, 1.000f, 1, 1 };

private:
	bool pixelMirror;
	int selectedPixelType;
	int tilesX;
	int tilesY;
};
