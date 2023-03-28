#include "drawFilter_Pixelate.h"
#include "ofx2d.h"

extern ofx2d x2d;

void Df_pixelate::loadSettings(ofxXmlSettings settings) {
	name = settings.getValue("name", "pixelate");
	visible = settings.getValue("visible", true);
	useMask = settings.getValue("useMask", false);
	maskMargin = settings.getValue("maskMargin", maskMargin);
	ui_currentPixelType = x2d.getIndex(v_pixelType, settings.getValue("pixelType", "Undefined"), 1);
	tilesX = settings.getValue("tilesX", 64);
	tilesY = settings.getValue("tilesY", 64);
	polka  = settings.getValue("polka", false);
	addonx = settings.getValue("addonx", 0);
	addony = settings.getValue("addony", 0);
	addonx_rand = settings.getValue("addonx_rand", 0);
	addony_rand = settings.getValue("addony_rand", 0);
	offsetx = settings.getValue("offsetx", 0);
	offsety = settings.getValue("offsety", 0);
	offsetx_rand = settings.getValue("offsetx_rand", 0);
	offsety_rand = settings.getValue("offsety_rand", 0);

	ui_currentRotationMap = x2d.getIndex(v_pixelDataMapOptions, settings.getValue("rotationMap", "None"), 0);
	rotationMinMax[0] = settings.getValue("rotationMin", 0);
	rotationMinMax[1] = settings.getValue("rotationMax", 0);

	ui_currentWidthMap = x2d.getIndex(v_pixelDataMapOptions, settings.getValue("widthMap", "None"), 0);
	widthMinMax[0] = settings.getValue("widthMin", 0);
	widthMinMax[1] = settings.getValue("widthMax", 0);

	ui_currentHeightMap = x2d.getIndex(v_pixelDataMapOptions, settings.getValue("heightMap", "None"), 0);
	heightMinMax[0] = settings.getValue("heightMin", 0);
	heightMinMax[1] = settings.getValue("heightMax", 0);

	ui_currentIgnore = x2d.getIndex(v_ignoreOptions, settings.getValue("currentIgnore", "None"), 0);
	ignorePercent = settings.getValue("ignorePercent", ignorePercent);
	ignoreModulo  = settings.getValue("ignoreModulo", ignoreModulo);
	ignorePlaid   = settings.getValue("ignorePlaid", ignorePlaid);
	ignoreScan    = settings.getValue("ignoreScan", ignoreScan);
	ignoreSeed    = settings.getValue("ignoreSeed", ignoreSeed);
	ignoreSeedAddon = settings.getValue("ignoreSeedAddon", ignoreSeedAddon);

	currentBlendModeIndex = settings.getValue("blendMode", 0);

	// Colours
	c_black.x = settings.getValue("colors:black:r", c_black.x);
	c_black.y = settings.getValue("colors:black:g", c_black.y);
	c_black.z = settings.getValue("colors:black:b", c_black.z);
	c_black.w = settings.getValue("colors:black:a", c_black.w);

	c_yellowGreen.x = settings.getValue("colors:yellowGreen:r", c_yellowGreen.x);
	c_yellowGreen.y = settings.getValue("colors:yellowGreen:g", c_yellowGreen.y);
	c_yellowGreen.z = settings.getValue("colors:yellowGreen:b", c_yellowGreen.z);
	c_yellowGreen.w = settings.getValue("colors:yellowGreen:a", c_yellowGreen.w);

	c_magentaRed.x = settings.getValue("colors:magentaRed:r", c_magentaRed.x);
	c_magentaRed.y = settings.getValue("colors:magentaRed:g", c_magentaRed.y);
	c_magentaRed.z = settings.getValue("colors:magentaRed:b", c_magentaRed.z);
	c_magentaRed.w = settings.getValue("colors:magentaRed:a", c_magentaRed.w);

	c_cyanBlue.x = settings.getValue("colors:cyanBlue:r", c_cyanBlue.x);
	c_cyanBlue.y = settings.getValue("colors:cyanBlue:g", c_cyanBlue.y);
	c_cyanBlue.z = settings.getValue("colors:cyanBlue:b", c_cyanBlue.z);
	c_cyanBlue.w = settings.getValue("colors:cyanBlue:a", c_cyanBlue.w);

	c_mask.x = settings.getValue("colors:mask:r", c_mask.x);
	c_mask.y = settings.getValue("colors:mask:g", c_mask.y);
	c_mask.z = settings.getValue("colors:mask:b", c_mask.z);
	c_mask.w = settings.getValue("colors:mask:a", c_mask.w);
}

