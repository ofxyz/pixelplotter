#include "drawFilters.h"

void Df_pixelate::renderImGuiSettings() {
	if (ImGui::CollapsingHeader(name.c_str(), &active)) {
		ImGui::AlignTextToFramePadding();

		/* Save and load presets, probably needs to move to parent class ...
/
		if (ImGui::Button("Quick Save"))
		{
			string savePath = "presets\/quicksave.xml";
			saveSettings(savePath);
		}
		ImGui::SameLine();
		if (ImGui::Button("Quick Load"))
		{
			string savePath = "presets\/quicksave.xml";
			loadSettings(savePath);
		}
		*/

		/*
		if (ofxImGui::VectorCombo("##Presets", &currentPresetIndex, presetFileNames))
		{
			loadSettings(presetFiles[currentPresetIndex].getAbsolutePath());
		}

		if (presetFileNames.size() > 0) {
			ImGui::SameLine();
			if (ImGui::Button("Delete Preset"))
			{
				presetFiles[currentPresetIndex].remove();
				gui_loadPresets();
			}
		}

		if (bSavePreset) {
			ImGui::InputText("##presetname", presetSaveName, IM_ARRAYSIZE(presetSaveName));
			ImGui::SameLine();
		}
		if (ImGui::Button("Save Preset"))
		{
			if (bSavePreset) {
				string savePath = "presets\/" + string(presetSaveName) + ".xml";
				saveSettings(savePath);
				gui_loadPresets();
				currentPresetIndex = getIndex(presetFileNames, string(presetSaveName), 0);
				bSavePreset = false;
			}
			else {
				if (presetFileNames.size() > 0) {
					strcpy(presetSaveName, presetFileNames[currentPresetIndex].c_str());
				}
				bSavePreset = true;
			}
		}
		*/

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
		ImGui::DragInt("X ##pixelate_tiles", &tilesX, 1, 1, 1200);
		ImGui::SameLine();
		ImGui::DragInt("Y ##pixelate_tiles", &tilesY, 1, 1, 1200);
		ImGui::SameLine();
		ImGui::Checkbox("Polka", &polka);

		ImGui::Text("+ Addon"); ImGui::SameLine(75);
		ImGui::DragFloat("X ##pixelate_addon", &addonx, 0.1f, -100.0f, 100.0f, "%.3f");
		ImGui::SameLine();
		ImGui::DragFloat("Y ##pixelate_addon", &addony, 0.1f, -100.0f, 100.0f, "%.3f");

		ImGui::Text("+ Random"); ImGui::SameLine(75);
		ImGui::DragFloat("X ##pixelate_addon_rand", &addonx_rand, 0.1f, -100.0f, 100.0f, "%.3f");
		ImGui::SameLine();
		ImGui::DragFloat("Y ##pixelate_addon_rand", &addony_rand, 0.1f, -100.0f, 100.0f, "%.3f");

		ImGui::Separator();

		ImGui::Text("Offset"); ImGui::SameLine(75);
		ImGui::DragFloat("X ##pixelate_offsetx", &offsetx, 0.1f, -250.0f, 250.0f, "%.3f");
		ImGui::SameLine();
		ImGui::DragFloat("Y ##pixelate_offsety", &offsety, 0.1f, -250.0f, 250.0f, "%.3f");

		ImGui::Text("+ Random"); ImGui::SameLine(75);
		ImGui::DragFloat("X ##pixelate_offsetX_random", &offsetx_rand, 0.1f, 0.0f, 500.0f, "%.3f");
		ImGui::SameLine();
		ImGui::DragFloat("Y ##pixelate_offsetY_random", &offsety_rand, 0.1f, -100.0f, 100.0f, "%.3f");

		ImGui::Separator();

		ImGui::Text("Rotation"); ImGui::SameLine(75);
		ofxImGui::VectorCombo("##pixelate_rotation", &ui_currentRotationMap, v_pixelDataMapOptions);
		if (ui_currentRotationMap > 0) {
			ImGui::Text(" "); ImGui::SameLine(75);
			ImGui::DragFloat("Min ##pixelate_rotation", &rotationMinMax[0], 0.1f, -360.0f, 360.0f, "%.3f");
			ImGui::SameLine();
			ImGui::DragFloat("Max ##pixelate_rotation", &rotationMinMax[1], 0.1f, -360.0f, 360.0f, "%.3f");
		}

		ImGui::Separator();

		ImGui::Text("Width"); ImGui::SameLine(75);
		ofxImGui::VectorCombo("##pixelate_width", &ui_currentWidthMap, v_pixelDataMapOptions);
		if (ui_currentWidthMap > 0) {
			ImGui::Text(" "); ImGui::SameLine(75);
			ImGui::DragFloat("Min ##pixelate_width", &widthMinMax[0], 0.1f, -250.0f, 250.0f, "%.3f");
			ImGui::SameLine();
			ImGui::DragFloat("Max ##pixelate_height", &widthMinMax[1], 0.1f, -250.0f, 250.0f, "%.3f");
		}

		ImGui::Text("Height"); ImGui::SameLine(75);
		ofxImGui::VectorCombo("##pixelate_height", &ui_currentHeightMap, v_pixelDataMapOptions);
		if (ui_currentHeightMap > 0) {
			ImGui::Text(" "); ImGui::SameLine(75);
			ImGui::DragFloat("Min ##pixelate_rotation", &heightMinMax[0], 0.1f, -250.0f, 250.0f, "%.3f");
			ImGui::SameLine();
			ImGui::DragFloat("Max ##pixelate_rotation", &heightMinMax[1], 0.1f, -250.0f, 250.0f, "%.3f");
		}

		ImGui::PopItemWidth();

		if (ui_currentPixelType > 1) {
			renderImGuiColourSettings(true);
		}

		ImGui::PushItemWidth(200);
		ofxImGui::VectorCombo("##Blend Mode", &currentBlendModeIndex, v_BlendModes);
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
		ofLog() << "Not a valid Draw Filter: " << ui_currentPixelType << endl;
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

	// This needs some control...
	//int rSeed = 0;

	for (float y = 0; y < imgH - halfTileH; y += tileH) {
		//rSeed += 100;
		//ofSeedRandom(rSeed++);
		(ycount % 2 == 0) ? ydiv = 0 : ydiv = 1;
		for (float x = 0; x < imgW - halfTileW; x += tileW) {
			if (!polka || ((xcount + ydiv) % 2 == 0)) {
				//rSeed += 200;
				//ofSeedRandom(rSeed++);
				float fx = x + halfTileW;
				float fy = y + halfTileH;
				ofColor c = input->getPixels().getColor(floor(fx), floor(fy));
				
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
