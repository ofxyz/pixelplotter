#include "imageFilter_Blur.h"

ofJson If_blur::getSettings() {
	ofJson settings;
	settings["name"] = name;
	settings["_isOpen"] = _isOpen;
	settings["cvBlur"] = cvBlur;
	return settings;
}

void If_blur::loadSettings(ofJson& settings) {
	//name = settings.value("name", "tint");
	_isOpen = settings.value("_isOpen", _isOpen);
	cvBlur = settings.value("cvBlur", cvBlur);
	return;
}

void If_blur::renderImGuiSettings() {
	ImGui::SetNextItemOpen(_isOpen, ImGuiCond_Once);
	if (ImGui::CollapsingHeader(name.c_str(), &bAlive)) {
		_isOpen = true;
		ImGui::AlignTextToFramePadding();

		ImGui::PushItemWidth(100);
		if (ImGui::DragInt("Blur ##blur", &cvBlur, 1, 0, 500)) {
			setFresh(true);
		}
	} else {
		_isOpen = false;
	}
}

void If_blur::apply(ofImage* img) {
	ofPixels pix = img->getPixels();
	pix.setImageType(OF_IMAGE_COLOR);
	colorCvImage.allocate(img->getWidth(), img->getHeight());
	colorCvImage.setFromPixels(pix);
	colorCvImage.blur(cvBlur);

	pix = colorCvImage.getPixels();
	pix.setImageType(OF_IMAGE_COLOR_ALPHA);
	img->setFromPixels(pix);
	img->update();
	setFresh(false);
}
