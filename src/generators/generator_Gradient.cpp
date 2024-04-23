#include "ofApp.h"
#include "generator_Gradient.h"

void G_gradient::setup(int _width /*= 100*/, int _height /*= 100*/) {
	if (_width  != 0) width  = _width;
	if (_height != 0) height = _height;
	bVisible = true;
	m_fbo.allocate(width, height);
	gradient.setupAsTurbo(10);
	setFresh(true);
	update();
}

void G_gradient::update() {
	if (isFresh()) {
		if (!m_fbo.isAllocated() || width != m_fbo.getWidth() || height != m_fbo.getHeight()) {
			if (width != 0 && height != 0) m_fbo.allocate(width, height);
		}
		m_fbo.begin();
		m_fbo.clearColorBuffer(ofColor(255,255,255,255));
		gradient.drawDebug(0, 0, m_fbo.getWidth(), m_fbo.getHeight(), numSteps, bDirFlip, bDrawVertical);
		m_fbo.end();
		setFresh(false);
	}
}

void G_gradient::draw() {
	if (!bVisible) return;
	drawPattern();
}

void G_gradient::renderImGuiSettings() {
	
	ImGui::PushItemWidth(60);
	if (ImGui::DragInt("Steps ##gradient_W", &numSteps, 1, 1, 2400)) {
		setFresh(true);
	}
	ImGui::SameLine();
	if (ImGui::Checkbox("Hard Mode", &bHardMode)) {
		gradient.setHardMode(bHardMode);
		setFresh(true);
	}

	if (ImGui::Checkbox("Flip Direction", &bDirFlip)) {
		setFresh(true);
	}
	if (ImGui::Checkbox("Vertical", &bDrawVertical)) {
		setFresh(true);
	}

	ImGui::PopItemWidth();

}

void G_gradient::loadSettings(ofJson& settings) {

}

ofJson G_gradient::getSettings() {
	ofJson settings;
	return settings;
}

void G_gradient::drawPattern() {
	ofPushStyle();
	ofFill();
	ofSetColor((ofColor)c_base);
	ofDrawRectangle(0, 0, width, height * 0.25);
	ofSetColor((ofColor)c_base2);
	ofDrawRectangle(0, 0, width*0.25, height);
	ofPopStyle();
}
