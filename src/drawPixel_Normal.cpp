#include "drawPixel_Normal.h"

ofxXmlSettings Dp_normal::getSettings() {
	ofxXmlSettings settings;
	settings.setValue("name", name);
	return settings;
}

void Dp_normal::loadSettings(ofxXmlSettings settings) {
	// name = settings.getValue("name", name);
	setFresh(true);
	return;
}

void Dp_normal::renderImGuiSettings() {
	// Nothing to see here ...
}

void Dp_normal::draw(ofColor c, float x, float y, float width, float height) {
	drawRectangle(x, y, width, height, c);
}
