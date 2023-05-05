#include "drawFilter_Noise.h"
#include "ofApp.h"

ofxXmlSettings Df_noise::getSettings() {
	ofxXmlSettings settings;
	settings.setValue("name", name);
	
	// Colours
	settings.setValue("colors:background:r", cBg.x);
	settings.setValue("colors:background:g", cBg.y);
	settings.setValue("colors:background:b", cBg.z);
	settings.setValue("colors:background:a", cBg.w);
	
	settings.addTag("palette");
	settings.pushTag("palette");
	// Palette
	int index = 0;
	for (auto col : palette) {
		settings.addTag("color");
		settings.pushTag("color", index++);
		settings.setValue("name", col->name);
		settings.setValue("r", col->color.x);
		settings.setValue("g", col->color.y);
		settings.setValue("b", col->color.z);
		settings.setValue("a", col->color.w);
		settings.setValue("percent", col->percent);
		settings.popTag();
	}
	settings.popTag();

	return settings;
}


void Df_noise::loadSettings(ofxXmlSettings settings) {
	name = settings.getValue("name", name);

	// Colours
	cBg.x = settings.getValue("colors:background:r", cBg.x);
	cBg.y = settings.getValue("colors:background:g", cBg.y);
	cBg.z = settings.getValue("colors:background:b", cBg.z);
	cBg.w = settings.getValue("colors:background:a", cBg.w);

	if (settings.tagExists("palette")) {
		settings.pushTag("palette");
		palette.clear();
		int colCount = settings.getNumTags("color");
		for (int i = 0; i < colCount; i++) {
			settings.pushTag("color", i);
			float r = settings.getValue("r", 0.0);
			float g = settings.getValue("g", 0.0);
			float b = settings.getValue("b", 0.0);
			float a = settings.getValue("a", 0.0);
			palette.push_back(new sColor(ofColor(r * 255, g * 255, b * 255, a * 255), settings.getValue("name", "Color " + to_string(i)), settings.getValue("percent", 0)));
			settings.popTag();
		}
		settings.popTag();
	}
}

void Df_noise::renderImGuiSettings() {
	if (ImGui::CollapsingHeader(name.c_str(), &active)) {
		ImGui::PushID("Df_noise");
		ImGui::AlignTextToFramePadding();

		renderUpDownButtons();

		ImGui::PushItemWidth(60);

		if (ImGui::ColorEdit4("Background", (float*)&cBg, ImGuiColorEditFlags_NoInputs)) {
			bFresh = true;
		}
		// We need a pallette manager...
		int index = 0;
		int deleteIndex = -1;
		for (auto col : palette) {
			ImGui::PushID(index++);
			if (ImGui::Button("Delete"))
			{
				deleteIndex = index - 1;
			}
			ImGui::SameLine();
			if (ImGui::ColorEdit4(col->name.c_str(), (float*)&col->color, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel)) {
				bFresh = true;
			}
			ImGui::SameLine();
			char colorName[40];
			memcpy(colorName, col->name.c_str(), 40);

			ImGui::PushItemWidth(120);
			if (ImGui::InputText("##ColorName", colorName, 40)) {
				col->name = colorName;
			}
			ImGui::PopItemWidth();

			std::string pName = "Percent ##" + col->name;
			if (ImGui::DragInt(pName.c_str(), &col->percent, 1, 1, 100)) {
				bFresh = true;
			}
			ImGui::PopID();
		}

		if (deleteIndex >= 0) {
			palette.erase(palette.begin() + deleteIndex);
		}

		ImGui::PopItemWidth();

		if (ImGui::Button("Add Colour"))
		{
			palette.push_back(new sColor(ofColor(255, 255, 255, 255), "New " + to_string(palette.size())));
			bFresh = true;
		}
		ImGui::PopID();
	}
}

void Df_noise::draw(ofImage* input, float width, float height, float x, float y) {
	bFresh = false;
	if (!visible) return;
	
	for (int y = 0; y < input->getHeight(); y += 10) {
		for (int x = 0; x < input->getWidth(); x += 10) {
			drawPixel(x, y);
		}
	}
}

void Df_noise::drawPixel(int x, int y) {
	
	// New Pixel
	subpixels.clear();
	for (int y = 0; y < 10; y++) {
		for (int x = 0; x < 10; x++) {
			subpixels.push_back(subpixel(x,y));
		}
	}

	ofPushStyle();
	ofSetColor(cBg);
	ofDrawRectangle(x, y, 10, 10);

	for (auto col : palette) {
		if (col->percent > 0) {
			ofSetColor(col->color);
			for (int count = 0; count < col->percent; count++) {
				if (subpixels.size() > 0) { // Make sure we have pixels left
					int p = ofRandom(subpixels.size() - 1);
					ofDrawRectangle(x + subpixels[p].x, y + subpixels[p].y, 1, 1);
					subpixels.erase(subpixels.begin() + p);
				}
			}
		}
	}

	ofPopStyle();
}
