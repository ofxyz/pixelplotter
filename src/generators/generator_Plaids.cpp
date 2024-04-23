#include "ofApp.h"
#include "generator_Plaids.h"

void G_plaids::setup(int _width, int _height) {
	width = _width;
	height = _height;
	bVisible = true;
	m_fbo.allocate(width, height);
	setFresh(true);
	update();
}

void G_plaids::update() {
	if (isFresh()) {
		m_fbo.begin();
		m_fbo.clearColorBuffer(ofColor(255, 255, 255, 0));
		drawPattern();
		m_fbo.end();
		setFresh(false);
	}
}

void G_plaids::draw() {
	if (!bVisible) return;
	drawPattern();
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

void G_plaids::drawPattern() {
	ofPushStyle();
	ofFill();
	ofSetColor((ofColor)c_base);
	ofDrawRectangle(0, 0, width, height * 0.25);
	ofPopStyle();
}
