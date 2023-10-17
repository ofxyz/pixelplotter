#include "imageFilter_Duplicate.h"

// Add resize option?

ofJson If_duplicate::getSettings() {
	ofJson settings;
	settings["name"] = name;
	settings["hCount"] = hCount;
	settings["vCount"] = vCount;
	settings["bMirror"] = bMirror;
	return settings;
}

void If_duplicate::loadSettings(ofJson settings) {
	name = settings.value("name", "duplicate");
	hCount = settings.value("hCount", hCount);
	vCount = settings.value("vCount", vCount);
	bMirror = settings.value("bMirror", bMirror);
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
		ImGui::SameLine();
		if (ImGui::Checkbox("Mirror Align", &bMirror)) {
			bFresh = true;
		}

		ImGui::PopItemWidth();
	}
}

void If_duplicate::apply(ofImage* img) {
	ofFbo cfbo;
	cfbo.allocate(img->getWidth(), img->getHeight(), GL_RGBA);

	float width = img->getWidth() / hCount;
	float height = img->getHeight() / vCount;

	cfbo.begin();
	cfbo.clearColorBuffer(ofColor(0, 0, 0));
	int xcount = 0;
	int ycount = 0;
	for (float y = 0; y < vCount * height; y += height) {
		float newHeight = height;
		float newY = y;
		if (bMirror && (ycount % 2 == 1)) {
			newHeight = -height;
			newY = y + height;
		}
		for (float x = 0; x < hCount * width; x += width) {
			if (bMirror && (xcount % 2 == 0)) {
				img->draw(x + width, newY, -width, newHeight);
			}
			else {
				img->draw(x, newY, width, newHeight);
			}
			xcount++;
		}
		ycount++;
	}
	cfbo.end();

	cfbo.readToPixels(img->getPixelsRef());
	img->update();
	bFresh = false;
}
