#include "ofMain.h"
#include "generator_Dots.h"

void G_dots::setup(int _width = 100, int _height = 100) {
	if (_width != 0) width = _width;
	if (_height != 0) height = _height;
	diameter = std::min(width, height)*0.5;
	bVisible = true;
	m_fbo.allocate(width, height);
	setFresh(true);
	update();
}

void G_dots::update() {
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

void G_dots::draw() {
	if (!bVisible) return;
	drawPattern();
}

void G_dots::renderImGuiSettings() {

	ImGui::PushItemWidth(60);
	ImGui::Text("Size"); ImGui::SameLine(75);
	if (ImGui::DragInt("W ##G_Dots", &width, 10, width * 0.5, width)) {
		setFresh(true);
	}
	ImGui::SameLine();
	if (ImGui::DragInt("H ##G_Dots", &height, 10, width * 0.5, width)) {
		setFresh(true);
	}

	if (ImGui::ColorEdit4("Background Colour ##G_Dots", (float *)&c_bg, ImGuiColorEditFlags_NoInputs)) {
		setFresh(true);
	}

	if (ImGui::ColorEdit4("Dot Color ##G_Dots", (float *)&c_Dots, ImGuiColorEditFlags_NoInputs)) {
		setFresh(true);
	}

	if (ImGui::DragFloat("Diameter ##G_Dots", &diameter, 1, 1, width, "%.3f")) {
		setFresh(true);
	}

	if (ImGui::DragInt("Sides ##G_Dots", &count, 1, 3, 600)) {
		setFresh(true);
	}

	if (ImGui::DragFloat("Rotation ##G_Dots", &rotation, 1, -180, 180, "%.3f")) {
		setFresh(true);
	}

	ImGui::PopItemWidth();

}

ofJson G_dots::getSettings() {
	ofJson settings;
	settings["name"] = name;
	settings["_isOpen"] = _isOpen;
	settings["bVisible"] = bVisible;
	settings["count"] = count;
	settings["diameter"] = diameter;
	settings["rotation"] = rotation;

	settings["colors"]["c_bg"]["r"] = c_bg.x;
	settings["colors"]["c_bg"]["g"] = c_bg.y;
	settings["colors"]["c_bg"]["b"] = c_bg.z;
	settings["colors"]["c_bg"]["a"] = c_bg.w;

	settings["colors"]["c_Dots"]["r"] = c_Dots.x;
	settings["colors"]["c_Dots"]["g"] = c_Dots.y;
	settings["colors"]["c_Dots"]["b"] = c_Dots.z;
	settings["colors"]["c_Dots"]["a"] = c_Dots.w;

	return settings;
}

void G_dots::loadSettings(ofJson & settings) {
	_isOpen  = settings.value("_isOpen", _isOpen);
	bVisible = settings.value("bVisible", bVisible);
	count  = settings.value("count", count);
	diameter = settings.value("diameter", diameter);
	rotation = settings.value("rotation", rotation);

	// Colours
	c_bg.x = settings["colors"]["c_bg"].value("r", c_bg.x);
	c_bg.y = settings["colors"]["c_bg"].value("g", c_bg.y);
	c_bg.z = settings["colors"]["c_bg"].value("b", c_bg.z);
	c_bg.w = settings["colors"]["c_bg"].value("a", c_bg.w);

	c_Dots.x = settings["colors"]["c_Dots"].value("r", c_Dots.x);
	c_Dots.y = settings["colors"]["c_Dots"].value("g", c_Dots.y);
	c_Dots.z = settings["colors"]["c_Dots"].value("b", c_Dots.z);
	c_Dots.w = settings["colors"]["c_Dots"].value("a", c_Dots.w);
}

void G_dots::drawPattern() {

	ofPushStyle();
	ofFill();
	ofSetColor((ofColor)c_Dots);
	ofSetCircleResolution(count);
	ofPushMatrix();
	ofTranslate(width * 0.5, height * 0.5);
	ofRotate(rotation);
	ofDrawCircle(0, 0, 0, diameter);
	ofPopMatrix();
	ofPopStyle();

}
