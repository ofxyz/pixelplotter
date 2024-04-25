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

	//---------------------
	state.AddColorMarker(0.0f, { 1.0f, 1.0f, 1.0f }, 1.0f);
	state.AddColorMarker(1.0f, { 1.0f, 1.0f, 1.0f }, 1.0f);
	state.AddAlphaMarker(0.0f, 1.0f);
	state.AddAlphaMarker(1.0f, 1.0f);
	//---------------------
}

void G_gradient::update() {
	if (isFresh()) {
		if (!m_fbo.isAllocated() || width != m_fbo.getWidth() || height != m_fbo.getHeight()) {
			if (width != 0 && height != 0) m_fbo.allocate(width, height);
		}
		m_fbo.begin();
		m_fbo.clearColorBuffer(ofColor(255,255,255,255));
		//gradient.drawDebug(0, 0, m_fbo.getWidth(), m_fbo.getHeight(), numSteps, bDirFlip, bDrawVertical);
		drawPattern();
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
	
	

	bool isMarkerShown = true;
	ImGradientHDR(stateID, state, tempState, isMarkerShown);

	if (ImGui::IsItemHovered()) {
		ImGui::SetTooltip("Gradient");
	}

	if (tempState.selectedMarkerType == ImGradientHDRMarkerType::Color) {
		auto selectedColorMarker = state.GetColorMarker(tempState.selectedIndex);
		if (selectedColorMarker != nullptr) {
			ImGui::ColorEdit3("Color", selectedColorMarker->Color.data(), ImGuiColorEditFlags_Float);
			ImGui::DragFloat("Intensity", &selectedColorMarker->Intensity, 0.1f, 0.0f, 100.0f, "%f", 1.0f);
		}
	}

	if (tempState.selectedMarkerType == ImGradientHDRMarkerType::Alpha) {
		auto selectedAlphaMarker = state.GetAlphaMarker(tempState.selectedIndex);
		if (selectedAlphaMarker != nullptr) {
			ImGui::DragFloat("Alpha", &selectedAlphaMarker->Alpha, 0.1f, 0.0f, 1.0f, "%f", 1.0f);
		}
	}

	if (tempState.selectedMarkerType != ImGradientHDRMarkerType::Unknown) {
		if (ImGui::Button("Delete")) {
			if (tempState.selectedMarkerType == ImGradientHDRMarkerType::Color) {
				state.RemoveColorMarker(tempState.selectedIndex);
				tempState = ImGradientHDRTemporaryState {};
			} else if (tempState.selectedMarkerType == ImGradientHDRMarkerType::Alpha) {
				state.RemoveAlphaMarker(tempState.selectedIndex);
				tempState = ImGradientHDRTemporaryState {};
			}
		}
	}

}

void G_gradient::loadSettings(ofJson& settings) {

}

ofJson G_gradient::getSettings() {
	ofJson settings;
	return settings;
}

void G_gradient::drawPattern()
{
		float step;
		if (!bDrawVertical) {
			step = width / numSteps;
		} else {
			step = height / numSteps;
		}

		ofMesh m;
		m.setMode(OF_PRIMITIVE_TRIANGLE_STRIP);
		m.getVertices().reserve(numSteps * 2);
		m.getColors().reserve(numSteps * 2);

		for (float i = 0; i <= numSteps; i++) {
			std::array<float, 4> color;
			float pct = i / (float)numSteps;

			if (!bDirFlip) {
				color = state.GetCombinedColor(pct);
			} else {
				color = state.GetCombinedColor(1.0f - pct);
			}

			ofColor ofC = ofColor(color[0], color[1], color[2], color[3]);
			ofColor ofD = ofColor(255, 0, 0, 255);
			if (!bDrawVertical) {
				m.addColor(ofC);
				m.addVertex(ofVec3f(i * step, 0));
				m.addColor(ofC);
				m.addVertex(ofVec3f(i * step, height));
			} else {
				m.addColor(ofC);
				m.addVertex(ofVec3f(0, i * step));
				m.addColor(ofC);
				m.addVertex(ofVec3f(width, i * step));
			}
		}
		m.draw();
}
