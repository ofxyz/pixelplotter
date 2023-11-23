#include "drawPixel_SeparateCMYK_Rect03.h"

ofJson Dp_separateCMYK_Rect03::getSettings() {
	ofJson settings;
	settings["name"] = name;
	return settings;
}

void Dp_separateCMYK_Rect03::loadSettings(ofJson& settings) {
	// name = settings.value("name", name);
	return;
}

void Dp_separateCMYK_Rect03::renderImGuiSettings() {
	// Add draw order (shuffle ofVec4f)
	// Save settings
	renderImGuiColourSettings();
}

void Dp_separateCMYK_Rect03::draw(ofColor c, glm::vec2 dim, glm::vec2 pos /*= { 0,0 }*/, glm::vec2 posNorm /*= { 0,0 }*/)
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
		drawRectangle(0, 0, dim.x, dim.y, c_white);
	}

	float barWidth, barHeight;
	glm::vec2 startPos = { -(dim.x * 0.5), -(dim.y * 0.5) };
	glm::vec2 drawPos = startPos;

	if (cmyk[0] > 0) {
		barWidth  = (cmyk[0] + cmyk[1] + cmyk[2] + cmyk[3] + whiteVal) * dim.x;
		barHeight = (cmyk[0] + cmyk[1] + cmyk[2] + cmyk[3] + whiteVal) * dim.y;
		drawPos += glm::vec2((barWidth * 0.5), (barHeight * 0.5));
		drawRectangle(drawPos.x, drawPos.y, barWidth, barHeight, c_cyanBlue);
		drawPos = startPos;
	}
	if (cmyk[1] > 0) {
		barWidth  = (cmyk[1] + cmyk[2] + cmyk[3] + whiteVal) * dim.x;
		barHeight = (cmyk[1] + cmyk[2] + cmyk[3] + whiteVal) * dim.y;
		drawPos += glm::vec2((barWidth * 0.5), (barHeight * 0.5));
		drawRectangle(drawPos.x, drawPos.y, barWidth, barHeight, c_magentaRed);
		drawPos = startPos;
	}
	if (cmyk[2] > 0) {
		barWidth  = (cmyk[2] + cmyk[3] + whiteVal) * dim.x;
		barHeight = (cmyk[2] + cmyk[3] + whiteVal) * dim.y;
		drawPos += glm::vec2((barWidth * 0.5), (barHeight * 0.5));
		drawRectangle(drawPos.x, drawPos.y, barWidth, barHeight, c_yellowGreen);
		drawPos = startPos;
	}
	if (cmyk[3] > 0) {
		barWidth  = (cmyk[3] + whiteVal) * dim.x;
		barHeight = (cmyk[3] + whiteVal) * dim.y;
		drawPos += glm::vec2((barWidth * 0.5), (barHeight * 0.5));
		drawRectangle(drawPos.x, drawPos.y, barWidth, barHeight, c_black);
		drawPos = startPos;
	}
	if (whiteVal > 0) {
		barWidth = whiteVal * dim.x;
		barHeight = whiteVal * dim.y;
		drawPos += glm::vec2((barWidth * 0.5), (barHeight * 0.5));
		drawRectangle(drawPos.x, drawPos.y, barWidth, barHeight, c_white);
		drawPos = startPos;
	}
}
