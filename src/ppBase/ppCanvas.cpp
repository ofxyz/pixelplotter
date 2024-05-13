#include "ppCanvas.h"
#include "ofApp.h"

ppCanvas::ppCanvas()
	:ppTexture() {
	// 0
}

bool ppCanvas::update() {
	return false;
}

void ppCanvas::renderImGuiSettings(bool bWrapped /* 1 */) {

	if (bWrapped) {
		ImGui::SetNextItemOpen(_bIsOpen, ImGuiCond_Once);
		if (ImGui::CollapsingHeader(getName().c_str(), &_bAlive)) {
			setOpen(true);
			renderImGuiSettings(!bWrapped /* false */);
		}
		else {
			setOpen(false);
		}
	}

	ImGui::AlignTextToFramePadding();
	ImGui::InputText("Name", &_sName);

	ImGui::AlignTextToFramePadding();
	ImGui::Text("Size"); ImGui::SameLine(60);

	ImGui::PushItemWidth(80);
	if (ImGui::DragScalar("W", ImGuiDataType_U32, &_width)) {
		setFresh(true);
	}
	ImGui::PopItemWidth();
	ImGui::SameLine();
	ImGui::PushItemWidth(80);
	if (ImGui::DragScalar("H", ImGuiDataType_U32, &_height)) {
		setFresh(true);
	}
	ImGui::PopItemWidth();

	if (ImGui::ColorEdit4("Background", (float*)&cBG, ImGuiColorEditFlags_NoInputs)) {
		setFresh(true);
	}
}

ofJson ppCanvas::getSettings() {
	ofJson settings;
	return settings;
}

void ppCanvas::loadSettings(ofJson & settings) {
}

void ppCanvas::draw(float x, float y, float w, float h) {
	_tFbo.begin();
	ofClear(255, 255, 255, 0);
	ofSetColor(ofColor(cBG));
	ofDrawRectangle(0, 0, _width, _height);
	_tFbo.end();
	_tFbo.draw(x, y, w, h);
}