ofxXmlSettings Df_pixelate::getSettings() {
	ofxXmlSettings settings;
	settings.setValue("name", name);
	settings.setValue("visible", visible);
	settings.setValue("useMask", useMask);
	settings.setValue("maskMargin", maskMargin);
	settings.setValue("pixelType", v_pixelType[ui_currentPixelType]);
	settings.setValue("tilesX", tilesX);
	settings.setValue("tilesY", tilesY);
	settings.setValue("polka", polka);
	settings.setValue("addonx", addonx);
	settings.setValue("addony", addony);
	settings.setValue("addonx_rand", addonx_rand);
	settings.setValue("addony_rand", addony_rand);
	settings.setValue("offsetx", offsetx);
	settings.setValue("offsety", offsety);
	settings.setValue("offsetx_rand", offsetx_rand);
	settings.setValue("offsety_rand", offsety_rand);

	settings.setValue("rotationMap", v_pixelDataMapOptions[ui_currentRotationMap]);
	settings.setValue("rotationMin", rotationMinMax[0]);
	settings.setValue("rotationMax", rotationMinMax[1]);

	settings.setValue("widthMap", v_pixelDataMapOptions[ui_currentWidthMap]);
	settings.setValue("widthMin", widthMinMax[0]);
	settings.setValue("widthMax", widthMinMax[1]);

	settings.setValue("heightMap", v_pixelDataMapOptions[ui_currentHeightMap]);
	settings.setValue("heightMin", heightMinMax[0]);
	settings.setValue("heightMax", heightMinMax[1]);

	settings.setValue("currentIgnore", v_pixelDataMapOptions[ui_currentIgnore]);
	settings.setValue("ignorePercent", ignorePercent);
	settings.setValue("ignoreModulo", ignoreModulo);
	settings.setValue("ignorePlaid", ignorePlaid);
	settings.setValue("ignoreScan", ignoreScan);
	settings.setValue("ignoreSeed", ignoreSeed);
	settings.setValue("ignoreSeedAddon", ignoreSeedAddon);

	settings.setValue("blendMode", currentBlendModeIndex);

	// Colours
	settings.setValue("colors:black:r", c_black.x);
	settings.setValue("colors:black:g", c_black.y);
	settings.setValue("colors:black:b", c_black.z);
	settings.setValue("colors:black:a", c_black.w);

	settings.setValue("colors:yellowGreen:r", c_yellowGreen.x);
	settings.setValue("colors:yellowGreen:g", c_yellowGreen.y);
	settings.setValue("colors:yellowGreen:b", c_yellowGreen.z);
	settings.setValue("colors:yellowGreen:a", c_yellowGreen.w);

	settings.setValue("colors:magentaRed:r", c_magentaRed.x);
	settings.setValue("colors:magentaRed:g", c_magentaRed.y);
	settings.setValue("colors:magentaRed:b", c_magentaRed.z);
	settings.setValue("colors:magentaRed:a", c_magentaRed.w);

	settings.setValue("colors:cyanBlue:r", c_cyanBlue.x);
	settings.setValue("colors:cyanBlue:g", c_cyanBlue.y);
	settings.setValue("colors:cyanBlue:b", c_cyanBlue.z);
	settings.setValue("colors:cyanBlue:a", c_cyanBlue.w);

	settings.setValue("colors:mask:r", c_mask.x);
	settings.setValue("colors:mask:g", c_mask.y);
	settings.setValue("colors:mask:b", c_mask.z);
	settings.setValue("colors:mask:a", c_mask.w);

	return settings;
}

