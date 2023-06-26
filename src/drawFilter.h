#pragma once
#include "ofxImGui.h"
#include "ofxXmlSettings.h"

class ofApp;

class DrawFilter {
public:
	bool active = true;
	bool visible = true;
	bool useMask = false;
	bool moveUp = false;
	bool moveDown = false;
	int maskMargin = 0;

	std::string name;

	virtual void draw(ofImage* input, float width = 0, float height = 0, float x = 0, float y = 0) = 0;
	virtual void renderImGuiSettings() = 0;
	virtual void loadSettings(ofxXmlSettings settings) = 0;
	virtual ofxXmlSettings getSettings() = 0;

	void renderUpDownButtons() {
		if (ImGui::Button("Move Up"))
		{
			moveUp = true;
			moveDown = false;
		}
		ImGui::SameLine();
		if (ImGui::Button("Move Down"))
		{
			moveUp = false;
			moveDown = true;
		}
		ImGui::SameLine();
		if (ImGui::Checkbox("Visible", &visible)) {
			setFresh(true);
		}
	}

	// -------------------------------------------------
	// Delete after implementing drawPixel class ...
	ofVec4f getCMYK(ofColor rgb) {
		double dr = 0, dg = 0, db = 0;
		if (rgb.r > 0) {
			dr = (double)rgb.r / 255;
		}
		if (rgb.g > 0) {
			dg = (double)rgb.g / 255;
		}
		if (rgb.b > 0) {
			db = (double)rgb.b / 255;
		}

		double k = 1 - max(max(dr, dg), db);

		double kop = 1 - k;
		
		double c = 1 - dr - k;
		double m = 1 - dg - k;
		double y = 1 - db - k;

		if (kop > 0) {
			if (c > 0) {
				c /= kop;
			}
			if (m > 0) {
				m /= kop;
			}
			if (y > 0) {
				y /= kop;
			}
		}

		return ofVec4f(c, m, y, k);
	}

	int currentBlendModeIndex = 0;
	std::vector<std::string> v_BlendModes{ "OF_BLENDMODE_DISABLED", "OF_BLENDMODE_ALPHA", "OF_BLENDMODE_ADD", "OF_BLENDMODE_SUBTRACT", "OF_BLENDMODE_MULTIPLY", "OF_BLENDMODE_SCREEN" };
	
	// -------------------------------------------------
	// Delete after implementing drawPixel class ...
	void gui_setGrey() {
		c_magentaRed = ofColor(108, 108, 108);
		c_cyanBlue = ofColor(139, 139, 139);
		c_yellowGreen = ofColor(239, 239, 239);
		c_black = ofColor(0, 0, 0);
		setFresh(true);
	}

	void gui_setRGB() {
		c_magentaRed = ofColor(255, 0, 0);
		c_cyanBlue = ofColor(0, 0, 255);
		c_yellowGreen = ofColor(0, 255, 0);
		c_black = ofColor(0, 0, 0);
		setFresh(true);
	}

	void gui_setCMYK() {
		c_magentaRed = ofColor(236, 0, 140);
		c_cyanBlue = ofColor(0, 174, 239);
		c_yellowGreen = ofColor(255, 242, 0);
		c_black = ofColor(0, 0, 0);
		setFresh(true);
	}

	void gui_setRYB() {
		c_magentaRed = ofColor(248, 11, 17);
		c_cyanBlue = ofColor(19, 57, 166);
		c_yellowGreen = ofColor(255, 230, 0);
		c_black = ofColor(0, 0, 0);
		setFresh(true);
	}

	void renderImGuiColourSettings(bool colors, bool mask) {
		if (colors || mask) {
			if (ImGui::CollapsingHeader("Colours ##drawFilter"))
			{
				if (colors) {
					if (ImGui::ColorEdit4("Cyan / Blue ##drawFilter", (float*)&c_cyanBlue, ImGuiColorEditFlags_NoInputs)) {
						setFresh(true);
					}
					if (ImGui::ColorEdit4("Magenta / Red ##drawFilter", (float*)&c_magentaRed, ImGuiColorEditFlags_NoInputs)) {
						setFresh(true);
					}
					if (ImGui::ColorEdit4("Yellow / Green ##drawFilter", (float*)&c_yellowGreen, ImGuiColorEditFlags_NoInputs)) {
						setFresh(true);
					}
					if (ImGui::ColorEdit4("Key / Black ##drawFilter", (float*)&c_black, ImGuiColorEditFlags_NoInputs)) {
						setFresh(true);
					}
				}
				if (mask) {
					if (ImGui::ColorEdit4("Mask ##drawFilter", (float*)&c_mask, ImGuiColorEditFlags_NoInputs)) {
						setFresh(true);
					}
					ImGui::SameLine();
					if (ImGui::Checkbox("Use Mask ##drawFilter", &useMask)) {
						setFresh(true);
					}
					ImGui::SameLine();
					ImGui::PushItemWidth(60);
					if (ImGui::DragInt("Margin ##drawFilter", &maskMargin, 1, 0, 255)) {
						setFresh(true);
					}
					ImGui::PopItemWidth();
				}

				if (colors) {
					if (ImGui::Button("Set Grey ##drawFilter"))
					{
						gui_setGrey();
					}
					ImGui::SameLine();
					if (ImGui::Button("Set RGB ##drawFilter"))
					{
						gui_setRGB();
					}
					ImGui::SameLine();
					if (ImGui::Button("Set CMYK ##drawFilter"))
					{
						gui_setCMYK();
					}
					ImGui::SameLine();
					if (ImGui::Button("Set RYB ##drawFilter"))
					{
						gui_setRYB();
					}
				}
			} // End Colors
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

	float lineWidth = 8;
	
	ImVec4 c_cyanBlue = ofColor(0, 174, 239, 255);
	ImVec4 c_magentaRed = ofColor(236, 0, 140, 255);
	ImVec4 c_yellowGreen = ofColor(255, 242, 0, 255);
	ImVec4 c_black = ofColor(0, 0, 0, 255);
	ImVec4 c_mask = ofColor(255, 255, 255, 255);

	bool isFresh() {
		return bFresh;
	}
	void setFresh(bool fresh) {
		bFresh = fresh;
	}

private:
	bool bFresh = false;
};

