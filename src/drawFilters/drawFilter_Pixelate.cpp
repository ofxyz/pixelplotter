#include "drawFilter_Pixelate.h"
#include "ofx2d.h"
#include "ofApp.h"
#include "ofxImGui.h"
#include "imgui_internal.h"
#include "ImHelpers.h"

Df_pixelate::Df_pixelate()
{
	InitDefaults();
}

Df_pixelate::Df_pixelate(ofJson& settings)
	: Df_pixelate()
{
	loadSettings(settings);
}

void Df_pixelate::InitDefaults()
{
	pixelplotter = (ofApp*)ofGetAppPtr();
	name = "Pixelate";
	v_pixelType = {
			"Rectangle",
			"Ellipse",
			"RGB Seperation Fill",
			"RGB Seperation Center",
			"RGB Seperation Square",
			"CMYK Seperation Fill",
			"CMYK Seperation Square",
			"CMYK Seperation Left",
			"CMYK Seperation Hills",
			"CMYK Seperation Bars",
			"Color Adjust",
			"Unusual Overprint",
			"Star Hash"
	};
	v_pixelDataMapOptions = {
		"None",
		"Between",
		"Color Lightness",
		"Sound"/*,
		"Color Red",
		"Color Green",
		"Color Blue",
		"Location X",
		"Location Y" */
	};
	v_ignoreOptions = {
		"None",
		"Random",
		"Modulo",
		"Plaid",
		"Scan"
	};
	rotationMinMax = glm::vec2(0, 0);
	widthMinMax = glm::vec2(5, 50);
	heightMinMax = glm::vec2(5, 50);
}

void Df_pixelate::loadSettings(ofJson& settings) {
	try {
		name = settings.value("name", "pixelate");
		bVisible = settings.value("bVisible", true);
		useMask = settings.value("useMask", false);
		maskMargin = settings.value("maskMargin", maskMargin);
		ui_currentPixelType = ofx2d::getIndex(v_pixelType, settings.value("pixelType", "Undefined"), 1);
		tilesX = settings.value("tilesX", 64);
		tilesY = settings.value("tilesY", 64);
		polka = settings.value("polka", false);
		usePalette = settings.value("usePalette", usePalette);
		addonx = settings.value("addonx", 0);
		addony = settings.value("addony", 0);
		addonx_rand = settings.value("addonx_rand", 0);
		addony_rand = settings.value("addony_rand", 0);
		offsetx = settings.value("offsetx", 0);
		offsety = settings.value("offsety", 0);
		offsetx_rand = settings.value("offsetx_rand", 0);
		offsety_rand = settings.value("offsety_rand", 0);

		ui_currentIgnore = ofx2d::getIndex(v_ignoreOptions, settings.value("currentIgnore", "None"), 0);
		ignorePercent = settings.value("ignorePercent", ignorePercent);
		ignoreModulo = settings.value("ignoreModulo", ignoreModulo);
		ignorePlaid = settings.value("ignorePlaid", ignorePlaid);
		ignoreScan = settings.value("ignoreScan", ignoreScan);
		ignoreSeed = settings.value("ignoreSeed", ignoreSeed);
		ignoreSeedAddon = settings.value("ignoreSeedAddon", ignoreSeedAddon);

		cLerp = settings.value("cLerp", cLerp);
		rounded = settings.value("rounded", rounded);

		ui_currentRotationMap = ofx2d::getIndex(v_pixelDataMapOptions, settings.value("rotationMap", "None"), 0);
		rotationMinMax[0] = settings.value("rotationMin", 0);
		rotationMinMax[1] = settings.value("rotationMax", 0);

		ui_currentWidthMap = ofx2d::getIndex(v_pixelDataMapOptions, settings.value("widthMap", "None"), 0);
		widthMinMax[0] = settings.value("widthMin", 0);
		widthMinMax[1] = settings.value("widthMax", 0);

		ui_currentHeightMap = ofx2d::getIndex(v_pixelDataMapOptions, settings.value("heightMap", "None"), 0);
		heightMinMax[0] = settings.value("heightMin", 0);
		heightMinMax[1] = settings.value("heightMax", 0);

		currentBlendModeIndex = settings.value("blendMode", 0);

		// Colours
		c_black.x = settings["colors"]["black"].value("r", c_black.x);
		c_black.y = settings["colors"]["black"].value("g", c_black.y);
		c_black.z = settings["colors"]["black"].value("b", c_black.z);
		c_black.w = settings["colors"]["black"].value("a", c_black.w);

		c_yellowGreen.x = settings["colors"]["yellowGreen"].value("r", c_yellowGreen.x);
		c_yellowGreen.y = settings["colors"]["yellowGreen"].value("g", c_yellowGreen.y);
		c_yellowGreen.z = settings["colors"]["yellowGreen"].value("b", c_yellowGreen.z);
		c_yellowGreen.w = settings["colors"]["yellowGreen"].value("a", c_yellowGreen.w);

		c_magentaRed.x = settings["colors"]["magentaRed"].value("r", c_magentaRed.x);
		c_magentaRed.y = settings["colors"]["magentaRed"].value("g", c_magentaRed.y);
		c_magentaRed.z = settings["colors"]["magentaRed"].value("b", c_magentaRed.z);
		c_magentaRed.w = settings["colors"]["magentaRed"].value("a", c_magentaRed.w);

		c_cyanBlue.x = settings["colors"]["cyanBlue"].value("r", c_cyanBlue.x);
		c_cyanBlue.y = settings["colors"]["cyanBlue"].value("g", c_cyanBlue.y);
		c_cyanBlue.z = settings["colors"]["cyanBlue"].value("b", c_cyanBlue.z);
		c_cyanBlue.w = settings["colors"]["cyanBlue"].value("a", c_cyanBlue.w);

		c_mask.x = settings["colors"]["mask"].value("r", c_mask.x);
		c_mask.y = settings["colors"]["mask"].value("g", c_mask.y);
		c_mask.z = settings["colors"]["mask"].value("b", c_mask.z);
		c_mask.w = settings["colors"]["mask"].value("a", c_mask.w);
	}
	catch (...) {
		ofLogNotice() << "Failed to load settings Df_pixelate";
	}
}

