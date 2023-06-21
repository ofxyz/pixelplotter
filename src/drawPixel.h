#pragma once
#include "ofxImGui.h"
#include "ofxXmlSettings.h"

/*
	A drawpixel draws rectangular art based on a colour.
 */

class DrawPixel {
public:
	std::string name; //PixelType

	// Pixels are draw from the centre (0,0) ... See drawRectangle()
	virtual void draw(ofColor c, float offsetX, float offsetY, float width, float height) = 0;
	virtual void renderImGuiSettings() = 0;
	virtual void loadSettings(ofxXmlSettings settings) = 0;
	virtual ofxXmlSettings getSettings() = 0;
	
	float rounded = 0;

	// Standard drawing methods
	void drawRectangle(float offsetX, float offsetY, float w, float h, ofColor c) {
		ofPushStyle();
		ofFill();
		ofSetColor(c);
		ofDrawRectangle(-(w * 0.5) + offsetX, -(h * 0.5) + offsetY, w, h);
		ofPopStyle();
	};

	void drawEllipse(float offsetX, float offsetY, float w, float h, ofColor c) {
		ofPushStyle();
		ofFill();
		ofSetColor(c);
		//ofSetCircleResolution(4); Does not make sense for ciaro renderer.
		ofDrawEllipse(offsetX, offsetY, w, h);
		ofPopStyle();
	};

	void quadraticBezierVertex(float cpx, float cpy, float x, float y, float prevX, float prevY) {
		float cp1x = prevX + 2.0 / 3.0 * (cpx - prevX);
		float cp1y = prevY + 2.0 / 3.0 * (cpy - prevY);
		float cp2x = cp1x + (x - prevX) / 3.0;
		float cp2y = cp1y + (y - prevY) / 3.0;

		// finally call cubic Bezier curve function  
		ofBezierVertex(cp1x, cp1y, cp2x, cp2y, x, y);
	};

	void drawRoundedRect(float offsetX, float offsetY, float w, float h, ofColor c) {
		// Use ofDrawRectRounded(p.x, p.y, 0.0, w, h, r,r,r,r) ?
		ofPushStyle();
		ofFill();
		ofSetColor(c);

		float x = -(w * 0.5) + offsetX;
		float y = -(h * 0.5) + offsetY;

		ofBeginShape();
		ofVertex(x + rounded, y);
		ofVertex(x + w - rounded, y);
		quadraticBezierVertex(x + w, y, x + w, y + rounded, x + w - rounded, y);
		ofVertex(x + w, y + h - rounded);
		quadraticBezierVertex(x + w, y + h, x + w - rounded, y + h, x + w, y + h - rounded);
		ofVertex(x + rounded, y + h);
		quadraticBezierVertex(x, y + h, x, y + h - rounded, x + rounded, y + h);
		ofVertex(x, y + rounded);
		quadraticBezierVertex(x, y, x + rounded, y, x, y + rounded);
		ofEndShape();

		ofPopStyle();
	};
};
