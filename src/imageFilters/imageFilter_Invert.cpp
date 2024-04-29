#include "imageFilter_invert.h"
#include "ImGui_General.h"

ofJson If_invert::getSettings() {
	ofJson settings;
	settings["name"] = name;
	settings["_isOpen"] = _isOpen;
	return settings;
}

void If_invert::loadSettings(ofJson& settings) {
	//name = settings.value("name", "duplicate");
	_isOpen = settings.value("_isOpen", _isOpen);
	return;
}

void If_invert::renderImGuiSettings() {
	ImGui::SetNextItemOpen(_isOpen, ImGuiCond_Once);
	if (ImGui::CollapsingHeader(name.c_str(), &bAlive)) {
		_isOpen = true;
		ImGui::AlignTextToFramePadding();

		renderUpDownButtons();

	} else {
		_isOpen = false;
	}
}

void If_invert::apply(ofImage* img) {
	if (!bVisible) return;
	for (int y = 0; y < img->getHeight(); y++) {
		for (int x = 0; x < img->getWidth(); x++) {
			img->setColor(x, y, img->getColor(x, y).invert());
		}
	}

	img->update();
	setFresh(false);
}