ofJson Df_pixelate::getSettings() {
	ofJson settings;
	settings["name"] = name;
	settings["bVisible"] = bVisible;
	settings["useMask"] = useMask;
	settings["maskMargin"] = maskMargin;
	settings["pixelType"] = v_pixelType[ui_currentPixelType];
	settings["tilesX"] = tilesX;
	settings["tilesY"] = tilesY;
	settings["polka"] = polka;
	settings["usePalette"] = usePalette;
	settings["addonx"] = addonx;
	settings["addony"] = addony;
	settings["addonx_rand"] = addonx_rand;
	settings["addony_rand"] = addony_rand;
	settings["offsetx"] = offsetx;
	settings["offsety"] = offsety;
	settings["offsetx_rand"] = offsetx_rand;
	settings["offsety_rand"] = offsety_rand;

	settings["currentIgnore"] = v_ignoreOptions[ui_currentIgnore];
	settings["ignorePercent"] = ignorePercent;
	settings["ignoreModulo"] = ignoreModulo;
	settings["ignorePlaid"] = ignorePlaid;
	settings["ignoreScan"] = ignoreScan;
	settings["ignoreSeed"] = ignoreSeed;
	settings["ignoreSeedAddon"] = ignoreSeedAddon;

	settings["cLerp"] = cLerp;
	settings["rounded"] = rounded;

	settings["rotationMap"] = v_pixelDataMapOptions[ui_currentRotationMap];
	settings["rotationMin"] = rotationMinMax[0];
	settings["rotationMax"] = rotationMinMax[1];

	settings["widthMap"] = v_pixelDataMapOptions[ui_currentWidthMap];
	settings["widthMin"] = widthMinMax[0];
	settings["widthMax"] = widthMinMax[1];

	settings["heightMap"] = v_pixelDataMapOptions[ui_currentHeightMap];
	settings["heightMin"] = heightMinMax[0];
	settings["heightMax"] = heightMinMax[1];

	settings["blendMode"] = currentBlendModeIndex;

	settings["colors"]["black"]["r"] = c_black.x;
	settings["colors"]["black"]["g"] = c_black.y;
	settings["colors"]["black"]["b"] = c_black.z;
	settings["colors"]["black"]["a"] = c_black.w;

	settings["colors"]["yellowGreen"]["r"] = c_yellowGreen.x;
	settings["colors"]["yellowGreen"]["g"] = c_yellowGreen.y;
	settings["colors"]["yellowGreen"]["b"] = c_yellowGreen.z;
	settings["colors"]["yellowGreen"]["a"] = c_yellowGreen.w;

	settings["colors"]["magentaRed"]["r"] = c_magentaRed.x;
	settings["colors"]["magentaRed"]["g"] = c_magentaRed.y;
	settings["colors"]["magentaRed"]["b"] = c_magentaRed.z;
	settings["colors"]["magentaRed"]["a"] = c_magentaRed.w;

	settings["colors"]["cyanBlue"]["r"] = c_cyanBlue.x;
	settings["colors"]["cyanBlue"]["g"] = c_cyanBlue.y;
	settings["colors"]["cyanBlue"]["b"] = c_cyanBlue.z;
	settings["colors"]["cyanBlue"]["a"] = c_cyanBlue.w;

	settings["colors"]["mask"]["r"] = c_mask.x;
	settings["colors"]["mask"]["g"] = c_mask.y;
	settings["colors"]["mask"]["b"] = c_mask.z;
	settings["colors"]["mask"]["a"] = c_mask.w;

	return settings;
}

