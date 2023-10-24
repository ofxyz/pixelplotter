#include "imageFilter_Tint.h"

ofJson If_tint::getSettings() {
	ofJson settings;
	settings["name"] = name;
	settings["bGrey"] = bGrey;

	settings["colors"]["tint"]["r"] = c_Tint.x;
	settings["colors"]["tint"]["g"] = c_Tint.y;
	settings["colors"]["tint"]["b"] = c_Tint.z;
	settings["colors"]["tint"]["a"] = c_Tint.w;

	return settings;
}

void If_tint::loadSettings(ofJson settings) {
	//name = settings.value("name", "tint");
	bGrey = settings.value("bGrey", bGrey);

	c_Tint.x = settings.value("colors:tint:r", c_Tint.x);
	c_Tint.y = settings.value("colors:tint:g", c_Tint.y);
	c_Tint.z = settings.value("colors:tint:b", c_Tint.z);
	c_Tint.w = settings.value("colors:tint:a", c_Tint.w);

	return;
}

void If_tint::renderImGuiSettings() {
	if (ImGui::CollapsingHeader(name.c_str(), &active)) {
		ImGui::AlignTextToFramePadding();

		if (ImGui::ColorEdit4("Colorize ##duplicate", (float*)&c_Tint, ImGuiColorEditFlags_NoInputs)) {
			bFresh = true;
		}
		ImGui::SameLine();
		if (ImGui::Checkbox("Grey", &bGrey)) {
			bFresh = true;
		}
	}
}

void If_tint::apply(ofImage* img) {
	ofFbo cfbo;
	cfbo.allocate(img->getWidth(), img->getHeight(), GL_RGBA);
	cfbo.begin();
	cfbo.clearColorBuffer(ofColor(0, 0, 0));
	ofEnableAlphaBlending();
	ofPushStyle();
	ofSetColor(c_Tint);
	img->draw(0, 0, img->getWidth(), img->getHeight());
	ofPopStyle();
	ofDisableAlphaBlending();
	cfbo.end();
	cfbo.readToPixels(img->getPixels());
	if (bGrey) {
		img->setImageType(OF_IMAGE_GRAYSCALE);
	}
	img->update();
	bFresh = false;
}
