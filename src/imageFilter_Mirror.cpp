#include "imageFilter_Mirror.h"

ofxXmlSettings If_mirror::getSettings() {
	ofxXmlSettings settings;
	settings.setValue("name", name);
	settings.setValue("vMirror", vMirror);
	settings.setValue("hMirror", hMirror);
	return settings;
}

void If_mirror::loadSettings(ofxXmlSettings settings) {
	name = settings.getValue("name", "mirror");
	vMirror = settings.getValue("vMirror", false);
	hMirror = settings.getValue("hMirror", false);
	return;
}

void If_mirror::renderImGuiSettings() {
	if (ImGui::CollapsingHeader(name.c_str(), &active)) {
		ImGui::AlignTextToFramePadding();
		if (ImGui::Checkbox("Horizontal", &hMirror)) {
			bFresh = true;
		}
		if (ImGui::Checkbox("Vertical", &vMirror)) {
			bFresh = true;
		}
	}
}

void If_mirror::apply(ofImage* img) {
	img->mirror(vMirror, hMirror);
	bFresh = false;
}