void Df_pixelate::renderImGuiSettings() {
	if (ImGui::CollapsingHeader(name.c_str(), &bAlive)) {
		ImGui::PushID("Df_pixelate");
		ImGui::AlignTextToFramePadding();

		renderUpDownButtons();

		if (ofxImGui::VectorCombo("Pixel Type ##pixelate", &ui_currentPixelType, v_pixelType)) {
			//if (ui_currentPixelType > 1) {
			//	gui_setRGB();
			//}
			//if (ui_currentPixelType > 5) {
			//	gui_setCMYK();
			//}
			setFresh(true);
		};

		ImGui::PushItemWidth(60);

		if (ImGui::DragFloat("Round ##pixelate_addon", &rounded, 0.1f, -100, 100, "%.3f")) {
			setFresh(true);
		}

		ImGui::Text("Tiles"); ImGui::SameLine(75);
		if (ImGui::DragInt("X ##pixelate_tiles", &tilesX, 1, 1, 1200)) {
			setFresh(true);
		}
		ImGui::SameLine();
		if (ImGui::DragInt("Y ##pixelate_tiles", &tilesY, 1, 1, 1200)) {
			setFresh(true);
		}
		ImGui::SameLine();
		if (ImGui::Checkbox("Polka", &polka)) {
			setFresh(true);
		}

		ImGui::Text("+ Addon"); ImGui::SameLine(75);
		if (ImGui::DragFloat("X ##pixelate_addon", &addonx, 0.1f, -100.0f, 100.0f, "%.3f")) {
			setFresh(true);
		}
		ImGui::SameLine();
		if (ImGui::DragFloat("Y ##pixelate_addon", &addony, 0.1f, -100.0f, 100.0f, "%.3f")) {
			setFresh(true);
		}

		ImGui::Text("+ Random"); ImGui::SameLine(75);
		if (ImGui::DragFloat("X ##pixelate_addon_rand", &addonx_rand, 0.1f, -100.0f, 100.0f, "%.3f")) {
			setFresh(true);
		}
		ImGui::SameLine();
		if (ImGui::DragFloat("Y ##pixelate_addon_rand", &addony_rand, 0.1f, -100.0f, 100.0f, "%.3f")) {
			setFresh(true);
		}

		ImGui::Text("+ Skip"); ImGui::SameLine(75);
		if (ofxImGui::VectorCombo("##pixelate_ignore", &ui_currentIgnore, v_ignoreOptions)) {
			setFresh(true);
		}
		if (v_ignoreOptions[ui_currentIgnore] == "Random") {
			ImGui::SameLine();
			if (ImGui::DragInt("% ##pixelate_percent", &ignorePercent, 1, 0, 100)) {
				setFresh(true);
			}
		}
		else if (v_ignoreOptions[ui_currentIgnore] == "Modulo") {
			ImGui::SameLine();
			if (ImGui::DragInt("Mod ##pixelate_modolo", &ignoreModulo, 1, 1, 500)) {
				if (ignoreModulo == 0) ignoreModulo = 1;
				setFresh(true);
			}
		}
		else if (v_ignoreOptions[ui_currentIgnore] == "Plaid") {
			ImGui::SameLine();
			if (ImGui::DragInt("% ##pixelate_plaid", &ignorePlaid, 1, 0, 100)) {
				setFresh(true);
			}
		}
		else if (v_ignoreOptions[ui_currentIgnore] == "Scan") {
			ImGui::SameLine();
			if (ImGui::DragInt("% ##pixelate_scan", &ignoreScan, 1, 0, 100)) {
				setFresh(true);
			}
			ImGui::Text("  + Seed"); ImGui::SameLine(75);
			if (ImGui::DragInt("Start ##pixelate_seed", &ignoreSeed, 1, 0, 500)) {
				setFresh(true);
			}
			ImGui::SameLine();
			if (ImGui::DragInt("Step ##pixelate_seedAddon", &ignoreSeedAddon, 1, 1, 250)) {
				setFresh(true);
			}
		}

		ImGui::Separator();

		ImGui::Text("Offset"); ImGui::SameLine(75);
		if (ImGui::DragFloat("X ##pixelate_offsetx", &offsetx, 0.1f, -250.0f, 250.0f, "%.3f")) {
			setFresh(true);
		}
		ImGui::SameLine();
		if (ImGui::DragFloat("Y ##pixelate_offsety", &offsety, 0.1f, -250.0f, 250.0f, "%.3f")) {
			setFresh(true);
		}

		ImGui::Text("+ Random"); ImGui::SameLine(75);
		if (ImGui::DragFloat("X ##pixelate_offsetX_random", &offsetx_rand, 0.1f, 0.0f, 500.0f, "%.3f")) {
			setFresh(true);
		}
		ImGui::SameLine();
		if (ImGui::DragFloat("Y ##pixelate_offsetY_random", &offsety_rand, 0.1f, -100.0f, 100.0f, "%.3f")) {
			setFresh(true);
		}

		ImGui::Separator();

		ImGui::Text("Rotation"); ImGui::SameLine(75);
		if (ofxImGui::VectorCombo("##pixelate_rotation", &ui_currentRotationMap, v_pixelDataMapOptions)) {
			setFresh(true);
		}
		if (ui_currentRotationMap > 0) {
			ImGui::Text(" "); ImGui::SameLine(75);
			if (ImGui::DragFloat("Min ##pixelate_rotation", &rotationMinMax[0], 0.1f, -360.0f, 360.0f, "%.3f")) {
				setFresh(true);
			}
			ImGui::SameLine();
			if (ImGui::DragFloat("Max ##pixelate_rotation", &rotationMinMax[1], 0.1f, -360.0f, 360.0f, "%.3f")) {
				setFresh(true);
			}
		}

		ImGui::Separator();

		ImGui::Text("Width"); ImGui::SameLine(75);
		if (ofxImGui::VectorCombo("##pixelate_width", &ui_currentWidthMap, v_pixelDataMapOptions)) {
			setFresh(true);
		}
		if (ui_currentWidthMap > 0) {
			ImGui::Text(" "); ImGui::SameLine(75);
			if (ImGui::DragFloat("Min ##pixelate_width", &widthMinMax[0], 0.1f, -250.0f, 250.0f, "%.3f")) {
				setFresh(true);
			}
			ImGui::SameLine();
			if (ImGui::DragFloat("Max ##pixelate_width", &widthMinMax[1], 0.1f, -250.0f, 250.0f, "%.3f")) {
				setFresh(true);
			}
		}

		ImGui::Text("Height"); ImGui::SameLine(75);
		if (ofxImGui::VectorCombo("##pixelate_height", &ui_currentHeightMap, v_pixelDataMapOptions)) {
			setFresh(true);
		}
		if (ui_currentHeightMap > 0) {
			ImGui::Text(" "); ImGui::SameLine(75);
			if (ImGui::DragFloat("Min ##pixelate_height", &heightMinMax[0], 0.1f, -250.0f, 250.0f, "%.3f")) {
				setFresh(true);
			}
			ImGui::SameLine();
			if (ImGui::DragFloat("Max ##pixelate_height", &heightMinMax[1], 0.1f, -250.0f, 250.0f, "%.3f")) {
				setFresh(true);
			}
		}

		if (ui_currentPixelType == 9) {

			ImGui::Separator();

			if (ImGui::DragFloat("Color Lerp ##pixelate_clerp", &cLerp, 0.01f, 0.0, 1.0, "%.3f")) {
				setFresh(true);
			}
			if (ImGui::Checkbox("Use palette", &usePalette)) {
				setFresh(true);
			}

			ImGui::Separator();
		}

		ImGui::PopItemWidth();

		if (ui_currentPixelType > 1) {
			renderImGuiColourSettings(true, true);
		}
		else {
			renderImGuiColourSettings(false, true);
		}

		ImGui::PushItemWidth(200);
		if (ofxImGui::VectorCombo("##Blend Mode", &currentBlendModeIndex, v_BlendModes)) {
			setFresh(true);
		}
		ImGui::PopItemWidth();

		ImGui::PopID();
	}
}

