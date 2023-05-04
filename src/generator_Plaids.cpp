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
	if (ImGui::ColorEdit4("Background Colour", (float*)&c_bg, ImGuiColorEditFlags_NoInputs)) {
		bFresh = true;
	}

	ImGui::Separator(); // Start Size

	ImGui::PushItemWidth(60);
	ImGui::Text("Size"); ImGui::SameLine(75);
	if (ImGui::DragInt("W ##canvas_W", &width, 1, 1, 2400)) {
		bFresh = true;
	}
	ImGui::SameLine();
	if (ImGui::DragInt("H ##canvas_H", &height, 1, 1, 2400)) {
		bFresh = true;
	}
}

void G_plaids::loadSettings(ofxXmlSettings settings) {

}

ofxXmlSettings G_plaids::getSettings() {
	ofxXmlSettings settings;
	return settings;
}
