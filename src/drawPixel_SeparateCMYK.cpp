#include "drawPixel_SeparateCMYK.h"

ofxXmlSettings Dp_separateCMYK::getSettings() {
	ofxXmlSettings settings;
	settings.setValue("name", name);
	return settings;
}

void Dp_separateCMYK::loadSettings(ofxXmlSettings settings) {
	// name = settings.getValue("name", name);
	return;
}

void Dp_separateCMYK::renderImGuiSettings() {

}

void Dp_separateCMYK::draw(ofColor c, float x, float y, float width, float height) {
	
}
