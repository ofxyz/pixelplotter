#include "drawPixel_Normal.h"

ofJson Dp_normal::getSettings() {
	ofJson settings;
	settings["name"] = name;
	return settings;
}

void Dp_normal::loadSettings(ofJson settings) {
	//name = settings.value("name", name);
	setFresh(true);
	return;
}

void Dp_normal::renderImGuiSettings() {
	// Nothing to see here ...
}

void Dp_normal::draw(ofColor c, float x, float y, float width, float height) {
	drawRectangle(x, y, width, height, c);
}
