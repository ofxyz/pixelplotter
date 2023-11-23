#include "drawPixel_SeparateCMYK_Bars01.h"

ofJson Dp_separateCMYK_Bars01::getSettings() {
	ofJson settings;
	settings["name"] = name;
	return settings;
}

void Dp_separateCMYK_Bars01::loadSettings(ofJson& settings) {
	// name = settings.value("name", name);
	return;
}

void Dp_separateCMYK_Bars01::renderImGuiSettings() {
	// Add draw order (shuffle ofVec4f)
	// Save settings
	renderImGuiColourSettings();
}

void Dp_separateCMYK_Bars01::draw(ofColor c, glm::vec2 dim, glm::vec2 pos /*= { 0,0 }*/, glm::vec2 posNorm /*= { 0,0 }*/)
{
	glm::vec4 cmyk = ofx2d::getCMYK(c);
	
	float whiteVal = min(min(c.r, c.g), c.b);
	if (whiteVal > 0) {
		whiteVal /= 255;
	}

	float totalInk = cmyk[0] + cmyk[1] + cmyk[2] + cmyk[3] + whiteVal;

	// TODO: Make a rich black conversion
	// Or maybe add adjustment curves?
	/*
	if (cmyk[3] > 0.1) {
		cmyk[3] *= 0.5;
		double z = cmyk[3] * 0.5;
		// Don't add new colors just rich existing
		if (cmyk[0] > 0) cmyk[0] += z;
		if (cmyk[1] > 0) cmyk[1] += z;
		if (cmyk[2] > 0) cmyk[2] += z;
	}
	*/

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
	if (barCount == 0) return;

	glm::vec2 start = { -(dim.x * 0.5), 0 };
	float barWidth = dim.x; // Space left

	/* white at end ...
	if (whiteVal > 0) { // Offset for white
		barWidth = whiteVal * width;
		start += barWidth;
	}
	*/
	if (cmyk[0] > 0) {
		barWidth = cmyk[0] * dim.x;
		start.x += (barWidth * 0.5);
		drawRectangle(start.x, start.y, barWidth, dim.y, c_cyanBlue);
		start.x += (barWidth * 0.5);
	}
	if (cmyk[1] > 0) {
		barWidth = cmyk[1] * dim.x;
		start.x += (barWidth * 0.5);
		drawRectangle(start.x, start.y, barWidth, dim.y, c_magentaRed);
		start.x += (barWidth * 0.5);
	}
	if (cmyk[2] > 0) {
		barWidth = cmyk[2] * dim.x;
		start.x += (barWidth * 0.5);
		drawRectangle(start.x, start.y, barWidth, dim.y, c_yellowGreen);
		start.x += (barWidth * 0.5);
	}
	if (cmyk[3] > 0) {
		barWidth = cmyk[3] * dim.x;
		start.x += (barWidth * 0.5);
		drawRectangle(start.x, start.y, barWidth, dim.y, c_black);
	}
}
