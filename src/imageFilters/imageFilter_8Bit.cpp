#include "imageFilter_8Bit.h"
#include "ofMain.h"
#include "ImGui_General.h"

ofJson If_8bit::getSettings() {
	ofJson settings;
	settings["name"] = name;
	settings["_isOpen"] = _isOpen;
	return settings;
}

void If_8bit::loadSettings(ofJson& settings) {
	//name = settings.value("name", "duplicate");
	_isOpen = settings.value("_isOpen", _isOpen);
	return;
}

void If_8bit::renderImGuiSettings() {
	ImGui::SetNextItemOpen(_isOpen, ImGuiCond_Once);
	if (ImGui::CollapsingHeader(name.c_str(), &bAlive)) {
		_isOpen = true;
		ImGui::AlignTextToFramePadding();

		ImGui::PushItemWidth(60);
		if (ImGui::DragInt("Factor ##8bit", &factor, 1, 0, 255)) {
			setFresh(true);
		}
		ImGui::SameLine();
		ImGui::HelpMarker("Above 0 will index colour based on factor.");
		ImGui::PopItemWidth();
	} else {
		_isOpen = false;
	}
}

void If_8bit::apply(ofImage* img) {
	ofPushStyle();
	for (int y = 0; y < img->getHeight(); y++) {
		for (int x = 0; x < img->getWidth(); x++) {
			ofColor c = img->getPixels().getColor(floor(x), floor(y));
			if (factor > 0) {
				c.r = std::floor(std::round(factor * c.r / 255) * (255 / factor));
				c.g = std::floor(std::round(factor * c.g / 255) * (255 / factor));
				c.b = std::floor(std::round(factor * c.b / 255) * (255 / factor));
			} else { // 8Bit
				(c.r > 127) ? c.r = 255 : c.r = 0;
				(c.g > 127) ? c.g = 255 : c.g = 0;
				(c.b > 127) ? c.b = 255 : c.g = 0;
			}
			img->setColor(x, y, c);
		}
	}
	ofPopStyle();

	img->update();
	setFresh(false);
}
