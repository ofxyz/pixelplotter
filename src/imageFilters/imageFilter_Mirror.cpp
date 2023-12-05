#include "imageFilter_Mirror.h"

ofJson If_mirror::getSettings() {
	ofJson settings;
	settings["name"] = name;
	settings["_isOpen"] = _isOpen;
	settings["vMirror"] = vMirror;
	settings["hMirror"] = hMirror;
	return settings;
}

void If_mirror::loadSettings(ofJson& settings) {
	//name = settings.value("name", "Mirror");
	_isOpen = settings.value("_isOpen", _isOpen);
	vMirror = settings.value("vMirror", false);
	hMirror = settings.value("hMirror", false);
	return;
}

void If_mirror::renderImGuiSettings() {
	ImGui::SetNextItemOpen(_isOpen);
	if (ImGui::CollapsingHeader(name.c_str(), &bAlive)) {
		ImGui::AlignTextToFramePadding();
		if (ImGui::Checkbox("Horizontal", &hMirror)) {
			setFresh(true);
		}
		if (ImGui::Checkbox("Vertical", &vMirror)) {
			setFresh(true);
		}
	}
}

void If_mirror::apply(ofImage* img) {
	img->mirror(vMirror, hMirror);
	setFresh(false);
}
