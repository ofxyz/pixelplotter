#include "imageFilter_Duplicate.h"
#include "ofFbo.h"
#include "ofApp.h"

// Add resize option?

ofJson If_duplicate::getSettings() {
	ofJson settings;
	settings["name"] = name;
	settings["_isOpen"] = _isOpen;
	settings["hCount"] = hCount;
	settings["vCount"] = vCount;
	settings["bMirror"] = bMirror;
	return settings;
}

void If_duplicate::loadSettings(ofJson& settings) {
	//name = settings.value("name", "duplicate");
	_isOpen = settings.value("_isOpen", _isOpen);
	hCount = settings.value("hCount", hCount);
	vCount = settings.value("vCount", vCount);
	bMirror = settings.value("bMirror", bMirror);
	return;
}

void If_duplicate::renderImGuiSettings() {
	ImGui::SetNextItemOpen(_isOpen, ImGuiCond_Once);
	if (ImGui::CollapsingHeader(name.c_str(), &bAlive)) {
		_isOpen = true;
		ImGui::AlignTextToFramePadding();
		ImGui::PushItemWidth(60);

		ImGui::Text("Copies"); ImGui::SameLine(75);
		if (ImGui::DragInt("Horz ##duplicate_hCount", &hCount, 1, 1, 100)) {
			setFresh(true);
		}
		ImGui::SameLine();
		if (ImGui::DragInt("Vert ##duplicate_vCount", &vCount, 1, 1, 100)) {
			setFresh(true);
		}
		ImGui::SameLine();
		if (ImGui::Checkbox("Mirror", &bMirror)) {
			setFresh(true);
		}

		ImGui::PopItemWidth();
	} else {
		_isOpen = false;
	}
}

void If_duplicate::apply(ofImage* img) {
	ofFbo cfbo;
	cfbo.allocate(img->getWidth(), img->getHeight(), GL_RGBA);

	float width = img->getWidth() / hCount;
	float height = img->getHeight() / vCount;

	cfbo.begin();
	cfbo.clearColorBuffer(ofColor(0, 0, 0, 0));

	for (int y = 0; y < vCount; y++) {
		ofPushMatrix();
		ofTranslate(0, y*height);
		for (int x = 0; x < hCount; x++) {
			ofPushMatrix();
			ofTranslate(x*width, 0);
			// Center for scale
			ofTranslate(width * 0.5, height * 0.5);
			if (bMirror && (x % 2 == 1)) {
				ofScale(-1, 1);
			}
			if (bMirror && (y % 2 == 1)) {
				ofScale(1, -1);
			}
			img->draw(-(width * 0.5), -(height * 0.5), width, height);
			ofPopMatrix();
		}
		ofPopMatrix();
	}
	cfbo.end();

	cfbo.readToPixels(img->getPixels());
	img->update();
	setFresh(false);
}