void Df_pixelate::drawRectangle(float offsetX, float offsetY, float w, float h, ofColor c) {
	ofPushStyle();
	ofFill();
	ofSetColor(c);
	ofDrawRectangle(-(w * 0.5) + offsetX, -(h * 0.5) + offsetY, w, h);
	ofPopStyle();
};

void Df_pixelate::drawEllipse(float offsetX, float offsetY, float w, float h, ofColor c) {
	ofPushStyle();
	ofFill();
	ofSetColor(c);
	//ofSetCircleResolution(4); Does not work for ciaro renderer
	ofDrawEllipse(offsetX, offsetY, w, h);
	ofPopStyle();
};

void Df_pixelate::quadraticBezierVertex(float cpx, float cpy, float x, float y, float prevX, float prevY) {
	float cp1x = prevX + 2.0 / 3.0 * (cpx - prevX);
	float cp1y = prevY + 2.0 / 3.0 * (cpy - prevY);
	float cp2x = cp1x + (x - prevX) / 3.0;
	float cp2y = cp1y + (y - prevY) / 3.0;

	// finally call cubic Bezier curve function  
	ofBezierVertex(cp1x, cp1y, cp2x, cp2y, x, y);
};

void Df_pixelate::drawRoundedRect(float offsetX, float offsetY, float w, float h, ofColor c) {
	ofPushStyle();
	ofFill();
	ofSetColor(c);

	float x = -(w * 0.5) + offsetX;
	float y = -(h * 0.5) + offsetY;

	ofBeginShape();
	ofVertex(x + rounded, y);
	ofVertex(x + w - rounded, y);
	quadraticBezierVertex(x + w, y, x + w, y + rounded, x + w - rounded, y);
	ofVertex(x + w, y + h - rounded);
	quadraticBezierVertex(x + w, y + h, x + w - rounded, y + h, x + w, y + h - rounded);
	ofVertex(x + rounded, y + h);
	quadraticBezierVertex(x, y + h, x, y + h - rounded, x + rounded, y + h);
	ofVertex(x, y + rounded);
	quadraticBezierVertex(x, y, x + rounded, y, x, y + rounded);
	ofEndShape();

	ofPopStyle();
};

void Df_pixelate::drawRgbSeperation_Fill(float offsetX, float offsetY, float w, float h, ofColor c) {
	float cWidth;
	float maxC = 765; // 255 * 3;

	float left = maxC - (c.r + c.g + c.b);
	float addon = ceil(left / 3);

	ofPushMatrix(); // offset

	cWidth = ofMap(c.r + addon, 0, maxC, 0, w);
	ofTranslate(-(w * 0.5) + (cWidth * 0.5), 0, 0);
	drawRoundedRect(offsetX, offsetY, cWidth, h, c_magentaRed);

	ofTranslate(cWidth * 0.5, 0, 0);
	cWidth = ofMap(c.g + addon, 0, maxC, 0, w);
	ofTranslate(cWidth * 0.5, 0, 0);
	drawRoundedRect(offsetX, offsetY, cWidth, h, c_yellowGreen);

	ofTranslate(cWidth * 0.5, 0, 0);
	cWidth = ofMap(c.b + addon, 0, maxC, 0, w);
	ofTranslate(cWidth * 0.5, 0, 0);
	drawRoundedRect(offsetX, offsetY, cWidth, h, ofColor(0, 0, 255));

	ofPopMatrix();
};

void Df_pixelate::drawRgbSeperation_Center(float offsetX, float offsetY, float w, float h, ofColor c) {
	float maxWidth = w / 3;
	float cWidth;

	cWidth = ofMap(c.r, 0, 255, 0, maxWidth);
	drawRoundedRect(offsetX, offsetY, cWidth, h, c_magentaRed); // Red

	ofPushMatrix(); // offset

	ofTranslate(maxWidth, 0, 0);
	cWidth = ofMap(c.g, 0, 255, 0, maxWidth);
	drawRoundedRect(offsetX, offsetY, cWidth, h, c_yellowGreen); // Green

	ofTranslate(maxWidth, 0, 0);
	cWidth = ofMap(c.b, 0, 255, 0, maxWidth);
	drawRoundedRect(offsetX, offsetY, cWidth, h, c_cyanBlue); // Blue

	ofPopMatrix();
};

