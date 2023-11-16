#include "imageFilter_Blur.h"

ofJson If_blur::getSettings() {
	ofJson settings;
	settings["name"] = name;
	settings["cvBlur"] = cvBlur;
	return settings;
}

void If_blur::loadSettings(ofJson& settings) {
	name = settings.value("name", "tint");
	cvBlur = settings.value("cvBlur", cvBlur);
	return;
}

void If_blur::renderImGuiSettings() {
	if (ImGui::CollapsingHeader(name.c_str(), &active)) {
		ImGui::AlignTextToFramePadding();

		ImGui::PushItemWidth(100);
		if (ImGui::DragInt("Blur ##blur", &cvBlur, 1, 0, 500)) {
			setFresh(true);
		}
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
