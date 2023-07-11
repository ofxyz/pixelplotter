#include "drawPixel_SeparateCMYK_Bars01.h"

ofxXmlSettings Dp_separateCMYK_Bars01::getSettings() {
	ofxXmlSettings settings;
	settings.setValue("name", name);
	return settings;
}

void Dp_separateCMYK_Bars01::loadSettings(ofxXmlSettings settings) {
	// name = settings.getValue("name", name);
	return;
}

void Dp_separateCMYK_Bars01::renderImGuiSettings() {
	// Add draw order (shuffle ofVec4f)
	// Save settings
	renderImGuiColourSettings();
}

void Dp_separateCMYK_Bars01::draw(ofColor c, float x, float y, float width, float height) {
	ofVec4f cmyk = getCMYK(c);
	float whiteVal = 1 - max(max(cmyk[0], cmyk[1]), max(cmyk[2], cmyk[3]));
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
	if (barCount == 0) return;

	float start = x - (width * 0.5);
	float barWidth = width; // Space left

	/* white at end ...
	if (whiteVal > 0) { // Offset for white
		barWidth = whiteVal * width;
		start += barWidth;
	}
	*/
	if (cmyk[0] > 0) {
		barWidth = cmyk[0] * width;
		start += (barWidth * 0.5);
		drawRectangle(start, y, barWidth, height, c_cyanBlue);
		start += (barWidth * 0.5);
	}
	if (cmyk[1] > 0) {
		barWidth = cmyk[1] * width;
		start += (barWidth * 0.5);
		drawRectangle(start, y, barWidth, height, c_magentaRed);
		start += (barWidth * 0.5);
	}
	if (cmyk[2] > 0) {
		barWidth = cmyk[2] * width;
		start += (barWidth * 0.5);
		drawRectangle(start, y, barWidth, height, c_yellowGreen);
		start += (barWidth * 0.5);
	}
	if (cmyk[3] > 0) {
		barWidth = cmyk[3] * width;
		start += (barWidth * 0.5);
		drawRectangle(start, y, barWidth, height, c_black);
	}
}
