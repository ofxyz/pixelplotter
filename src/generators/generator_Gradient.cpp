#include "ofApp.h"
#include "generator_Gradient.h"


void G_gradient::setup(int _width /*= 100*/, int _height /*= 100*/) {
	if (_width  != 0) width  = _width;
	if (_height != 0) height = _height;
	bVisible = true;
	m_fbo.allocate(width, height);

	//---------------------
	state.AddColorMarker(0.0f, { 0.0f, 0.0f, 0.0f }, 1.0f);
	state.AddColorMarker(1.0f, { 1.0f, 1.0f, 1.0f }, 1.0f);
	state.AddAlphaMarker(0.0f, 1.0f);
	state.AddAlphaMarker(1.0f, 1.0f);
	//---------------------

	setFresh(true);
	update();
}

void G_gradient::update() {
	if (isFresh()) {
		if (!m_fbo.isAllocated() || width != m_fbo.getWidth() || height != m_fbo.getHeight()) {
			if (width != 0 && height != 0) m_fbo.allocate(width, height);
		}
		m_fbo.begin();
		m_fbo.clearColorBuffer(ofColor(255,255,255,0));
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
	ImGui::Indent();
	ImGui::Separator();
	ImGui::AlignTextToFramePadding();
	// TODO: Add ratio's dropdown (1:1, 1:1.25, Custom)
	ImGui::PushItemWidth(60);
	ImGui::Text("Size");
	ImGui::SameLine(75);
	if (ImGui::DragInt("W ##Gradient", &width, 1, 3, 2400)) {
		setFresh(true);
	}
	ImGui::SameLine();
	if (ImGui::Button("<-> ##Gradient")) {
		int w = width;
		width = height;
		height = w;
		setFresh(true);
	}
	ImGui::SameLine();
	if (ImGui::DragInt("H ##Gradient", &height, 1, 3, 2400)) {
		setFresh(true);
	}
	ImGui::SameLine();
	if (ImGui::Button("x2 ##Gradient")) {
		width *= 2;
		height *= 2;
		setFresh(true);
	}
	ImGui::PopItemWidth();

	ImGui::PushItemWidth(60);
	if (ImGui::DragInt("Steps ##gradient_W", &numSteps, 1, 1, 2400)) {
		setFresh(true);
	}

	if (ImGui::Checkbox("Flip Direction ##Gradient", &bDirFlip)) {
		setFresh(true);
	}
	if (ImGui::Checkbox("Vertical ##Gradient", &bDrawVertical)) {
		setFresh(true);
	}

	ImGui::PopItemWidth();

	bool isMarkerShown = true;
	if (ImGradientHDR(stateID, state, tempState, isMarkerShown)) {
		setFresh(true);
	}

	if (tempState.selectedMarkerType == ImGradientHDRMarkerType::Color) {
		auto selectedColorMarker = state.GetColorMarker(tempState.selectedIndex);
		if (selectedColorMarker != nullptr) {
			if (ImGui::ColorEdit3("Color", selectedColorMarker->Color.data(), ImGuiColorEditFlags_Float)) {
				setFresh(true);
			}
			if (ImGui::DragFloat("Intensity", &selectedColorMarker->Intensity, 0.1f, 0.0f, 100.0f, "%f", 1.0f)) {
				setFresh(true);
			}
		}
	}

	if (tempState.selectedMarkerType == ImGradientHDRMarkerType::Alpha) {
		auto selectedAlphaMarker = state.GetAlphaMarker(tempState.selectedIndex);
		if (selectedAlphaMarker != nullptr) {
			if (ImGui::DragFloat("Alpha", &selectedAlphaMarker->Alpha, 0.1f, 0.0f, 1.0f, "%f", 1.0f)) {
				setFresh(true);
			}
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
			setFresh(true);
		}
	}
	ImGui::Unindent();
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
			step = (float)width / (float)numSteps;
		} else {
			step = (float)height / (float)numSteps;
		}

		ofMesh m;
		m.setMode(OF_PRIMITIVE_TRIANGLE_STRIP);
		m.getVertices().reserve(numSteps * 2);
		m.getColors().reserve(numSteps * 2);

		for (float i = 0; i <= numSteps; i++) {
			std::array<float, 4> color;
			float pct = i / (float)numSteps;
			ofLog() << "PCT: " << pct << endl;

			if (!bDirFlip) {
				color = state.GetCombinedColor(pct);
			} else {
				color = state.GetCombinedColor(1.0f - pct);
			}

			ofColor ofC = ofColor(color[0] * 255.f, color[1] * 255.f, color[2] * 255.f, color[3] * 255.f);

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
