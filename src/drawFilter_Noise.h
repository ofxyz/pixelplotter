#pragma once

#include "drawFilter.h"


class Df_noise : public DrawFilter {
public:
	ofApp* pixelplotter;
	Df_noise(ofApp* app) {
		pixelplotter = app;
		name = "Noise";
		//palette.push_back(new sColor(ofColor(255,0,0,255), "Red"));
	};

	void draw(ofImage* input, float width = 0, float height = 0, float x = 0, float y = 0);
	
	void drawPixel(int x, int y);

	void renderImGuiSettings();
	virtual void loadSettings(ofxXmlSettings settings);
	ofxXmlSettings getSettings();
	ImVec4 cBg = ofColor(255,255,255,255);

	class sColor {
	public:
		sColor(ofColor c, std::string n, int p = 0){
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
