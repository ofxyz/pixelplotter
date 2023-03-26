#include "imageFilter_Duplicate.h"

// Add resize option?

ofxXmlSettings If_duplicate::getSettings() {
	ofxXmlSettings settings;
	return settings;
}

void If_duplicate::loadSettings(ofxXmlSettings settings) {
	return;
}

void If_duplicate::renderImGuiSettings() {
	if (ImGui::CollapsingHeader(name.c_str(), &active)) {
		ImGui::AlignTextToFramePadding();
		ImGui::PushItemWidth(60);
		ImGui::Text("Copies"); ImGui::SameLine(75);
		if (ImGui::DragInt("Horz ##duplicate_hCount", &hCount, 1, 1, 100)) {
			bFresh = true;
		}
		ImGui::SameLine();
		if (ImGui::DragInt("Vert ##duplicate_vCount", &vCount, 1, 1, 100)) {
			bFresh = true;
		}
		ImGui::PopItemWidth();
	}
}

void If_duplicate::apply(ofImage* img) {
	ofFbo cfbo;
	cfbo.allocate(img->getWidth(), img->getHeight(), GL_RGB);

	float width = img->getWidth() / hCount;
	float height = img->getHeight() / vCount;

	cfbo.begin();
	cfbo.clearColorBuffer(ofColor(255,255,255));
	for (float y = 0; y < vCount* height; y+= height) {
		for (float x = 0; x < hCount*width; x+= width) {
			img->draw(x,y,width,height);
		}
	}

	cfbo.end();

	cfbo.readToPixels(img->getPixelsRef());
	img->update();
	bFresh = false;
}
