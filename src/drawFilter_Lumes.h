#pragma once

#include "drawFilter.h"
#include "ofxSvg.h"

class Df_lumes : public DrawFilter {
public:
	Df_lumes() {
		pixelplotter = (ofApp*)ofGetAppPtr();
		name = "Lumes";
	};

	Df_lumes(ofxXmlSettings settings) {
		Df_lumes();
		loadSettings(settings);
	};

	virtual void draw(ofImage* input, float width = 0, float height = 0, float x = 0, float y = 0);
	void drawKite(ofColor c, float offsetX, float offsetY, float width, float height, float centerPercent, glm::ivec2 pos);
	virtual void renderImGuiSettings();
	virtual void loadSettings(ofxXmlSettings settings);
	ofxXmlSettings getSettings();

private:
	ofxSVG svgKite;
	int tilesX = 64;
	int tilesY = 64;
	float centerPercent = 0.75;
	float rotDeg = 0;
};
