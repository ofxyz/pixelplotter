#include "ofMain.h"
#include "generator_Stripes.h"

void G_stripes::setup(int _width = 100, int _height = 100) {
	if (_width  != 0) width  = _width;
	if (_height != 0) height = _height;
	bVisible = true;
	m_fbo.allocate(width, height);
	c_bg.w = 0;
	setFresh(true);
	update();
}

void G_stripes::update() {
	if (isFresh()) {
		if (!m_fbo.isAllocated() || width != m_fbo.getWidth() || height != m_fbo.getHeight()) {
			if (width != 0 && height != 0) m_fbo.allocate(width, height);
		}
		m_fbo.begin();
		ofClear((ofColor)c_bg);
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
	if (ImGui::DragInt("W ##G_Stripes", &width, 1, 1, 2400)) {
		setFresh(true);
	}
	ImGui::SameLine();
	if (ImGui::DragInt("H ##G_Stripes", &height, 1, 1, 2400)) {
		setFresh(true);
	}

	if (ImGui::DragInt("Count ##G_Stripes", &count, 1, 1, 1200)) {
		setFresh(true);
	}
	ImGui::SameLine();
	if (ImGui::Checkbox("Horizontal", &bHorizontal)) {
		setFresh(true);
	}

	ImGui::PopItemWidth();
	
	if (ImGui::DragFloat("Offset ##G_Stripes", &offset, 0.1, -offsetBounds, offsetBounds*0.5)) {
		setFresh(true);
	}

	if (ImGui::ColorEdit4("Stripe Colour ##G_Stripes", (float *)&c_x, ImGuiColorEditFlags_NoInputs)) {
		setFresh(true);
	}

	
}

ofJson G_stripes::getSettings() {
	ofJson settings;
	settings["name"] = name;
	settings["_isOpen"] = _isOpen;
	settings["bVisible"] = bVisible;
	settings["count"] = count;
	settings["offset"] = offset;

	settings["colors"]["c_bg"]["r"] = c_bg.x;
	settings["colors"]["c_bg"]["g"] = c_bg.y;
	settings["colors"]["c_bg"]["b"] = c_bg.z;
	settings["colors"]["c_bg"]["a"] = c_bg.w;

	settings["colors"]["c_x"]["r"] = c_x.x;
	settings["colors"]["c_x"]["g"] = c_x.y;
	settings["colors"]["c_x"]["b"] = c_x.z;
	settings["colors"]["c_x"]["a"] = c_x.w;

	return settings;
}

void G_stripes::loadSettings(ofJson & settings) {
	_isOpen  = settings.value("_isOpen", _isOpen);
	bVisible = settings.value("bVisible", bVisible);
	count  = settings.value("count", count);
	offset = settings.value("offset", offset);

	// Colours
	c_bg.x = settings["colors"]["c_bg"].value("r", c_bg.x);
	c_bg.y = settings["colors"]["c_bg"].value("g", c_bg.y);
	c_bg.z = settings["colors"]["c_bg"].value("b", c_bg.z);
	c_bg.w = settings["colors"]["c_bg"].value("a", c_bg.w);

	c_x.x = settings["colors"]["c_x"].value("r", c_x.x);
	c_x.y = settings["colors"]["c_x"].value("g", c_x.y);
	c_x.z = settings["colors"]["c_x"].value("b", c_x.z);
	c_x.w = settings["colors"]["c_x"].value("a", c_x.w);
}

void G_stripes::drawPattern() {
	float step = (float)height / (float)count; 
	if (bHorizontal) {
		step = (float)width / (float)count;
	}
	offsetBounds = step;

	ofPushStyle();
	ofFill();

	ofSetColor((ofColor)c_x);

	for (float s = 0; s <= count; s++) {
		if (bHorizontal) {
			ofDrawRectangle((s * step) + offset, 0, step * 0.5, height);
		} else {
			ofDrawRectangle(0, (s * step) + offset, width, step * 0.5);
		}
	}

	ofPopStyle();

}
