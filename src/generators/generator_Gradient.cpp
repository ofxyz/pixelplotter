#include "ofApp.h"
#include "generator_Gradient.h"


void G_gradient::setup(int _width /*= 100*/, int _height /*= 100*/) {
	if (_width  != 0) width  = _width;
	if (_height != 0) height = _height;
	bVisible = true;
	m_fbo.allocate(width, height);

	if(state.ColorCount == 0) {
		state.AddColorMarker(0.0f, { 0.0f, 0.0f, 0.0f }, 1.0f);
		state.AddColorMarker(1.0f, { 1.0f, 1.0f, 1.0f }, 1.0f);
	}
	if (state.AlphaCount == 0) {
		state.AddAlphaMarker(0.0f, 1.0f);
		state.AddAlphaMarker(1.0f, 1.0f);
	}

	setFresh(true);
	update();
}

void G_gradient::update() {
	if (isFresh()) {
		if (!m_fbo.isAllocated() || width != m_fbo.getWidth() || height != m_fbo.getHeight()) {
			if (width != 0 && height != 0) m_fbo.allocate(width, height);
		}
		m_fbo.begin();
		//m_fbo.clearColorBuffer(ofColor(255,255,255,0));
		ofClear(ofColor(255,255,255,0));
		drawPattern();
		m_fbo.end();
		setFresh(false);
	}
}

void G_gradient::draw() {
	if (!bVisible) return;
	drawPattern();
	setFresh(false);
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
	try {
		_isOpen = settings.value("_isOpen", _isOpen);
		width = settings.value("width", width);
		height = settings.value("height", height);
		numSteps = settings.value("numSteps", numSteps);
		bDirFlip = settings.value("bDirFlip", bDirFlip);
		bDrawVertical = settings.value("bDrawVertical", bDrawVertical);

		// First clear previous markers
		while (state.ColorCount > 0) {
			state.RemoveColorMarker(0);
		}
		while (state.AlphaCount > 0) {
			state.RemoveAlphaMarker(0);
		}

		// Now Load Gradient Data
		int colorCount = 0;
		colorCount = settings.value("colorCount", colorCount);
		for (int i = 0; i < colorCount; i++) {
			state.AddColorMarker(settings["colorMarkers"][i][0],
				{ settings["colorMarkers"][i][1],
				  settings["colorMarkers"][i][2],
				  settings["colorMarkers"][i][3] },
			settings["colorMarkers"][i][4]);
		}
		int alphaCount = 0;
		alphaCount = settings.value("alphaCount", alphaCount);
		for (int i = 0; i < alphaCount; i++) {
			state.AddAlphaMarker(settings["alphaMarkers"][i][0], settings["alphaMarkers"][i][1]);
		}
	} catch (...) {
		ofLogNotice() << "Failed to load settings G_gradient";
	}
}

ofJson G_gradient::getSettings() {
	ofJson settings;
	settings["name"] = name;
	settings["_isOpen"] = _isOpen;
	settings["width"] = width;
	settings["height"] = height;
	settings["numSteps"] = numSteps;
	settings["bDirFlip"] = bDirFlip;
	settings["bDrawVertical"] = bDrawVertical;
	// Save Gradient Data
	settings["colorCount"] = state.ColorCount;
	for (int i = 0; i < state.ColorCount; i++) {
		auto selectedColorMarker = state.GetColorMarker(i);
		if (selectedColorMarker != nullptr) {
			// Position
			settings["colorMarkers"][i][0] = selectedColorMarker->Position;
			// RGB Color
			settings["colorMarkers"][i][1] = selectedColorMarker->Color[0];
			settings["colorMarkers"][i][2] = selectedColorMarker->Color[1];
			settings["colorMarkers"][i][3] = selectedColorMarker->Color[2];
			// Intensity
			settings["colorMarkers"][i][4] = selectedColorMarker->Intensity;
		}
	}
	settings["alphaCount"] = state.AlphaCount;
	for (int i = 0; i < state.AlphaCount; i++) {
		auto selectedAlphaMarker = state.GetAlphaMarker(i);
		if (selectedAlphaMarker != nullptr) {
			settings["alphaMarkers"][i][0] = selectedAlphaMarker->Position;
			settings["alphaMarkers"][i][1] = selectedAlphaMarker->Alpha;
		}
	}

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
