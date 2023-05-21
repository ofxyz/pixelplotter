#pragma once
#include "ofxImGui.h"
#include "ofxXmlSettings.h"

/*
	The draw pixel class draws rectangular art based on a colour.
	We should use Blend2d for this, step 1 create Blend2D canvas ...

	Add DrawPixel Controller:
		- Set Pixel Type
		- DrawFilters
 */

class DrawPixel {
public:
	std::string name; //PixelType

	virtual void draw(ofColor c, float width = 0, float height = 0, float x = 0, float y = 0) = 0;
	virtual void renderImGuiSettings() = 0;
	virtual void loadSettings(ofxXmlSettings settings) = 0;
	virtual ofxXmlSettings getSettings() = 0;

	ofVec4f getCMYK(ofColor rgb) {
		double dr = 0, dg = 0, db = 0;
		if (rgb.r > 0) {
			dr = (double)rgb.r / 255;
		}
		if (rgb.g > 0) {
			dg = (double)rgb.g / 255;
		}
		if (rgb.b > 0) {
			db = (double)rgb.b / 255;
		}

		double k = 1 - max(max(dr, dg), db);

		double kop = 1 - k;
		
		double c = 1 - dr - k;
		double m = 1 - dg - k;
		double y = 1 - db - k;

		if (kop > 0) {
			if (c > 0) {
				c /= kop;
			}
			if (m > 0) {
				m /= kop;
			}
			if (y > 0) {
				y /= kop;
			}
		}

		return ofVec4f(c, m, y, k);
	}
};
