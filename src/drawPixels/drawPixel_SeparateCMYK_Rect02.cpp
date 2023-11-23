#include "drawPixel_SeparateCMYK_Rect02.h"

ofJson Dp_separateCMYK_Rect02::getSettings() {
	ofJson settings;
	settings["name"] = name;
	return settings;
}

void Dp_separateCMYK_Rect02::loadSettings(ofJson& settings) {
	// name = settings.value("name", name);
	return;
}

void Dp_separateCMYK_Rect02::renderImGuiSettings() {
	// Add draw order (shuffle ofVec4f)
	// Save settings
	renderImGuiColourSettings();
}

void Dp_separateCMYK_Rect02::draw(ofColor c, glm::vec2 dim, glm::vec2 pos /*= { 0,0 }*/, glm::vec2 posNorm /*= { 0,0 }*/)
{
	glm::vec4 cmyk = ofx2d::getCMYK(c);
	
	float whiteVal = min(min(c.r, c.g), c.b);
	if (whiteVal > 0) {
		whiteVal /= 255;
	}

	float totalInk = cmyk[0] + cmyk[1] + cmyk[2] + cmyk[3] + whiteVal;

	cmyk[0] = ofMap(cmyk[0], 0, totalInk, 0, 1);
	cmyk[1] = ofMap(cmyk[1], 0, totalInk, 0, 1);
	cmyk[2] = ofMap(cmyk[2], 0, totalInk, 0, 1);
	cmyk[3] = ofMap(cmyk[3], 0, totalInk, 0, 1);

	glm::vec2 startPos = { 0, 0 };

	float barCount = 4;
	if (cmyk[0] == 0) {
		barCount--;
	}
	if (cmyk[1] == 0) {
		barCount--;
	}
	if (cmyk[2] == 0) {
		barCount--;
	}
	if (cmyk[3] == 0) {
		barCount--;
	}
	if (barCount == 0) {
		drawRectangle(startPos.x, startPos.y, dim.x, dim.y, c_white);
	}

	float barWidth, barHeight;
	
	if (cmyk[0] > 0) {
		barWidth  = (cmyk[0] + cmyk[1] + cmyk[2] + cmyk[3] + whiteVal) * dim.x;
		barHeight = (cmyk[0] + cmyk[1] + cmyk[2] + cmyk[3] + whiteVal) * dim.y;
		drawRectangle(startPos.x, startPos.y, barWidth, barHeight, c_cyanBlue);
	}
	if (cmyk[1] > 0) {
		barWidth  = (cmyk[1] + cmyk[2] + cmyk[3] + whiteVal) * dim.x;
		barHeight = (cmyk[1] + cmyk[2] + cmyk[3] + whiteVal) * dim.y;
		drawRectangle(startPos.x, startPos.y, barWidth, barHeight, c_magentaRed);
	}
	if (cmyk[2] > 0) {
		barWidth  = (cmyk[2] + cmyk[3] + whiteVal) * dim.x;
		barHeight = (cmyk[2] + cmyk[3] + whiteVal) * dim.y;
		drawRectangle(startPos.x, startPos.y, barWidth, barHeight, c_yellowGreen);
	}
	if (cmyk[3] > 0) {
		barWidth  = (cmyk[3] + whiteVal) * dim.x;
		barHeight = (cmyk[3] + whiteVal) * dim.y;
		drawRectangle(startPos.x, startPos.y, barWidth, barHeight, c_black);
	}
	if (whiteVal > 0) {
		barWidth = whiteVal * dim.x;
		barHeight = whiteVal * dim.y;
		drawRectangle(startPos.x, startPos.y, barWidth, barHeight, c_white);
	}
}
