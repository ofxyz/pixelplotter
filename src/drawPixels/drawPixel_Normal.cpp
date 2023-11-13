#include "drawPixel_Normal.h"

ofJson Dp_normal::getSettings() {
	ofJson settings;
	settings["name"] = name;
	return settings;
}

void Dp_normal::loadSettings(ofJson& settings) {
	//name = settings.value("name", name);
	setFresh(true);
	return;
}

void Dp_normal::renderImGuiSettings() {
	// Nothing to see here ...
}

void Dp_normal::draw(ofColor c, glm::vec2 dim, glm::vec2 pos /*= { 0,0 }*/, glm::vec2 posNorm /*= { 0,0 }*/)
{
	drawRectangle(0, 0, dim.x, dim.y, c);
}
