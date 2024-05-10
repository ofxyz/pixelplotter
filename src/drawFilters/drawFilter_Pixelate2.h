#pragma once

#include "drawFilter.h"
#include "vc_drawPixel.h"

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

private:
	bool pixelMirror;
	bool useCurve;
	int selectedPixelType;
	int tilesX;
	int tilesY;
	ImVec2 xCurvePoints[11];
	ImVec2 yCurvePoints[11];
	int selectionIdx = -1;
	int selectionIdy = -1;
};
