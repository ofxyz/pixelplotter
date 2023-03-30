#pragma once

#include "drawFilter.h"

class Df_pixelate : public DrawFilter {
public:
	Df_pixelate() {
		name = "Pixelate";
	};
	void draw(ofImage* input);
	void renderImGuiSettings();
	void loadSettings(ofxXmlSettings settings);
	ofxXmlSettings getSettings();

private:
	void drawRectangle(float offsetX, float offsetY, float w, float h, ofColor c);
	void drawEllipse(float offsetX, float offsetY, float w, float h, ofColor c);
	void drawRgbSeperation_Fill(float offsetX, float offsetY, float w, float h, ofColor c);
	void drawRgbSeperation_Center(float offsetX, float offsetY, float w, float h, ofColor c);
	void drawRgbSeperation_Square(float offsetX, float offsetY, float w, float h, ofColor c);
	void drawCMYKSeperation_Square(float offsetX, float offsetY, float w, float h, ofColor c);
	void drawCMYKSeperation_Left(float offsetX, float offsetY, float w, float h, ofColor c);
	void drawCMYKSeperation_Hills(float offsetX, float offsetY, float w, float h, ofColor c);
	void drawCMYKSeperation_Bars(float offsetX, float offsetY, float w, float h, ofColor c);
	void drawColorAdjust(float offsetX, float offsetY, float w, float h, ofColor c);
	void drawUnusualOverprint(float offsetX, float offsetY, float w, float h, ofColor c);

	void drawPixel(float w, float h, ofColor c);
	float getRotation(ofColor c, float w, float h);
	float getWidth(ofColor c, float w, float h, float r);
	float getHeight(ofColor c, float w, float h, float r);

	std::vector<std::string> v_pixelType{
			"Rectangle",
			"Ellipse",
			"RGB Seperation Fill",
			"RGB Seperation Center",
			"RGB Seperation Square",
			"CMYK Seperation Square",
			"CMYK Seperation Left",
			"CMYK Seperation Hills",
			"CMYK Seperation Bars",
			"Color Adjust",
			"Unusual Overprint"
	};

	std::vector<std::string> v_pixelDataMapOptions{
		"None",
		"Between",
		"Color Lightness"/*,
		"Color Red",
		"Color Green",
		"Color Blue",
		"Location X",
		"Location Y" */
	};

	std::vector<std::string> v_ignoreOptions{
		"None",
		"Random",
		"Modulo",
		"Plaid",
		"Scan"
	};

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

	ofVec2f rotationMinMax = ofVec2f(0, 0);
	ofVec2f widthMinMax = ofVec2f(5, 50);
	ofVec2f heightMinMax = ofVec2f(5, 50);
};