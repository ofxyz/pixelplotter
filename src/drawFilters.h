#pragma once
#include "ofxImGui.h"
#include "ofxOpenCv.h"
#include "ofxXmlSettings.h"

class DrawFilter {
public:
	bool active = true;
	std::string name;

	//virtual void draw(ofFbo* input) = 0;
	virtual void draw(ofImage* input) = 0;
	virtual void renderImGuiSettings() = 0;

	int currentBlendModeIndex = 0;
	std::vector<std::string> v_BlendModes{ "OF_BLENDMODE_DISABLED", "OF_BLENDMODE_ALPHA", "OF_BLENDMODE_ADD", "OF_BLENDMODE_SUBTRACT", "OF_BLENDMODE_MULTIPLY", "OF_BLENDMODE_SCREEN" };
	void setBlendMode() {
		if (v_BlendModes[currentBlendModeIndex] == "OF_BLENDMODE_ALPHA") {
			ofEnableBlendMode(OF_BLENDMODE_DISABLED);
		}
		else if (v_BlendModes[currentBlendModeIndex] == "OF_BLENDMODE_ADD") {
			ofEnableBlendMode(OF_BLENDMODE_ADD);
		}
		else if (v_BlendModes[currentBlendModeIndex] == "OF_BLENDMODE_SUBTRACT") {
			ofEnableBlendMode(OF_BLENDMODE_SUBTRACT);
		}
		else if (v_BlendModes[currentBlendModeIndex] == "OF_BLENDMODE_MULTIPLY") {
			ofEnableBlendMode(OF_BLENDMODE_MULTIPLY);
		}
		else if (v_BlendModes[currentBlendModeIndex] == "OF_BLENDMODE_SCREEN") {
			ofEnableBlendMode(OF_BLENDMODE_SCREEN);
		}
		else {
			ofEnableBlendMode(OF_BLENDMODE_DISABLED);
		}
	}

	float drawScale = 4; // zoomMultiplier
};

class Df_pixelate : public DrawFilter {
public:
	std::string name = "Pixelate";
	void draw(ofImage* input);
	void renderImGuiSettings();

private:
	void drawPixel(float w, float h, ofColor c);
	float getRotation(ofColor c, float w, float h);
	float getWidth(ofColor c, float w, float h, float r);
	float getHeight(ofColor c, float w, float h, float r);

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

	// ADD PIXEL OFFSET so next filter can be misaligned ...
	// ADD Transparency Slider
	int ui_currentRotationMap = 0;
	int ui_currentWidthMap = 0;
	int ui_currentHeightMap = 0;
	bool normalize = false;
	bool polka = false;
	bool roundPixels = false;
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

	ofVec2f rotationMinMax = ofVec2f(0, 0);
	ofVec2f widthMinMax = ofVec2f(5, 50);
	ofVec2f heightMinMax = ofVec2f(5, 50);
};

class Df_rings : public DrawFilter {
public:
	std::string name = "Rings";
	void draw(ofImage* input);
	void renderImGuiSettings();

private:
	int cvThresh = 128;
	int cvBlur = 150;
	int cvSteps = 10;

	ofxCvContourFinder contourFinder;

	ofxCvColorImage colorCvImage;
	ofxCvGrayscaleImage greyCvImage;
	ofxCvGrayscaleImage greyCvBlur;
	ofxCvGrayscaleImage greyCvThresh;
};
