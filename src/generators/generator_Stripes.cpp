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
	if (ImGui::DragInt("W ##G_Stripes", &width, 1, 1, 2400)) {
		setFresh(true);
	}
	ImGui::SameLine();
	if (ImGui::DragInt("H ##G_Stripes", &height, 1, 1, 2400)) {
		setFresh(true);
	}

	ImGui::Text("Stripe Count");
	ImGui::SameLine(75);
	if (ImGui::DragInt("X ##G_Stripes", &xcount, 1, 1, 1200)) {
		setFresh(true);
	}
	ImGui::SameLine();
	if (ImGui::DragInt("Y ##G_Stripes", &ycount, 1, 1, 1200)) {
		setFresh(true);
	}

	if (ImGui::ColorEdit4("Horizontal ##G_Stripes", (float *)&c_x, ImGuiColorEditFlags_NoInputs)) {
		setFresh(true);
	}
	if (ImGui::ColorEdit4("Vertical ##G_Stripes", (float *)&c_y, ImGuiColorEditFlags_NoInputs)) {
		setFresh(true);
	}

	ImGui::PopItemWidth();
}

ofJson G_stripes::getSettings() {
	ofJson settings;
	settings["name"] = name;
	settings["_isOpen"] = _isOpen;
	settings["bVisible"] = bVisible;
	settings["xcount"] = xcount;
	settings["ycount"] = ycount;

	settings["colors"]["c_x"]["r"] = c_x.x;
	settings["colors"]["c_x"]["g"] = c_x.y;
	settings["colors"]["c_x"]["b"] = c_x.z;
	settings["colors"]["c_x"]["a"] = c_x.w;

	settings["colors"]["c_y"]["r"] = c_y.x;
	settings["colors"]["c_y"]["g"] = c_y.y;
	settings["colors"]["c_y"]["b"] = c_y.z;
	settings["colors"]["c_y"]["a"] = c_y.w;

	return settings;
}

void G_stripes::loadSettings(ofJson & settings) {
	_isOpen = settings.value("_isOpen", _isOpen);
	bVisible = settings.value("bVisible", bVisible);
	xcount = settings.value("xcount", xcount);
	ycount = settings.value("ycount", ycount);

	// Colours
	c_x.x = settings["colors"]["c_x"].value("r", c_x.x);
	c_x.y = settings["colors"]["c_x"].value("g", c_x.y);
	c_x.z = settings["colors"]["c_x"].value("b", c_x.z);
	c_x.w = settings["colors"]["c_x"].value("a", c_x.w);

	c_y.x = settings["colors"]["c_y"].value("r", c_y.x);
	c_y.y = settings["colors"]["c_y"].value("g", c_y.y);
	c_y.z = settings["colors"]["c_y"].value("b", c_y.z);
	c_y.w = settings["colors"]["c_y"].value("a", c_y.w);
}

void G_stripes::drawPattern() {
	float stepX = (float)width / (float)xcount;
	float stepY = (float)height / (float)ycount;

	ofPushStyle();
	ofFill();

	ofSetColor((ofColor)c_x);
	for (float x = 0; x < xcount; x++) {
		ofDrawRectangle(x*stepX, 0, stepX*0.5, height);
	}

	ofSetColor((ofColor)c_y);
	for (float y = 0; y < ycount; y++) {
		ofDrawRectangle(0, y * stepY, width, stepY * 0.5);
	}

	ofPopStyle();
	
}
