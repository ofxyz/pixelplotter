#include "drawPixel_SeparateRGB_Bars01.h"

ofJson Dp_separateRGB_Bars01::getSettings() {
	ofJson settings;
	settings["name"] = name;
	return settings;
}

void Dp_separateRGB_Bars01::loadSettings(ofJson& settings) {
	// name = settings.value("name", name);
	return;
}

void Dp_separateRGB_Bars01::renderImGuiSettings() {
	// Add draw order (shuffle ofVec4f)
	// Save settings
	renderImGuiColourSettings();
}

void Dp_separateRGB_Bars01::draw(ofColor c, glm::vec2 dim, glm::vec2 pos /*= { 0,0 }*/, glm::vec2 posNorm /*= { 0,0 }*/)
{
	float whiteVal = std::min(std::min(c.r, c.g), c.b);
	float blackVal = 255 - std::max(std::max(c.r, c.g), c.b);

	glm::vec4 rgbk = { (float)c.r - whiteVal, (float)c.g - whiteVal, (float)c.b - whiteVal, blackVal };

	float totalInk = rgbk[0] + rgbk[1] + rgbk[2] + whiteVal + blackVal;

	rgbk[0] = ofMap(rgbk[0], 0, totalInk, 0, 255);
	rgbk[1] = ofMap(rgbk[1], 0, totalInk, 0, 255);
	rgbk[2] = ofMap(rgbk[2], 0, totalInk, 0, 255);
	rgbk[3] = ofMap(rgbk[3], 0, totalInk, 0, 255);

	float barCount = 3;
	if (rgbk[0] == 0) {
		barCount--;
	}
	if (rgbk[1] == 0) {
		barCount--;
	}
	if (rgbk[2] == 0) {
		barCount--;
	}

	glm::vec2 startPos = { -(dim.x * 0.5), 0 };
	glm::vec2 drawPos = startPos;
	float barWidth = dim.x; // Space left

	// Do black
	if (blackVal > 0) { // Offset for white
		barWidth = (blackVal / 255) * dim.x;
		drawPos.x += (barWidth * 0.5);
		drawRectangle(drawPos.x, drawPos.y, barWidth, dim.y, c_black);
		drawPos.x += (barWidth * 0.5);
	}

	/* white at end ...
	if (whiteVal > 0) { // Offset for white
		barWidth = whiteVal * width;
		start += barWidth;
	}
	*/
	if (rgbk[0] > 0) {
		barWidth = (rgbk[0] / 255) * dim.x;
		drawPos.x += (barWidth * 0.5);
		drawRectangle(drawPos.x, drawPos.y, barWidth, dim.y, c_magentaRed);
		drawPos.x += (barWidth * 0.5);
	}
	if (rgbk[1] > 0) {
		barWidth = (rgbk[1] / 255) * dim.x;
		drawPos.x += (barWidth * 0.5);
		drawRectangle(drawPos.x, drawPos.y, barWidth, dim.y, c_yellowGreen);
		drawPos.x += (barWidth * 0.5);
	}
	if (rgbk[2] > 0) {
		barWidth = (rgbk[2] / 255) * dim.x;
		drawPos.x += (barWidth * 0.5);
		drawRectangle(drawPos.x, drawPos.y, barWidth, dim.y, c_cyanBlue);
		drawPos.x += (barWidth * 0.5);
	}
}
