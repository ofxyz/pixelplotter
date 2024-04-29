#include "imageFilter_Dither.h"
#include "ofMain.h"
#include "ImGui_General.h"

ofJson If_dither::getSettings() {
	ofJson settings;
	settings["name"] = name;
	settings["_isOpen"] = _isOpen;
	settings["_bDither"] = _bDither;
	settings["_bGrey"] = _bGrey;
	settings["_iFactor"] = _iFactor;
	return settings;
}

void If_dither::loadSettings(ofJson & settings) {
	_isOpen = settings.value("_isOpen", _isOpen);
	_bDither = settings.value("_bDither", _bDither);
	_bGrey = settings.value("_bGrey", _bGrey);
	_iFactor = settings.value("_iFactor", _iFactor);
	return;
}

void If_dither::renderImGuiSettings() {
	ImGui::SetNextItemOpen(_isOpen, ImGuiCond_Once);
	if (ImGui::CollapsingHeader(name.c_str(), &bAlive)) {
		_isOpen = true;
		ImGui::AlignTextToFramePadding();

		renderUpDownButtons();

		if (ImGui::SliderInt("Factor ##iF_dither", &_iFactor, 0, 255)) setFresh(true);
		ImGui::SameLine();
		ImGui::HelpMarker("Above 0 will index colors. 0 creates a hard 8bit effect");

		if (ImGui::Checkbox("Dither ##iF_dither", &_bDither)) {
			setFresh(true);
		}
		ImGui::SameLine();
		if (ImGui::Checkbox("Grey ##iF_dither", &_bGrey)) {
			setFresh(true);
		}

	} else {
		_isOpen = false;
	}
}

void If_dither::addError(ofImage * img, float factor, glm::vec2 pos, glm::vec3 err) {
	if (pos.x < 0 || pos.x >= img->getWidth() || pos.y < 0 || pos.y >= img->getHeight()) return;
	ofColor c = img->getColor(pos.x, pos.y);
	c.r = ofClamp(c.r + err.x * factor, 0, 255);
	c.g = ofClamp(c.g + err.y * factor, 0, 255);
	c.b = ofClamp(c.b + err.z * factor, 0, 255);
	img->setColor(pos.x, pos.y, c);
}

void If_dither::distributeError(ofImage * img, glm::vec2 pos, glm::vec3 err) {
	// Add your own variable float besides the Floyd-Steinberg Dither?
	addError(img, FloydSteinberg[0], glm::vec2(pos.x + 1, pos.y), err);
	addError(img, FloydSteinberg[1], glm::vec2(pos.x - 1, pos.y + 1), err);
	addError(img, FloydSteinberg[2], glm::vec2(pos.x, pos.y + 1), err);
	addError(img, FloydSteinberg[3], glm::vec2(pos.x + 1, pos.y + 1), err);
}

void If_dither::apply(ofImage* img) {
	if (!bVisible) return;
	if (_bGrey) img->setImageType(OF_IMAGE_GRAYSCALE);

	ofPushStyle();
	for (int y = 0; y < img->getHeight(); y++) {
		for (int x = 0; x < img->getWidth(); x++) {
			ofColor c = img->getColor(floor(x), floor(y));
			ofColor oc(c); // Original Color
			if (_iFactor > 0) {
				c.r = std::round((float)_iFactor * c.r / 255.f) * std::floor((float)255 / (float)_iFactor);
				c.g = std::round((float)_iFactor * c.g / 255.f) * std::floor((float)255 / (float)_iFactor);
				c.b = std::round((float)_iFactor * c.b / 255.f) * std::floor((float)255 / (float)_iFactor);
			} else { // 8Bit
				(c.r > 127) ? c.r = 255 : c.r = 0;
				(c.g > 127) ? c.g = 255 : c.g = 0;
				(c.b > 127) ? c.b = 255 : c.g = 0;
			}
	
			img->setColor(x, y, c);

			if (_bDither) {
				distributeError(img, glm::vec2(x, y), glm::vec3(oc.r - c.r, oc.g - c.g, oc.b - c.b));
			}
		}
	}
	ofPopStyle();
	img->update();
	setFresh(false);
}
