#pragma once

#include "drawFilter.h"


class Df_noise : public DrawFilter {
public:
	Df_noise() {
		pixelplotter = (ofApp*)ofGetAppPtr();
		name = "Noise";
	};

	Df_noise(ofxXmlSettings settings) {
		Df_noise();
		loadSettings(settings);
	};

	virtual void draw(ofImage* input, float width = 0, float height = 0, float x = 0, float y = 0);
	virtual void renderImGuiSettings();
	virtual void loadSettings(ofxXmlSettings settings);
	virtual ofxXmlSettings getSettings();

	void drawPixel(int x, int y);

	ImVec4 cBg = ofColor(255, 255, 255, 255);

	class sColor {
	public:
		sColor(ofColor c, std::string n, int p = 0) {
			color = c;
			name = n;
			percent = p;
		}
		int percent = 0;
		std::string name;
		ImVec4 color;
	};

	std::vector<sColor*> palette;

	class subpixel {
	public:
		subpixel(int _x, int _y) {
			x = _x;
			y = _y;
		}
		int x;
		int y;
		ofColor c;
	};

	vector<subpixel> subpixels;
};
