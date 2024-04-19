#include "drawPixel_SeparateCMY_Bars01.h"

ofJson Dp_separateCMY_Bars01::getSettings() {
	ofJson settings;
	settings["name"] = name;
	return settings;
}

void Dp_separateCMY_Bars01::loadSettings(ofJson& settings) {
	// name = settings.value("name", name);
	return;
}

void Dp_separateCMY_Bars01::renderImGuiSettings() {
	// Add draw order (shuffle ofVec4f)
	// Save settings
	renderImGuiColourSettings();
}

void Dp_separateCMY_Bars01::draw(ofColor c, glm::vec2 dim, glm::vec2 pos /*= { 0,0 }*/, glm::vec2 posNorm /*= { 0,0 }*/)
{
	glm::vec3 cmy = ofx2d::getCMY(c);
	
	float whiteVal = std::min(std::min(c.r, c.g), c.b);
	if (whiteVal > 0) {
		whiteVal /= 255;
	}

	float totalInk = cmy[0] + cmy[1] + cmy[2] + whiteVal;

	cmy[0] = ofMap(cmy[0], 0, totalInk, 0, 1);
	cmy[1] = ofMap(cmy[1], 0, totalInk, 0, 1);
	cmy[2] = ofMap(cmy[2], 0, totalInk, 0, 1);
	
	float barCount = 3;
	if (cmy[0] == 0) {
		barCount--;
	}
	if (cmy[1] == 0) {
		barCount--;
	}
	if (cmy[2] == 0) {
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

	if (cmy[0] > 0) {
		barWidth = cmy[0] * dim.x;
		start.x += (barWidth * 0.5);
		drawRectangle(start.x, start.y, barWidth, dim.y, c_cyanBlue);
		start.x += (barWidth * 0.5);
	}
	if (cmy[1] > 0) {
		barWidth = cmy[1] * dim.x;
		start.x += (barWidth * 0.5);
		drawRectangle(start.x, start.y, barWidth, dim.y, c_magentaRed);
		start.x += (barWidth * 0.5);
	}
	if (cmy[2] > 0) {
		barWidth = cmy[2] * dim.x;
		start.x += (barWidth * 0.5);
		drawRectangle(start.x, start.y, barWidth, dim.y, c_yellowGreen);
		start.x += (barWidth * 0.5);
	}

}