void Df_pixelate::drawRgbSeperation_Square(float offsetX, float offsetY, float w, float h, ofColor c) {
	float cWidth, cHeight;

	cWidth = ofMap(c.r, 0, 255, 0, w);
	cHeight = ofMap(c.r, 0, 255, 0, h);
	drawRoundedRect(offsetX, offsetY, cWidth, cHeight, c_magentaRed);

	cWidth = ofMap(c.g, 0, 255, 0, w);
	cHeight = ofMap(c.g, 0, 255, 0, h);
	drawRoundedRect(offsetX, offsetY, cWidth, cHeight, c_yellowGreen);

	cWidth = ofMap(c.b, 0, 255, 0, w);
	cHeight = ofMap(c.b, 0, 255, 0, h);
	drawRoundedRect(offsetX, offsetY, cWidth, cHeight, c_cyanBlue);
};

void Df_pixelate::drawCMYKSeperation_Fill(float offsetX, float offsetY, float w, float h, ofColor c) {
	float cWidth;

	glm::vec4 cmyk = ofx2d::getCMYK(c);

	float totalInk = cmyk[0] + cmyk[1] + cmyk[2] + cmyk[3];
	if (totalInk < 0.25) return; // Show some white :)

	ofPushMatrix();
	ofTranslate(-(w * 0.5), 0, 0);

	if (cmyk[0] > 0) {
		cWidth = ofMap(cmyk[0], 0, totalInk, 0, w);
		ofTranslate(cWidth * 0.5, 0, 0);
		drawRoundedRect(offsetX, offsetY, cWidth, h, c_cyanBlue); // Cyan
		ofTranslate(cWidth * 0.5, 0, 0);
	}

	if (cmyk[1] > 0) {
		cWidth = ofMap(cmyk[1], 0, totalInk, 0, w);
		ofTranslate(cWidth * 0.5, 0, 0);
		drawRoundedRect(offsetX, offsetY, cWidth, h, c_magentaRed); // Magenta
		ofTranslate(cWidth * 0.5, 0, 0);
	}

	if (cmyk[2] > 0) {
		cWidth = ofMap(cmyk[2], 0, totalInk, 0, w);
		ofTranslate(cWidth * 0.5, 0, 0);
		drawRoundedRect(offsetX, offsetY, cWidth, h, c_yellowGreen); // Yellow
		ofTranslate(cWidth * 0.5, 0, 0);
	}

	if (cmyk[3] > 0) {
		cWidth = ofMap(cmyk[3], 0, totalInk, 0, w);
		ofTranslate(cWidth * 0.5, 0, 0);
		drawRoundedRect(offsetX, offsetY, cWidth, h, c_black); // Black
	}

	ofPopMatrix();
};

void Df_pixelate::drawCMYKSeperation_Square(float offsetX, float offsetY, float w, float h, ofColor c) {
	float cWidth, cHeight;
	glm::vec4 cmyk = ofx2d::getCMYK(c);

	cWidth = ofMap(cmyk[2], 0, 1, 0, w);
	cHeight = ofMap(cmyk[2], 0, 1, 0, h);
	drawRoundedRect(offsetX, offsetY, cWidth, cHeight, c_yellowGreen); // Yellow

	cWidth = ofMap(cmyk[1], 0, 1, 0, w);
	cHeight = ofMap(cmyk[1], 0, 1, 0, h);
	drawRoundedRect(offsetX, offsetY, cWidth, cHeight, c_magentaRed); // Magenta

	cWidth = ofMap(cmyk[0], 0, 1, 0, w);
	cHeight = ofMap(cmyk[0], 0, 1, 0, h);
	drawRoundedRect(offsetX, offsetY, cWidth, cHeight, c_cyanBlue); // Cyan

	cWidth = ofMap(cmyk[3], 0, 1, 0, w);
	cHeight = ofMap(cmyk[3], 0, 1, 0, h);
	drawRoundedRect(offsetX, offsetY, cWidth, cHeight, c_black); // Black

};

void Df_pixelate::drawCMYKSeperation_Left(float offsetX, float offsetY, float w, float h, ofColor c) {
	float cWidth;
	glm::vec4 cmyk = ofx2d::getCMYK(c);

	ofPushMatrix();

	cWidth = ofMap(cmyk[3], 0, 1, 0, w / 2);
	ofTranslate(-(w * 0.5) + (cWidth * 0.5), 0, 0);
	drawRoundedRect(offsetX, offsetY, cWidth, h, c_black); // Black
	ofTranslate(cWidth * 0.5, 0, 0);

	cWidth = ofMap(cmyk[0], 0, 1, 0, w / 2);
	ofTranslate(cWidth * 0.5, 0, 0);
	drawRoundedRect(offsetX, offsetY, cWidth, h, c_cyanBlue); // Cyan
	ofTranslate(cWidth * 0.5, 0, 0);

	cWidth = ofMap(cmyk[1], 0, 1, 0, w / 2);
	ofTranslate(cWidth * 0.5, 0, 0);
	drawRoundedRect(offsetX, offsetY, cWidth, h, c_magentaRed); // Magenta
	ofTranslate(cWidth * 0.5, 0, 0);

	cWidth = ofMap(cmyk[2], 0, 1, 0, w / 2);
	ofTranslate(cWidth * 0.5, 0, 0);
	drawRoundedRect(offsetX, offsetY, cWidth, h, c_yellowGreen); // Yellow

	ofPopMatrix();
};

