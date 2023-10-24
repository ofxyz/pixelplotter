#pragma once
#include "ofApp.h"
#include "ofxImGui.h"
/*
	A drawpixel draws rectangular art based on a colour.
 */

class DrawPixel {
public:
	std::string name; //PixelType

	// Pixels are draw from the centre (0,0) ... See drawRectangle()
	virtual void draw(ofColor c, float offsetX, float offsetY, float width, float height) = 0;
	virtual void renderImGuiSettings() = 0;
	virtual void loadSettings(ofJson settings) = 0;
	virtual ofJson getSettings() = 0;

	ImVec4 c_cyanBlue = ofColor(0, 174, 239, 255);
	ImVec4 c_magentaRed = ofColor(236, 0, 140, 255);
	ImVec4 c_yellowGreen = ofColor(255, 242, 0, 255);
	ImVec4 c_black = ofColor(0, 0, 0, 255);

	float rounded = 0;

	// Standard drawing methods
	void drawRectangle(float offsetX, float offsetY, float w, float h, ofColor c);;

	void drawEllipse(float offsetX, float offsetY, float w, float h, ofColor c);;

	void quadraticBezierVertex(float cpx, float cpy, float x, float y, float prevX, float prevY);;

	void drawRoundedRect(float offsetX, float offsetY, float w, float h, ofColor c);;

	// Standard color methods
	glm::vec4 getCMYK(ofColor rgb);;

	void gui_setGrey();

	void gui_setRGB();

	void gui_setCMYK();

	void gui_setRYB();

	void renderImGuiColourSettings();;

	bool isFresh();
	void setFresh(bool fresh);

private:
	bool bFresh = false;
};
