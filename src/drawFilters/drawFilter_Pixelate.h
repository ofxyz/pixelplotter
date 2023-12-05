#pragma once

#include "drawFilter.h"

class Df_pixelate : public DrawFilter {
public:
	Df_pixelate();

	Df_pixelate(ofJson& settings);

	void InitDefaults();

	void draw(ofImage* input, float width = 0, float height = 0) override;
	void renderImGuiSettings() override;
	void loadSettings(ofJson& settings) override;
	ofJson getSettings() override;

private:
	void drawRectangle(float offsetX, float offsetY, float w, float h, ofColor c);
	void drawEllipse(float offsetX, float offsetY, float w, float h, ofColor c);
	void drawRoundedRect(float offsetX, float offsetY, float w, float h, ofColor c);
	void drawRgbSeperation_Fill(float offsetX, float offsetY, float w, float h, ofColor c);
	void drawRgbSeperation_Center(float offsetX, float offsetY, float w, float h, ofColor c);
	void drawRgbSeperation_Square(float offsetX, float offsetY, float w, float h, ofColor c);
	void drawCMYKSeperation_Fill(float offsetX, float offsetY, float w, float h, ofColor c);
	void drawCMYKSeperation_Square(float offsetX, float offsetY, float w, float h, ofColor c);
	void drawCMYKSeperation_Left(float offsetX, float offsetY, float w, float h, ofColor c);
	void drawCMYKSeperation_Hills(float offsetX, float offsetY, float w, float h, ofColor c);
	void drawCMYKSeperation_Bars(float offsetX, float offsetY, float w, float h, ofColor c);
	void drawColorAdjust(float offsetX, float offsetY, float w, float h, ofColor c);
	void drawUnusualOverprint(float offsetX, float offsetY, float w, float h, ofColor c);
	void drawStarHash(float offsetX, float offsetY, float w, float h, ofColor c);

	void quadraticBezierVertex(float cpx, float cpy, float x, float y, float prevX, float prevY);

	void drawPixel(float w, float h, ofColor c);
	float getRotation(ofColor c, float w, float h);
	float getWidth(ofColor c, float w, float h, float r);
	float getHeight(ofColor c, float w, float h, float r);

	std::vector<std::string> v_pixelType;
	std::vector<std::string> v_pixelDataMapOptions;
	std::vector<std::string> v_ignoreOptions;

	int ui_currentRotationMap = 0;
	int ui_currentWidthMap = 0;
	int ui_currentHeightMap = 0;
	int ui_currentPixelType = 0;
	int ui_currentIgnore = 0;
	bool normalize = false;
	bool polka = false;
	bool usePalette = true;
	int tilesX = 64;
	int tilesY = 64;
	float addonx = 0;
	float addony = 0;
	float addonx_rand = 0;
	float addony_rand = 0;
	float offsetx = 0;
	float offsety = 0;
	float offsetx_rand = 0;
	float offsety_rand = 0;
	int ignorePercent = 0;
	int ignoreModulo = 1;
	int ignorePlaid = 0;
	int ignoreScan = 0;
	int ignoreSeedAddon = 1;
	int ignoreSeed = 0;
	float cLerp = 0;
	float rounded = 0;

	glm::vec2 rotationMinMax;
	glm::vec2 widthMinMax;
	glm::vec2 heightMinMax;

	ImVec4 c_cyanBlue = ofColor(0, 174, 239, 255);
	ImVec4 c_magentaRed = ofColor(236, 0, 140, 255);
	ImVec4 c_yellowGreen = ofColor(255, 242, 0, 255);
	ImVec4 c_black = ofColor(0, 0, 0, 255);
	ImVec4 c_white = ofColor(255, 255, 255, 255);
	ImVec4 c_mask = ofColor(255, 255, 255, 255);

	void gui_setGrey();
	void gui_setRGB();
	void gui_setCMYK();
	void gui_setRYB();
	void renderImGuiColourSettings(bool colors, bool mask);
};