void Df_pixelate::drawCMYKSeperation_Hills(float offsetX, float offsetY, float w, float h, ofColor c) {
	float cHeight;
	glm::vec4 cmyk = ofx2d::getCMYK(c);

	float total = cmyk[0] + cmyk[1] + cmyk[2];
	if (total == 0) return;

	ofPushMatrix();

	if (cmyk[0] > 0) {
		float pc = (100 * cmyk[0]) / total;
		cHeight = (pc / 100) * h;
		ofTranslate(0, (-h * 0.5) + (cHeight * 0.5), 0);
		drawRoundedRect(offsetX, offsetY, w, cHeight, c_cyanBlue); // Cyan
		ofTranslate(0, cHeight * 0.5, 0);
	}

	if (cmyk[1] > 0) {
		float pm = (100 * cmyk[1]) / total;
		cHeight = (pm / 100) * h;
		ofTranslate(0, cHeight * 0.5, 0);
		drawRoundedRect(offsetX, offsetY, w, cHeight, c_magentaRed); // Magenta
		ofTranslate(0, cHeight * 0.5, 0);
	}

	if (cmyk[2] > 0) {
		float py = (100 * cmyk[2]) / total;
		cHeight = (py / 100) * h;
		ofTranslate(0, cHeight * 0.5, 0);
		drawRoundedRect(offsetX, offsetY, w, cHeight, c_yellowGreen); // Yellow
	}

	ofPopMatrix();

	if (cmyk[3] > 0) {
		cHeight = ofMap(cmyk[3], 0, 1, 0, h);
		drawRoundedRect(offsetX, offsetY, w, cHeight, c_black); // c_black
	}
};

void Df_pixelate::drawCMYKSeperation_Bars(float offsetX, float offsetY, float w, float h, ofColor c) {
	ofPushMatrix();
	if (ofRandom(100) > 50) ofRotateDeg(90);

	glm::vec4 cmyk = ofx2d::getCMYK(c);

	float c_w = ofMap(cmyk[0], 0, 1, 0, w * 0.75);
	float m_w = ofMap(cmyk[1], 0, 1, 0, w * 0.75);
	float y_w = ofMap(cmyk[2], 0, 1, 0, w * 0.75);
	float k_w = ofMap(cmyk[3], 0, 1, 0, w * 0.75);
	float w_w = w - c_w - m_w - y_w - k_w;
	if (w_w < 0) {
		w_w = 0;
	}

	float h_w = w * 0.5;
	offsetX -= (h_w - (w_w * 0.5));

	bool run = true;
	float runX = offsetX;

	float minSize = w / 8;
	float maxSize = w / 6;


	while (run) {
		float size = ofRandom(minSize, maxSize);
		if (c_w > size) {
			drawRoundedRect(runX, offsetY, size, h, c_cyanBlue);
			c_w -= size;
			runX += size;
		}
		else if (c_w != 0) {
			drawRoundedRect(runX, offsetY, c_w, h, c_cyanBlue);
			runX += c_w;
			c_w = 0;
		}

		size = ofRandom(minSize, maxSize);
		if (m_w > size) {
			drawRoundedRect(runX, offsetY, size, h, c_magentaRed);
			m_w -= size;
			runX += size;
		}
		else if (m_w != 0) {
			drawRoundedRect(runX, offsetY, m_w, h, c_magentaRed);
			runX += m_w;
			m_w = 0;
		}

		size = ofRandom(minSize, maxSize);
		if (y_w > size) {
			drawRoundedRect(runX, offsetY, size, h, c_yellowGreen);
			y_w -= size;
			runX += size;
		}
		else if (y_w != 0) {
			drawRoundedRect(runX, offsetY, y_w, h, c_yellowGreen);
			runX += y_w;
			y_w = 0;
		}

		size = ofRandom(minSize, maxSize);
		if (k_w > size) {
			drawRoundedRect(runX, offsetY, size, h, c_black);
			k_w -= size;
			runX += size;
		}
		else if (k_w != 0) {
			drawRoundedRect(runX, offsetY, k_w, h, c_black);
			runX += k_w;
			k_w = 0;
		}

		size = ofRandom(minSize, maxSize);
		if (w_w > size) {
			w_w -= size;
			runX += size;
		}
		else if (w_w != 0) {
			runX += w_w;
			w_w = 0;
		}

		if ((c_w + m_w + y_w + k_w) == 0) {
			run = false;
		}
	}
	ofPopMatrix();
}

void Df_pixelate::drawColorAdjust(float offsetX, float offsetY, float w, float h, ofColor c) {
	ofColor c_Rand = ofColor(ofRandom(255), ofRandom(255), ofRandom(255));
	if (usePalette) {
		int x = ofRandom(3);
		switch (x) {
		case 0:
			c_Rand = c_cyanBlue;
			break;
		case 1:
			c_Rand = c_magentaRed;
			break;
		case 2:
			c_Rand = c_yellowGreen;
			break;
		}
	}
	ofColor c_Lerped = c.getLerped(c_Rand, cLerp);
	drawRoundedRect(offsetX, offsetY, w, h, c_Lerped);
}

