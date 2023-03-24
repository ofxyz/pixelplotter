#include "imageFilter_Mirror.h"

ofxXmlSettings If_mirror::getSettings() {
	ofxXmlSettings settings;
	return settings;
}

void If_mirror::loadSettings(ofxXmlSettings settings) {
	return;
}

void If_mirror::renderImGuiSettings() {
	if (ImGui::CollapsingHeader(name.c_str(), &active)) {
		ImGui::AlignTextToFramePadding();
		ImGui::Checkbox("Horizontal", &hMirror);
		ImGui::Checkbox("Vertical", &vMirror);
	}
}

void If_mirror::apply(ofImage* img) {
	img->mirror(vMirror, hMirror);
}
