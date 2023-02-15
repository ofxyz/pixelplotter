#pragma once
#include "ofxImGui.h"
#include "ofxOpenCv.h"
#include "ofxXmlSettings.h"

class DrawFilter {
public:
	bool active = true;
	std::string name;

	//virtual void draw(ofFbo* input) = 0;
	virtual void draw(ofImage* input) = 0;
	virtual void renderImGuiSettings() = 0;

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
	}

	void gui_setCMYK() {
		c_magentaRed = ofColor(236, 0, 140);
		c_cyanBlue = ofColor(0, 174, 239);
		c_yellowGreen = ofColor(255, 242, 0);
		c_black = ofColor(0, 0, 0);
	}

	void renderImGuiColourSettings(bool doRender) {
		if (doRender) {
			if (ImGui::CollapsingHeader("Colours"))
			{
				ImGui::ColorEdit4("Cyan / Blue", (float*)&c_cyanBlue, ImGuiColorEditFlags_NoInputs);
				ImGui::ColorEdit4("Magenta / Red", (float*)&c_magentaRed, ImGuiColorEditFlags_NoInputs);
				ImGui::ColorEdit4("Yellow / Green", (float*)&c_yellowGreen, ImGuiColorEditFlags_NoInputs);
				ImGui::ColorEdit4("Key / Black", (float*)&c_black, ImGuiColorEditFlags_NoInputs);

				if (ImGui::Button("Set RGB"))
				{
					gui_setRGB();
				}
				ImGui::SameLine();
				if (ImGui::Button("Set CMYK"))
				{
					gui_setCMYK();
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

	float percentage(float percent, float total) {
		return (percent / 100) * total;
	}

	float drawScale = 4; // zoomMultiplier
 
	// Add UI for standard colours ... 
	ImVec4 c_cyanBlue = ofColor(0, 174, 239, 255);
	ImVec4 c_magentaRed = ofColor(236, 0, 140, 255);
	ImVec4 c_yellowGreen = ofColor(255, 242, 0, 255);
	ImVec4 c_black = ofColor(0, 0, 0, 255);

};

class Df_pixelate : public DrawFilter {
public:
	std::string name = "Pixelate";
	void draw(ofImage* input);
	void renderImGuiSettings();

private:
	void drawRectangle(float offsetX, float offsetY, float w, float h, ofColor c);
	void drawEllipse(float offsetX, float offsetY, float w, float h, ofColor c);
	void drawRgbSeperation_Fill(float offsetX, float offsetY, float w, float h, ofColor c);
	void drawRgbSeperation_Center(float offsetX, float offsetY, float w, float h, ofColor c);
	void drawRgbSeperation_Square(float offsetX, float offsetY, float w, float h, ofColor c);
	void drawRgbSeperation_Black(float offsetX, float offsetY, float w, float h, ofColor c);
	void drawCMYKSeperation_Square(float offsetX, float offsetY, float w, float h, ofColor c);
	void drawCMYKSeperation_Left(float offsetX, float offsetY, float w, float h, ofColor c);
	void drawCMYKSeperation_Hills(float offsetX, float offsetY, float w, float h, ofColor c);

	void drawPixel(float w, float h, ofColor c);
	float getRotation(ofColor c, float w, float h);
	float getWidth(ofColor c, float w, float h, float r);
	float getHeight(ofColor c, float w, float h, float r);

	std::vector<std::string> v_pixelType{
			"Rectangle",
			"Ellipse",
			"RGB Seperation Fill",
			"RGB Seperation Center",
			"RGB Seperation Square",
			"CMYK Seperation Square",
			"CMYK Seperation Left",
			"CMYK Seperation Hills"
	};

	std::vector<std::string> v_pixelDataMapOptions{
		"None",
		"Between",
		"Color Lightness"/*,
		"Color Red",
		"Color Green",
		"Color Blue",
		"Location X",
		"Location Y" */
	};

	// ADD PIXEL OFFSET so next filter can be misaligned ...
	// ADD Transparency Slider
	int ui_currentRotationMap = 0;
	int ui_currentWidthMap = 0;
	int ui_currentHeightMap = 0;
	int ui_currentPixelType = 0;
	bool normalize = false;
	bool polka = false;
	int tilesX = 64;
	int tilesY = 64;
	float addonx = 0;
	float addony = 0;
	float addonx_rand = 0;
	float addony_rand = 0;
	float offsetx = 0;
	float offsety = 0;
	float offsetx_rand = 0;
	float offsety_rand = 0;

	ofVec2f rotationMinMax = ofVec2f(0, 0);
	ofVec2f widthMinMax = ofVec2f(5, 50);
	ofVec2f heightMinMax = ofVec2f(5, 50);
};

class Df_rings : public DrawFilter {
public:
	std::string name = "Rings";
	void draw(ofImage* input);
	void renderImGuiSettings();

private:
	int cvThresh = 128;
	int cvBlur = 150;
	int cvSteps = 10;

	ofxCvContourFinder contourFinder;

	ofxCvColorImage colorCvImage;
	ofxCvGrayscaleImage greyCvImage;
	ofxCvGrayscaleImage greyCvBlur;
	ofxCvGrayscaleImage greyCvThresh;
};
