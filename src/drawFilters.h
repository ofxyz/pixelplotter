#pragma once
#include "ofxImGui.h"
#include "ofxOpenCv.h"
#include "ofxXmlSettings.h"

class DrawFilter {
public:
	//virtual void draw(ofFbo* input) = 0;
	virtual void draw(ofImage* input) = 0;
	virtual void renderImGuiSettings() = 0;
	virtual std::string getFilterName() = 0;
	bool active = true;
private:
	std::string name;
	ofxXmlSettings settings;
};

class Df_pixelate : public DrawFilter {
public:
	void draw(ofImage* input);
	void renderImGuiSettings();
	std::string getFilterName() {
		return name;
	}
private:
	void drawPixel(float w, float h, ofColor c);
	float getRotation(ofColor c, float w, float h);
	std::string name = "Pixelate";

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


	int ui_currentRotationMap = 0;
	bool normalize = false;
	bool polka = false;
	bool roundPixels = false;
	int tilesX = 64;
	int tilesY = 64;
	float addonx = 0;
	float addony = 0;
	float randomOffset = 0;
	int rotationMapTo = 0;
	ofVec2f rotationMinMax = ofVec2f(0, 0);
	float drawScale = 4; // zoomMultiplier
};

class Df_rings : public DrawFilter {
public:
	void update(ofImage* input);
	void draw(ofImage* input);
	void renderImGuiSettings();
	std::string getFilterName() {
		return name;
	}
private:
	std::string name = "Rings";
	float drawScale = 4; // zoomMultiplier

	int cvThresh = 128;
	int cvBlur = 150;
	int cvSteps = 10;

	ofxCvContourFinder contourFinder;

	ofxCvColorImage colorCvImage;
	ofxCvGrayscaleImage greyCvImage;
	ofxCvGrayscaleImage greyCvBlur;
	ofxCvGrayscaleImage greyCvThresh;
};
