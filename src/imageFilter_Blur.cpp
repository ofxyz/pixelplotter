#include "imageFilter_Blur.h"

ofxXmlSettings If_blur::getSettings() {
	ofxXmlSettings settings;
	settings.setValue("name", name);
	settings.setValue("cvBlur", cvBlur);
	return settings;
}

void If_blur::loadSettings(ofxXmlSettings settings) {
	name = settings.getValue("name", "tint");
	cvBlur = settings.getValue("cvBlur", cvBlur);
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
	bFresh = false;
}
