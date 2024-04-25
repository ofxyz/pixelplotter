#include "imageFilter_8Bit.h"
#include "ofMain.h"
#include "ImGui_General.h"

ofJson If_8bit::getSettings() {
	ofJson settings;
	settings["name"] = name;
	settings["_isOpen"] = _isOpen;
	return settings;
}


void If_8bit::addError(ofImage * img, float factor, glm::vec2 pos, glm::vec3 err) {
	if (pos.x < 0 || pos.x >= img->getWidth() || pos.y < 0 || pos.y >= img->getHeight()) return;
	ofColor c = img->getColor(pos.x,pos.y);
	c.r = ofClamp(c.r + err.x * factor, 0, 255);
	c.g = ofClamp(c.g + err.y * factor, 0, 255);
	c.b = ofClamp(c.b + err.z * factor, 0, 255);
	img->setColor(pos.x, pos.y, c);
}

void If_8bit::distributeError(ofImage * img, glm::vec2 pos, glm::vec3 err) {
	// Add your own variable float besides the Floyd-Steinberg Dither?
	addError(img, (7.0f / 16.0f), glm::vec2(pos.x + 1, pos.y    ), err);
	addError(img, (3.0f / 16.0f), glm::vec2(pos.x - 1, pos.y + 1), err);
	addError(img, (5.0f / 16.0f), glm::vec2(pos.x,     pos.y + 1), err);
	addError(img, (1.0f / 16.0f), glm::vec2(pos.x + 1, pos.y + 1), err);
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

		if (ImGui::Checkbox("Dither", &bDither)) {
			setFresh(true);
		}

		ImGui::PopItemWidth();
	} else {
		_isOpen = false;
	}
}

void If_8bit::apply(ofImage* img) {
	ofPushStyle();
	for (int y = 0; y < img->getHeight(); y++) {
		for (int x = 0; x < img->getWidth(); x++) {
			ofColor c = img->getColor(floor(x), floor(y));
			ofColor oc(c); // Original Color
			if (factor > 0) {
				c.r = std::round((float)factor * c.r / 255.f) * std::floor((float)255 / (float)factor);
				c.g = std::round((float)factor * c.g / 255.f) * std::floor((float)255 / (float)factor);
				c.b = std::round((float)factor * c.b / 255.f) * std::floor((float)255 / (float)factor);
			} else { // 8Bit
				(c.r > 127) ? c.r = 255 : c.r = 0;
				(c.g > 127) ? c.g = 255 : c.g = 0;
				(c.b > 127) ? c.b = 255 : c.g = 0;
			}
			img->setColor(x, y, c);

			if (bDither) {
				distributeError(img, glm::vec2(x, y), glm::vec3(oc.r - c.r, oc.g - c.g, oc.b - c.b));
			}
		}
	}
	ofPopStyle();

	img->update();
	setFresh(false);
}
