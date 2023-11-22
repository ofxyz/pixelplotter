#include "ofApp.h"
#include "generator_Plaids.h"

void G_plaids::setup(int _width, int _height) {
	width = _width;
	height = _height;
	setFresh(true);
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
		setFresh(true);
	}

	ImGui::Separator(); // Start Size

	ImGui::PushItemWidth(60);
	ImGui::Text("Size"); ImGui::SameLine(75);
	if (ImGui::DragInt("W ##canvas_W", &width, 1, 1, 2400)) {
		setFresh(true);
	}
	ImGui::SameLine();
	if (ImGui::DragInt("H ##canvas_H", &height, 1, 1, 2400)) {
		setFresh(true);
	}
}

void G_plaids::loadSettings(ofJson& settings) {

}

ofJson G_plaids::getSettings() {
	ofJson settings;
	return settings;
}
