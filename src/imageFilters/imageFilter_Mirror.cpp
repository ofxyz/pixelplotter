#include "imageFilter_Mirror.h"

ofJson If_mirror::getSettings() {
	ofJson settings;
	settings["name"] = name;
	settings["vMirror"] = vMirror;
	settings["hMirror"] = hMirror;
	return settings;
}

void If_mirror::loadSettings(ofJson& settings) {
	name = settings.value("name", "mirror");
	vMirror = settings.value("vMirror", false);
	hMirror = settings.value("hMirror", false);
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
