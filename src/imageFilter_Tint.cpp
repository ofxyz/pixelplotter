#include "imageFilter_Tint.h"

ofxXmlSettings If_tint::getSettings() {
	ofxXmlSettings settings;
	settings.setValue("name", name);
	settings.setValue("bGrey", bGrey);

	c_Tint.x = settings.getValue("colors:tint:r", c_Tint.x);
	c_Tint.y = settings.getValue("colors:tint:g", c_Tint.y);
	c_Tint.z = settings.getValue("colors:tint:b", c_Tint.z);
	c_Tint.w = settings.getValue("colors:tint:a", c_Tint.w);

	return settings;
}

void If_tint::loadSettings(ofxXmlSettings settings) {
	name = settings.getValue("name", "tint");
	bGrey = settings.getValue("bGrey", bGrey);

	settings.setValue("colors:tint:r", c_Tint.x);
	settings.setValue("colors:tint:g", c_Tint.y);
	settings.setValue("colors:tint:b", c_Tint.z);
	settings.setValue("colors:tint:a", c_Tint.w);

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
	cfbo.allocate(img->getWidth(), img->getHeight(), GL_RGB);
	cfbo.begin();
	cfbo.clearColorBuffer(ofColor(0, 0, 0));
	ofEnableAlphaBlending();
	ofPushStyle();
	ofSetColor(c_Tint);
	img->draw(0, 0, img->getWidth(), img->getHeight());
	ofPopStyle();
	ofDisableAlphaBlending();
	cfbo.end();
	cfbo.readToPixels(img->getPixelsRef());
	if (bGrey) {
		img->setImageType(OF_IMAGE_GRAYSCALE);
	}
	img->update();
	bFresh = false;
}