void Df_pixelate::renderImGuiSettings() {
	if (ImGui::CollapsingHeader(name.c_str(), &active)) {
		ImGui::AlignTextToFramePadding();

		if (ImGui::Checkbox("Visible", &visible)) {
			bFresh = true;
		}

		if (ofxImGui::VectorCombo("Pixel Type ##pixelate", &ui_currentPixelType, v_pixelType)) {
			if (ui_currentPixelType > 1) {
				gui_setRGB();
			}
			if (ui_currentPixelType > 5) {
				gui_setCMYK();
			}
		};

		ImGui::PushItemWidth(60);
		ImGui::Text("Tiles"); ImGui::SameLine(75);
		if (ImGui::DragInt("X ##pixelate_tiles", &tilesX, 1, 1, 1200)) {
			bFresh = true;
		}
		ImGui::SameLine();
		if (ImGui::DragInt("Y ##pixelate_tiles", &tilesY, 1, 1, 1200)) {
			bFresh = true;
		}
		ImGui::SameLine();
		if (ImGui::Checkbox("Polka", &polka)) {
			bFresh = true;
		}

		ImGui::Text("+ Addon"); ImGui::SameLine(75);
		if (ImGui::DragFloat("X ##pixelate_addon", &addonx, 0.1f, -100.0f, 100.0f, "%.3f")) {
			bFresh = true;
		}
		ImGui::SameLine();
		if (ImGui::DragFloat("Y ##pixelate_addon", &addony, 0.1f, -100.0f, 100.0f, "%.3f")) {
			bFresh = true;
		}

		ImGui::Text("+ Random"); ImGui::SameLine(75);
		if (ImGui::DragFloat("X ##pixelate_addon_rand", &addonx_rand, 0.1f, -100.0f, 100.0f, "%.3f")) {
			bFresh = true;
		}
		ImGui::SameLine();
		if (ImGui::DragFloat("Y ##pixelate_addon_rand", &addony_rand, 0.1f, -100.0f, 100.0f, "%.3f")) {
			bFresh = true;
		}

		ImGui::Text("+ Skip"); ImGui::SameLine(75);
		if (ofxImGui::VectorCombo("##pixelate_ignore", &ui_currentIgnore, v_ignoreOptions)) {
			bFresh = true;
		}
		if (v_ignoreOptions[ui_currentIgnore] == "Random") {
			ImGui::SameLine();
			if (ImGui::DragInt("% ##pixelate_percent", &ignorePercent, 1, 0, 100)) {
				bFresh = true;
			}
		}
		else if (v_ignoreOptions[ui_currentIgnore] == "Modulo") {
			ImGui::SameLine();
			if (ImGui::DragInt("Mod ##pixelate_modolo", &ignoreModulo, 1, 1, 500)) {
				bFresh = true;
			}
		} else if (v_ignoreOptions[ui_currentIgnore] == "Plaid") {
			ImGui::SameLine();
			if (ImGui::DragInt("% ##pixelate_plaid", &ignorePlaid, 1, 0, 100)) {
				bFresh = true;
			}
		} else if (v_ignoreOptions[ui_currentIgnore] == "Scan") {
			ImGui::SameLine();
			if (ImGui::DragInt("% ##pixelate_scan", &ignoreScan, 1, 0, 100)) {
				bFresh = true;
			}
			ImGui::Text("  + Seed"); ImGui::SameLine(75);
			if (ImGui::DragInt("Start ##pixelate_seed", &ignoreSeed, 1, 0, 500)) {
				bFresh = true;
			}
			ImGui::SameLine();
			if (ImGui::DragInt("Step ##pixelate_seedAddon", &ignoreSeedAddon, 1, 1, 250)) {
				bFresh = true;
			}
		}
 
		ImGui::Separator();

		ImGui::Text("Offset"); ImGui::SameLine(75);
		if (ImGui::DragFloat("X ##pixelate_offsetx", &offsetx, 0.1f, -250.0f, 250.0f, "%.3f")) {
			bFresh = true;
		}
		ImGui::SameLine();
		if (ImGui::DragFloat("Y ##pixelate_offsety", &offsety, 0.1f, -250.0f, 250.0f, "%.3f")) {
			bFresh = true;
		}

		ImGui::Text("+ Random"); ImGui::SameLine(75);
		if (ImGui::DragFloat("X ##pixelate_offsetX_random", &offsetx_rand, 0.1f, 0.0f, 500.0f, "%.3f")) {
			bFresh = true;
		}
		ImGui::SameLine();
		if (ImGui::DragFloat("Y ##pixelate_offsetY_random", &offsety_rand, 0.1f, -100.0f, 100.0f, "%.3f")) {
			bFresh = true;
		}

		ImGui::Separator();

		ImGui::Text("Rotation"); ImGui::SameLine(75);
		if (ofxImGui::VectorCombo("##pixelate_rotation", &ui_currentRotationMap, v_pixelDataMapOptions)) {
			bFresh = true;
		}
		if (ui_currentRotationMap > 0) {
			ImGui::Text(" "); ImGui::SameLine(75);
			if (ImGui::DragFloat("Min ##pixelate_rotation", &rotationMinMax[0], 0.1f, -360.0f, 360.0f, "%.3f")) {
				bFresh = true;
			}
			ImGui::SameLine();
			if (ImGui::DragFloat("Max ##pixelate_rotation", &rotationMinMax[1], 0.1f, -360.0f, 360.0f, "%.3f")) {
				bFresh = true;
			}
		}

		ImGui::Separator();

		ImGui::Text("Width"); ImGui::SameLine(75);
		if (ofxImGui::VectorCombo("##pixelate_width", &ui_currentWidthMap, v_pixelDataMapOptions)) {
			bFresh = true;
		}
		if (ui_currentWidthMap > 0) {
			ImGui::Text(" "); ImGui::SameLine(75);
			if (ImGui::DragFloat("Min ##pixelate_width", &widthMinMax[0], 0.1f, -250.0f, 250.0f, "%.3f")) {
				bFresh = true;
			}
			ImGui::SameLine();
			if (ImGui::DragFloat("Max ##pixelate_height", &widthMinMax[1], 0.1f, -250.0f, 250.0f, "%.3f")) {
				bFresh = true;
			}
		}

		ImGui::Text("Height"); ImGui::SameLine(75);
		if (ofxImGui::VectorCombo("##pixelate_height", &ui_currentHeightMap, v_pixelDataMapOptions)) {
			bFresh = true;
		}
		if (ui_currentHeightMap > 0) {
			ImGui::Text(" "); ImGui::SameLine(75);
			if (ImGui::DragFloat("Min ##pixelate_height", &heightMinMax[0], 0.1f, -250.0f, 250.0f, "%.3f")) {
				bFresh = true;
			}
			ImGui::SameLine();
			if (ImGui::DragFloat("Max ##pixelate_height", &heightMinMax[1], 0.1f, -250.0f, 250.0f, "%.3f")) {
				bFresh = true;
			}
		}

		ImGui::PopItemWidth();

		if (ui_currentPixelType > 1) {
			renderImGuiColourSettings(true, true);
		}
		else {
			renderImGuiColourSettings(false, true);
		}

		ImGui::PushItemWidth(200);
		if(ofxImGui::VectorCombo("##Blend Mode", &currentBlendModeIndex, v_BlendModes)) {
			bFresh = true;
		}
		ImGui::PopItemWidth();
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
	ofDrawEllipse(offsetX, offsetY, w, h);
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
	drawRectangle(offsetX, offsetY, cWidth, h, c_magentaRed);

	ofTranslate(cWidth * 0.5, 0, 0);
	cWidth = ofMap(c.g + addon, 0, maxC, 0, w);
	ofTranslate(cWidth * 0.5, 0, 0);
	drawRectangle(offsetX, offsetY, cWidth, h, c_yellowGreen);

	ofTranslate(cWidth * 0.5, 0, 0);
	cWidth = ofMap(c.b + addon, 0, maxC, 0, w);
	ofTranslate(cWidth * 0.5, 0, 0);
	drawRectangle(offsetX, offsetY, cWidth, h, ofColor(0, 0, 255));

	ofPopMatrix();
};

void Df_pixelate::drawRgbSeperation_Center(float offsetX, float offsetY, float w, float h, ofColor c) {
	float maxWidth = w / 3;
	float cWidth;

	cWidth = ofMap(c.r, 0, 255, 0, maxWidth);
	drawRectangle(offsetX, offsetY, cWidth, h, c_magentaRed); // Red

	ofPushMatrix(); // offset

	ofTranslate(maxWidth, 0, 0);
	cWidth = ofMap(c.g, 0, 255, 0, maxWidth);
	drawRectangle(offsetX, offsetY, cWidth, h, c_yellowGreen); // Green

	ofTranslate(maxWidth, 0, 0);
	cWidth = ofMap(c.b, 0, 255, 0, maxWidth);
	drawRectangle(offsetX, offsetY, cWidth, h, c_cyanBlue); // Blue

	ofPopMatrix();
};

void Df_pixelate::drawRgbSeperation_Square(float offsetX, float offsetY, float w, float h, ofColor c) {
	float cWidth, cHeight;

	cWidth = ofMap(c.r, 0, 255, 0, w);
	cHeight = ofMap(c.r, 0, 255, 0, h);
	drawRectangle(offsetX, offsetY, cWidth, cHeight, c_magentaRed);

	cWidth = ofMap(c.g, 0, 255, 0, w);
	cHeight = ofMap(c.g, 0, 255, 0, h);
	drawRectangle(offsetX, offsetY, cWidth, cHeight, c_yellowGreen);

	cWidth = ofMap(c.b, 0, 255, 0, w);
	cHeight = ofMap(c.b, 0, 255, 0, h);
	drawRectangle(offsetX, offsetY, cWidth, cHeight, c_cyanBlue);
};

void Df_pixelate::drawCMYKSeperation_Square(float offsetX, float offsetY, float w, float h, ofColor c) {
	float cWidth, cHeight;
	ofVec4f cmyk = getCMYK(c);

	cWidth = ofMap(cmyk[2], 0, 1, 0, w);
	cHeight = ofMap(cmyk[2], 0, 1, 0, h);
	drawRectangle(offsetX, offsetY, cWidth, cHeight, c_yellowGreen); // Yellow

	cWidth = ofMap(cmyk[1], 0, 1, 0, w);
	cHeight = ofMap(cmyk[1], 0, 1, 0, h);
	drawRectangle(offsetX, offsetY, cWidth, cHeight, c_magentaRed); // Magenta

	cWidth = ofMap(cmyk[0], 0, 1, 0, w);
	cHeight = ofMap(cmyk[0], 0, 1, 0, h);
	drawRectangle(offsetX, offsetY, cWidth, cHeight, c_cyanBlue); // Cyan

	cWidth = ofMap(cmyk[3], 0, 1, 0, w);
	cHeight = ofMap(cmyk[3], 0, 1, 0, h);
	drawRectangle(offsetX, offsetY, cWidth, cHeight, c_black); // Black

};

void Df_pixelate::drawCMYKSeperation_Left(float offsetX, float offsetY, float w, float h, ofColor c) {
	float cWidth;
	ofVec4f cmyk = getCMYK(c);

	ofPushMatrix(); // offset

	cWidth = ofMap(cmyk[3], 0, 1, 0, w / 2);
	ofTranslate(-(w * 0.5) + (cWidth * 0.5), 0, 0);
	drawRectangle(offsetX, offsetY, cWidth, h, c_black); // Black
	ofTranslate(cWidth * 0.5, 0, 0);

	cWidth = ofMap(cmyk[0], 0, 1, 0, w / 2);
	ofTranslate(cWidth * 0.5, 0, 0);
	drawRectangle(offsetX, offsetY, cWidth, h, c_cyanBlue); // Cyan
	ofTranslate(cWidth * 0.5, 0, 0);

	cWidth = ofMap(cmyk[1], 0, 1, 0, w / 2);
	ofTranslate(cWidth * 0.5, 0, 0);
	drawRectangle(offsetX, offsetY, cWidth, h, c_magentaRed); // Magenta
	ofTranslate(cWidth * 0.5, 0, 0);

	cWidth = ofMap(cmyk[2], 0, 1, 0, w / 2);
	ofTranslate(cWidth * 0.5, 0, 0);
	drawRectangle(offsetX, offsetY, cWidth, h, c_yellowGreen); // Yellow

	ofPopMatrix();
};

void Df_pixelate::drawCMYKSeperation_Hills(float offsetX, float offsetY, float w, float h, ofColor c) {
	float cHeight;
	ofVec4f cmyk = getCMYK(c);

	float total = cmyk[0] + cmyk[1] + cmyk[2];
	float pc = (100 * cmyk[0]) / total;
	float pm = (100 * cmyk[1]) / total;
	float py = (100 * cmyk[2]) / total;

	cHeight = (pc / 100) * h;
	ofPushMatrix();
	ofTranslate(0, (-h * 0.5) + (cHeight * 0.5), 0);
	drawRectangle(offsetX, offsetY, w, cHeight, c_cyanBlue); // Cyan

	ofTranslate(0, cHeight * 0.5, 0);

	cHeight = (pm / 100) * h;
	ofTranslate(0, cHeight * 0.5, 0);
	drawRectangle(offsetX, offsetY, w, cHeight, c_magentaRed); // Magenta

	ofTranslate(0, cHeight * 0.5, 0);

	cHeight = (py / 100) * h;
	ofTranslate(0, cHeight * 0.5, 0);
	drawRectangle(offsetX, offsetY, w, cHeight, c_yellowGreen); // Yellow

	ofPopMatrix();

	cHeight = ofMap(cmyk[3], 0, 1, 0, h);
	drawRectangle(offsetX, offsetY, w, cHeight, c_black); // c_black
};

void Df_pixelate::drawPixel(float w, float h, ofColor c) {

	float offsetX = offsetx + ofRandom(0, offsetx_rand);
	float offsetY = offsety + ofRandom(0, offsetx_rand);

	switch (ui_currentPixelType) {
	case 0:
		drawRectangle(offsetX, offsetY, w, h, c);
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
		drawCMYKSeperation_Square(offsetX, offsetY, w, h, c);
		break;
	case 6:
		drawCMYKSeperation_Left(offsetX, offsetY, w, h, c);
		break;
	case 7:
		drawCMYKSeperation_Hills(offsetX, offsetY, w, h, c);
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
	return r;
}

float Df_pixelate::getHeight(ofColor c, float x, float y, float r) {
	if (v_pixelDataMapOptions[ui_currentHeightMap] == "Between") {
		return ofRandom(heightMinMax[0], heightMinMax[1]);
	}
	if (v_pixelDataMapOptions[ui_currentHeightMap] == "Color Lightness") {
		return ofMap(c.getLightness(), 0, 255, heightMinMax[0], heightMinMax[1]);
	}
	return r;
}

void Df_pixelate::draw(ofImage* input) {
	bFresh = false;
	if (!visible) return;

	int imgW = input->getWidth();
	int imgH = input->getHeight();
	float tileW = (float)imgW / (float)tilesX;
	float tileH = (float)imgH / (float)tilesY;
	float halfTileW = tileW * 0.5;
	float halfTileH = tileH * 0.5;

	int ycount = 0;
	int xcount = 0;
	int ydiv   = 0;

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
					if ( (abs(c.r - d.r) < maskMargin) &&
						 (abs(c.g - d.g) < maskMargin) &&
						 (abs(c.b - d.b) < maskMargin) ){
						continue;
					}
				}

				float tileWidth = tileW;
				float tileHeight = tileH;
				float offsetX = offsetx;
				float offsetY = offsety;

				if (ui_currentWidthMap > 0) {
					tileWidth = getWidth(c, x, y, tileW);
				}

				if (ui_currentHeightMap > 0) {
					tileHeight = getHeight(c, x, y, tileH);
				}

				ofPushMatrix();
				ofTranslate(fx * drawScale, fy * drawScale, 0);

				if(ui_currentRotationMap > 0) {
					ofRotateZDeg(getRotation(c,x,y));
				}

				// Add or not ...
				tileWidth += ofRandom(0, addonx_rand);
				tileWidth += ofRandom(0, addony_rand);
				offsetX += ofRandom(0, offsetx_rand);
				offsetY += ofRandom(0, offsetx_rand);

				drawPixel( (tileWidth + addonx) * drawScale, (tileHeight + addony) * drawScale, c);
				ofPopMatrix();
			}
			xcount++;
		}
		ycount++;
		xcount = 0;
	}
}
