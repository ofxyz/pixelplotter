#include "drawFilter_Noise.h"
#include <string>
#include "ofGraphics.h"
#include "ofAppRunner.h"

ofJson Df_noise::getSettings() {
	ofJson settings;
	settings["name"] = name;
	settings["_isOpen"] = _isOpen;

	settings["colors"]["background"]["r"] = cBg.x;
	settings["colors"]["background"]["g"] = cBg.y;
	settings["colors"]["background"]["b"] = cBg.z;
	settings["colors"]["background"]["w"] = cBg.w;

	for (auto col : palette) {
		settings["palette"][col->name]["name"] = col->name;
		settings["palette"][col->name]["r"] = col->color.x;
		settings["palette"][col->name]["g"] = col->color.y;
		settings["palette"][col->name]["b"] = col->color.z;
		settings["palette"][col->name]["a"] = col->color.w;
		settings["palette"][col->name]["percent"] = col->percent;
	}

	return settings;
}


void Df_noise::loadSettings(ofJson& settings) {
	try {
		//name = settings.value("name", name);
		_isOpen = settings.value("_isOpen", _isOpen);

		cBg.x = settings["colors"]["background"].value("r", cBg.x);
		cBg.y = settings["colors"]["background"].value("g", cBg.y);
		cBg.z = settings["colors"]["background"].value("b", cBg.z);
		cBg.w = settings["colors"]["background"].value("a", cBg.w);

		palette.clear();
		int counter = 1;
		for (auto& pal : settings["palette"]) {
			float r = pal.value("r", 0.0);
			float g = pal.value("g", 0.0);
			float b = pal.value("b", 0.0);
			float a = pal.value("a", 0.0);
			palette.push_back(new sColor(ofColor(r * 255, g * 255, b * 255, a * 255), pal.value("name", "Color " + std::to_string(counter)), pal.value("percent", 0)));
			counter++;
		}
	}
	catch (...) {
		ofLogNotice() << "Failed to load settings Df_noise";
	}
}

void Df_noise::renderImGuiSettings() {
	ImGui::SetNextItemOpen(_isOpen, ImGuiCond_Once);
	if (ImGui::CollapsingHeader(name.c_str(), &bAlive)) {
		_isOpen = true;
		ImGui::AlignTextToFramePadding();

		ImGui::PushItemWidth(60);

		if (ImGui::ColorEdit4("Background", (float*)&cBg, ImGuiColorEditFlags_NoInputs)) {
			setFresh(true);
		}
		// We need a palette manager...
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
				setFresh(true);
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
				setFresh(true);
			}
			ImGui::PopID();
		}

		if (deleteIndex >= 0) {
			palette.erase(palette.begin() + deleteIndex);
		}

		ImGui::PopItemWidth();

		if (ImGui::Button("Add Colour"))
		{
			palette.push_back(new sColor(ofColor(255, 255, 255, 255), "New " + std::to_string(palette.size())));
			setFresh(true);
		}

	} else {
		_isOpen = false;
	}
}

Df_noise::Df_noise(ofJson& settings)
	: Df_noise()
{
	loadSettings(settings);
}

Df_noise::Df_noise()
{
	pixelplotter = (ofApp*)ofGetAppPtr();
	name = "Noise";
}

void Df_noise::draw(ofImage* input, float width, float height) {
	setFresh(false);
	if (!bVisible) return;

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
			subpixels.push_back(subpixel(x, y));
		}
	}

	ofPushStyle();
	ofEnableAlphaBlending();
	ofSetColor((ofColor)cBg);
	ofDrawRectangle(x, y, 10, 10);

	for (auto col : palette) {
		if (col->percent > 0) {
			ofSetColor((ofColor)col->color);
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

Df_noise::subpixel::subpixel(int _x, int _y)
{
	x = _x;
	y = _y;
}

Df_noise::sColor::sColor(ofColor c, std::string n, int p /*= 0*/)
{
	color = c;
	name = n;
	percent = p;
}
