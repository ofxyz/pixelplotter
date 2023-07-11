#include "drawPixel_SeparateRGB_Bars01.h"

ofxXmlSettings Dp_separateRGB_Bars01::getSettings() {
	ofxXmlSettings settings;
	settings.setValue("name", name);
	return settings;
}

void Dp_separateRGB_Bars01::loadSettings(ofxXmlSettings settings) {
	// name = settings.getValue("name", name);
	return;
}

void Dp_separateRGB_Bars01::renderImGuiSettings() {
	// Add draw order (shuffle ofVec4f)
	// Save settings
	renderImGuiColourSettings();
}

void Dp_separateRGB_Bars01::draw(ofColor c, float x, float y, float width, float height) {
	float whiteVal = min(min(c.r, c.g), c.b);
	float blackVal = 255 - max(max(c.r, c.g), c.b);

	ofVec4f rgbk = { (float)c.r - whiteVal, (float)c.g - whiteVal, (float)c.b - whiteVal, blackVal };

	float totalInk = rgbk[0] + rgbk[1] + rgbk[2] + whiteVal + blackVal;

	rgbk[0] = ofMap(rgbk[0], 0, totalInk, 0, 255);
	rgbk[1] = ofMap(rgbk[1], 0, totalInk, 0, 255);
	rgbk[2] = ofMap(rgbk[2], 0, totalInk, 0, 255);

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

	float start = x - (width * 0.5);
	float barWidth = width; // Space left

	// Do black
	if (blackVal > 0) { // Offset for white
		barWidth = (blackVal / 255) * width;
		start += (barWidth * 0.5);
		drawRectangle(start, y, barWidth, height, c_black);
		start += (barWidth * 0.5);
	}

	/* white at end ...
	if (whiteVal > 0) { // Offset for white
		barWidth = whiteVal * width;
		start += barWidth;
	}
	*/
	if (rgbk[0] > 0) {
		barWidth = (rgbk[0] / 255) * width;
		start += (barWidth * 0.5);
		drawRectangle(start, y, barWidth, height, c_magentaRed);
		start += (barWidth * 0.5);
	}
	if (rgbk[1] > 0) {
		barWidth = (rgbk[1] / 255) * width;
		start += (barWidth * 0.5);
		drawRectangle(start, y, barWidth, height, c_yellowGreen);
		start += (barWidth * 0.5);
	}
	if (rgbk[2] > 0) {
		barWidth = (rgbk[2] / 255) * width;
		start += (barWidth * 0.5);
		drawRectangle(start, y, barWidth, height, c_cyanBlue);
		start += (barWidth * 0.5);
	}
}
