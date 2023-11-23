#pragma once
#include "ofApp.h"
#include "ofxImGui.h"

/*
	A DrawPixel draws rectangular art based on a colour.
	It always draws from the center out.
 */

class DrawPixel {
public:
	std::string name; //PixelType
	
	/* PARAMS
		ofColor    c        : Colour of the pixel
		glm::vec2  dim      : (w, h): Dimension (Width, Height) of the pixel
		glm::vec2  pos      : (x, y): Position of the pixel (Used for %2 changes etc)
		glm::vec2  posNorm  : (x, y): Position Normal of the pixel. Between 0 and 1,
		                              Used to interpolate and extrapolate position of sub pixels
	*/
	virtual void draw(ofColor c, glm::vec2 dim, glm::vec2 pos = { 0,0 }, glm::vec2 posNorm = { 0,0 }) = 0;

	virtual void renderImGuiSettings() = 0;
	virtual void loadSettings(ofJson& settings) = 0;
	virtual ofJson getSettings() = 0;

	ImVec4 c_cyanBlue = ofColor(0, 174, 239, 255);
	ImVec4 c_magentaRed = ofColor(236, 0, 140, 255);
	ImVec4 c_yellowGreen = ofColor(255, 242, 0, 255);
	ImVec4 c_black = ofColor(0, 0, 0, 255);
	ImVec4 c_white = ofColor(255, 255, 255, 255);

	// TODO: Create colour palette manager... Swatches
	// We need to be able to re-order these and save it 
	// Need name, order, ImGui
	// std::vector<ImVec4*> c_colors;

	float rounded = 0;

	// Standard drawing methods
	void drawRectangle(float offsetX, float offsetY, float w, float h, ofColor c);

	void drawEllipse(float offsetX, float offsetY, float w, float h, ofColor c);

	void quadraticBezierVertex(float cpx, float cpy, float x, float y, float prevX, float prevY);

	void drawRoundedRect(float offsetX, float offsetY, float w, float h, ofColor c);

	void gui_setGrey();

	void gui_setRGB();

	void gui_setCMYK();

	void gui_setRYB();

	void renderImGuiColourSettings();

	bool isFresh();
	void setFresh(bool fresh);

private:
	bool bFresh = false;
};