void Df_pixelate::drawUnusualOverprint(float offsetX, float offsetY, float w, float h, ofColor c) {
	float pWidth = w / 3;
	float pHeight = h / 3;

	offsetX -= pWidth;
	offsetY -= pHeight;

	float div = ofRandom(100) / 100;
	float idiv = 1 - div;

	float r1 = c.r * div;
	float r2 = c.r * idiv;

	div = ofRandom(100) / 100;
	idiv = 1 - div;

	float g1 = c.g * div;
	float g2 = c.g * idiv;

	div = ofRandom(100) / 100;
	idiv = 1 - div;
	float b1 = c.b * div;
	float b2 = c.b * idiv;

	ofColor c1(r1, g1, b1);
	ofColor c2(r2, g2, b2);

	ofColor c3(r1, g1, b1);
	c3.lerp(c2, 0.5);

	c1.setBrightness(c.getBrightness());
	c2.setBrightness(c.getBrightness());
	c3.setBrightness(c.getBrightness());

	c1.setSaturation(c.getSaturation());
	c2.setSaturation(c.getSaturation());
	c3.setSaturation(c.getSaturation());

	// Center
	drawRoundedRect(offsetX + pWidth, offsetY + pHeight, pWidth, pHeight, c);

	// Vertical 1
	drawRoundedRect(offsetX + pWidth, offsetY, pWidth, pHeight, c1);
	drawRoundedRect(offsetX + pWidth, offsetY + (pHeight * 2), pWidth, pHeight, c1);

	// Horizontal 2
	drawRoundedRect(offsetX, offsetY + pHeight, pWidth, pHeight, c2);
	drawRoundedRect(offsetX + (pWidth * 2), offsetY + pHeight, pWidth, pHeight, c2);

	// Corners
	drawRoundedRect(offsetX, offsetY, pWidth, pHeight, c3);
	drawRoundedRect(offsetX + (pWidth * 2), offsetY, pWidth, pHeight, c3);
	drawRoundedRect(offsetX, offsetY + (pHeight * 2), pWidth, pHeight, c3);
	drawRoundedRect(offsetX + (pWidth * 2), offsetY + (pHeight * 2), pWidth, pHeight, c3);

}

void Df_pixelate::drawStarHash(float offsetX, float offsetY, float w, float h, ofColor c) {
	float brightness = 255-c.getLightness();
	float bPercent = (brightness / 255) * 100;
	int maxLines = 8;
	int lineCount = ceil((bPercent / 100)* maxLines);

	if (lineCount == 0) return;
	float strokeWidth = 1;
	float lineLen = ofx2d::findDiagonal(w,h);
	float angle = 360 / lineCount;

	ofPushMatrix();
	ofPushStyle();
	ofNoFill();
	ofSetLineWidth(strokeWidth);
	//ofSetColor(c);
	ofSetColor(c_black);

	// Width should be length of diagonal

	for (int i = 0; i < lineCount; i++) {
		ofDrawLine(-(lineLen * 0.5) + offsetX, -(strokeWidth * 0.5) + offsetY, (lineLen * 0.5) + offsetX, -(strokeWidth * 0.5) + offsetY);
		ofRotateDeg(angle);
		//ofDrawRectangle(-(w * 0.5) + offsetX, -(h * 0.5) + offsetY, w, 1);
	}

	ofPopStyle();
	ofPopMatrix();

}

void Df_pixelate::drawPixel(float w, float h, ofColor c) {

	float offsetX = offsetx + ofRandom(0, offsetx_rand);
	float offsetY = offsety + ofRandom(0, offsetx_rand);

	switch (ui_currentPixelType) {
	case 0:
		drawRoundedRect(offsetX, offsetY, w, h, c);
		break;
	case 1:
		drawEllipse(offsetX, offsetY, w, h, c);
		break;
	case 2:
		drawRgbSeperation_Fill(offsetX, offsetY, w, h, c);
		break;
	case 3:
		drawRgbSeperation_Center(offsetX, offsetY, w, h, c);
		break;
	case 4:
		drawRgbSeperation_Square(offsetX, offsetY, w, h, c);
		break;
	case 5:
		drawCMYKSeperation_Fill(offsetX, offsetY, w, h, c);
		break;
	case 6:
		drawCMYKSeperation_Square(offsetX, offsetY, w, h, c);
		break;
	case 7:
		drawCMYKSeperation_Left(offsetX, offsetY, w, h, c);
		break;
	case 8:
		drawCMYKSeperation_Hills(offsetX, offsetY, w, h, c);
		break;
	case 9:
		drawCMYKSeperation_Bars(offsetX, offsetY, w, h, c);
		break;
	case 10:
		drawColorAdjust(offsetX, offsetY, w, h, c);
		break;
	case 11:
		drawUnusualOverprint(offsetX, offsetY, w, h, c);
		break;
	case 12:
		drawStarHash(offsetX, offsetY, w, h, c);
		break;
	default:
		ofLog() << "Not a valid draw style: " << ui_currentPixelType << endl;
	}
};

float Df_pixelate::getRotation(ofColor c, float x, float y) {
	if (v_pixelDataMapOptions[ui_currentRotationMap] == "Between") {
		return ofRandom(rotationMinMax[0], rotationMinMax[1]);
	}
	if (v_pixelDataMapOptions[ui_currentRotationMap] == "Color Lightness") {
		return ofMap(c.getLightness(), 0, 255, rotationMinMax[0], rotationMinMax[1]);
	}
	return 0;
}

float Df_pixelate::getWidth(ofColor c, float x, float y, float r) {
	if (v_pixelDataMapOptions[ui_currentWidthMap] == "Between") {
		return ofRandom(widthMinMax[0], widthMinMax[1]);
	}
	if (v_pixelDataMapOptions[ui_currentWidthMap] == "Color Lightness") {
		return ofMap(c.getLightness(), 0, 255, widthMinMax[0], widthMinMax[1]);
	}
	if (v_pixelDataMapOptions[ui_currentWidthMap] == "Sound") {
		return ofMap(pixelplotter->soundManager.smoothedVol, 0, 0.85, widthMinMax[0], widthMinMax[1]);
	}
	return r;
}

