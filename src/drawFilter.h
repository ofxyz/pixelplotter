#pragma once
#include "ofxImGui.h"
#include "ofxXmlSettings.h"

class DrawFilter {
public:
	bool active = true;
	bool visible = true;
	bool useMask = false;
	int maskMargin = 0;

	std::string name;

	bool bFresh = false;
	bool isFresh() {
		return bFresh;
	}

	virtual void draw(ofImage* input) = 0;
	virtual void renderImGuiSettings() = 0;
	virtual void loadSettings(ofxXmlSettings settings) = 0;
	virtual ofxXmlSettings getSettings() = 0;

	ofVec4f getCMYK(ofColor rgb) {
		double dr = (double)rgb.r / 255;
		double dg = (double)rgb.g / 255;
		double db = (double)rgb.b / 255;
		double k = 1 - max(max(dr, dg), db);
		double c = (1 - dr - k) / (1 - k);
		double m = (1 - dg - k) / (1 - k);
		double y = (1 - db - k) / (1 - k);

		return ofVec4f(c, m, y, k);
	}

	int currentBlendModeIndex = 0;
	std::vector<std::string> v_BlendModes{ "OF_BLENDMODE_DISABLED", "OF_BLENDMODE_ALPHA", "OF_BLENDMODE_ADD", "OF_BLENDMODE_SUBTRACT", "OF_BLENDMODE_MULTIPLY", "OF_BLENDMODE_SCREEN" };
	
	void gui_setRGB() {
		c_magentaRed = ofColor(255, 0, 0);
		c_cyanBlue = ofColor(0, 0, 255);
		c_yellowGreen = ofColor(0, 255, 0);
		c_black = ofColor(0, 0, 0);
		bFresh = true;
	}

	void gui_setCMYK() {
		c_magentaRed = ofColor(236, 0, 140);
		c_cyanBlue = ofColor(0, 174, 239);
		c_yellowGreen = ofColor(255, 242, 0);
		c_black = ofColor(0, 0, 0);
		bFresh = true;
	}

	void renderImGuiColourSettings(bool colors, bool mask) {
		if (colors || mask) {
			if (ImGui::CollapsingHeader("Colours ##drawFilter"))
			{
				if (colors) {
					if (ImGui::ColorEdit4("Cyan / Blue ##drawFilter", (float*)&c_cyanBlue, ImGuiColorEditFlags_NoInputs)) {
						bFresh = true;
					}
					if (ImGui::ColorEdit4("Magenta / Red ##drawFilter", (float*)&c_magentaRed, ImGuiColorEditFlags_NoInputs)) {
						bFresh = true;
					}
					if (ImGui::ColorEdit4("Yellow / Green ##drawFilter", (float*)&c_yellowGreen, ImGuiColorEditFlags_NoInputs)) {
						bFresh = true;
					}
					if (ImGui::ColorEdit4("Key / Black ##drawFilter", (float*)&c_black, ImGuiColorEditFlags_NoInputs)) {
						bFresh = true;
					}
				}
				if (mask) {
					if (ImGui::ColorEdit4("Mask ##drawFilter", (float*)&c_mask, ImGuiColorEditFlags_NoInputs)) {
						bFresh = true;
					}
					ImGui::SameLine();
					if (ImGui::Checkbox("Use Mask ##drawFilter", &useMask)) {
						bFresh = true;
					}
					ImGui::SameLine();
					ImGui::PushItemWidth(60);
					if (ImGui::DragInt("Margin ##drawFilter", &maskMargin, 1, 0, 255)) {
						bFresh = true;
					}
					ImGui::PopItemWidth();
				}

				if (colors) {
					if (ImGui::Button("Set RGB ##drawFilter"))
					{
						gui_setRGB();
					}
					ImGui::SameLine();
					if (ImGui::Button("Set CMYK ##drawFilter"))
					{
						gui_setCMYK();
					}
				}
			} // End Colours
		}
	}
	
	void setBlendMode() {
		if (v_BlendModes[currentBlendModeIndex] == "OF_BLENDMODE_ALPHA") {
			ofEnableBlendMode(OF_BLENDMODE_DISABLED);
		}
		else if (v_BlendModes[currentBlendModeIndex] == "OF_BLENDMODE_ADD") {
			ofEnableBlendMode(OF_BLENDMODE_ADD);
		}
		else if (v_BlendModes[currentBlendModeIndex] == "OF_BLENDMODE_SUBTRACT") {
			ofEnableBlendMode(OF_BLENDMODE_SUBTRACT);
		}
		else if (v_BlendModes[currentBlendModeIndex] == "OF_BLENDMODE_MULTIPLY") {
			ofEnableBlendMode(OF_BLENDMODE_MULTIPLY);
		}
		else if (v_BlendModes[currentBlendModeIndex] == "OF_BLENDMODE_SCREEN") {
			ofEnableBlendMode(OF_BLENDMODE_SCREEN);
		}
		else {
			ofEnableBlendMode(OF_BLENDMODE_DISABLED);
		}
	}

	float percent(float percentage, float total) {
		return (percentage / 100) * total;
	}

	float drawScale = 4; // zoomMultiplier
	float lineWidth = 8;
	// Add UI for standard colours ... 
	ImVec4 c_cyanBlue = ofColor(0, 174, 239, 255);
	ImVec4 c_magentaRed = ofColor(236, 0, 140, 255);
	ImVec4 c_yellowGreen = ofColor(255, 242, 0, 255);
	ImVec4 c_black = ofColor(0, 0, 0, 255);
	ImVec4 c_mask = ofColor(255, 255, 255, 255);
};

