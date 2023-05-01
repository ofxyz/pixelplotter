#pragma once

#include "generator_Plaids.h"

void G_plaids::setup(int _width, int _height) {
	width  = _width;
	height = _height;
	bFresh = true;
}

void G_plaids::update() {

}

void G_plaids::draw() {
	ofPushStyle();
	ofFill();
	ofSetColor(c_base);
	ofDrawRectangle(0, 100, width, 100);
	ofPopStyle();
}

void G_plaids::renderImGuiSettings() {

}

void G_plaids::loadSettings(ofxXmlSettings settings) {

}

ofxXmlSettings G_plaids::getSettings() {
	ofxXmlSettings settings;
	return settings;
}