float Df_pixelate::getHeight(ofColor c, float x, float y, float r) {
	if (v_pixelDataMapOptions[ui_currentHeightMap] == "Between") {
		return ofRandom(heightMinMax[0], heightMinMax[1]);
	}
	if (v_pixelDataMapOptions[ui_currentHeightMap] == "Color Lightness") {
		return ofMap(c.getLightness(), 0, 255, heightMinMax[0], heightMinMax[1]);
	}
	if (v_pixelDataMapOptions[ui_currentWidthMap] == "Sound") {
		return ofMap(pixelplotter->soundManager.smoothedVol, 0, 0.85, heightMinMax[0], heightMinMax[1]);
	}
	return r;
}

void Df_pixelate::draw(ofImage* input, float width, float height, float x, float y) {
	if (!bVisible) return;
	setFresh(false);

	int imgW = input->getWidth();
	int imgH = input->getHeight();

	float tileW = (float)imgW / (float)tilesX;
	float tileH = (float)imgH / (float)tilesY;
	float halfTileW = tileW * 0.5;
	float halfTileH = tileH * 0.5;

	int ycount = 0;
	int xcount = 0;
	int ydiv = 0;

	setBlendMode();

	int rSeed = ignoreSeed;
	int pixNo = 0;
	for (float y = 0; y < imgH - halfTileH; y += tileH) {
		(ycount % 2 == 0) ? ydiv = 0 : ydiv = 1;
		for (float x = 0; x < imgW - halfTileW; x += tileW) {
			pixNo++;
			if (v_ignoreOptions[ui_currentIgnore] == "Random") {
				if ((rand() % 100) > ignorePercent) {
					continue;
				}
			}
			else if (v_ignoreOptions[ui_currentIgnore] == "Modulo") {
				if (pixNo % ignoreModulo > 0) {
					continue;
				}
			}
			else if (v_ignoreOptions[ui_currentIgnore] == "Plaid") {
				bool ignoreX = false;
				bool ignoreY = false;
				srand(x);
				if ((rand() % 100) >= ignorePlaid) {
					ignoreX = true;
				}
				srand(y);
				if ((rand() % 100) >= ignorePlaid) {
					ignoreY = true;
				}
				if (ignoreX && ignoreY) {
					continue;
				}
			}
			else if (v_ignoreOptions[ui_currentIgnore] == "Scan") {
				srand(rSeed += ignoreSeedAddon);
				if ((rand() % 100) > ignoreScan) {
					continue;
				}
			}

			if (!polka || ((xcount + ydiv) % 2 == 0)) {
				float fx = x + halfTileW;
				float fy = y + halfTileH;
				ofColor c = input->getPixels().getColor(floor(fx), floor(fy));

				if (useMask) {
					ofColor d = c_mask;
					if ((abs(c.r - d.r) < maskMargin) &&
						(abs(c.g - d.g) < maskMargin) &&
						(abs(c.b - d.b) < maskMargin)) {
						continue;
					}
				}

				float tileWidth = tileW;
				float tileHeight = tileH;

				if (ui_currentWidthMap > 0) {
					tileWidth = getWidth(c, x, y, tileW);
				}

				if (ui_currentHeightMap > 0) {
					tileHeight = getHeight(c, x, y, tileH);
				}

				if (width + height != 0) {
					fx = ofMap(fx, 0, imgW, 0, width);
					fy = ofMap(fy, 0, imgH, 0, height);
				}
				ofPushMatrix();
				ofTranslate(fx, fy, 0);

				if (ui_currentRotationMap > 0) {
					ofRotateZDeg(getRotation(c, x, y));
				}

				// Add or not ...
				tileWidth += ofRandom(0, addonx_rand);
				tileWidth += ofRandom(0, addony_rand);

				// Normal values
				float tw = tileWidth + addonx;
				float th = tileHeight + addony;

				if (width + height != 0) {
					tw = ofMap(tw, 0, imgW, 0, width);
					th = ofMap(th, 0, imgH, 0, height);
				}

				drawPixel(tw, th, c);
				ofPopMatrix();
			}
			xcount++;
		}
		ycount++;
		xcount = 0;
	}
}

void Df_pixelate::gui_setGrey()
{
	c_magentaRed = ofColor(108, 108, 108);
	c_cyanBlue = ofColor(139, 139, 139);
	c_yellowGreen = ofColor(239, 239, 239);
	c_black = ofColor(0, 0, 0);
	setFresh(true);
}
void Df_pixelate::gui_setRGB()
{
	c_magentaRed = ofColor(255, 0, 0);
	c_cyanBlue = ofColor(0, 0, 255);
	c_yellowGreen = ofColor(0, 255, 0);
	c_black = ofColor(0, 0, 0);
	setFresh(true);
}
void Df_pixelate::gui_setCMYK()
{
	c_magentaRed = ofColor(236, 0, 140);
	c_cyanBlue = ofColor(0, 174, 239);
	c_yellowGreen = ofColor(255, 242, 0);
	c_black = ofColor(0, 0, 0);
	setFresh(true);
}
void Df_pixelate::gui_setRYB()
{
	c_magentaRed = ofColor(248, 11, 17);
	c_cyanBlue = ofColor(19, 57, 166);
	c_yellowGreen = ofColor(255, 230, 0);
	c_black = ofColor(0, 0, 0);
	setFresh(true);
}

void Df_pixelate::renderImGuiColourSettings(bool colors, bool mask)
{
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
				if (ImGui::ColorEdit4("Paper / White ##drawFilter", (float*)&c_white, ImGuiColorEditFlags_NoInputs)) {
					setFresh(true);
				}
				// TODO: REMOVE
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
