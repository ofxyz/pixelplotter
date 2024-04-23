#include "ofMain.h"
#include "generator_Stripes.h"

void G_stripes::setup(int _width = 100, int _height = 100) {
	if (_width  != 0) width  = _width;
	if (_height != 0) height = _height;
	bVisible = true;
	m_fbo.allocate(width, height);
	setFresh(true);
	update();
}

void G_stripes::update() {
	if (isFresh()) {
		if (!m_fbo.isAllocated() || width != m_fbo.getWidth() || height != m_fbo.getHeight()) {
			if (width != 0 && height != 0) m_fbo.allocate(width, height);
		}
		m_fbo.begin();
		m_fbo.clearColorBuffer((ofColor)c_bg);
		drawPattern();
		m_fbo.end();
		setFresh(false);
	}
}

void G_stripes::draw() {
	if (!bVisible) return;
	drawPattern();
}

void G_stripes::renderImGuiSettings() {
	if (ImGui::ColorEdit4("Background Colour", (float*)&c_bg, ImGuiColorEditFlags_NoInputs)) {
		setFresh(true);
	}

	ImGui::PushItemWidth(60);
	ImGui::Text("Size"); ImGui::SameLine(75);
	if (ImGui::DragInt("W ##stripes_W", &width, 1, 1, 2400)) {
		setFresh(true);
	}
	ImGui::SameLine();
	if (ImGui::DragInt("H ##stripes_H", &height, 1, 1, 2400)) {
		setFresh(true);
	}
	ImGui::PopItemWidth();
}

void G_stripes::loadSettings(ofJson& settings) {

}

ofJson G_stripes::getSettings() {
	ofJson settings;
	return settings;
}

void G_stripes::drawPattern() {
	ofPushStyle();
	ofFill();
	ofSetColor((ofColor)c_base);
	ofDrawRectangle(0, 0, width, height * 0.25);
	ofSetColor((ofColor)c_base2);
	ofDrawRectangle(0, 0, width*0.25, height);
	ofPopStyle();
}
