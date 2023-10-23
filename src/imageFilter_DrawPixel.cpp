#include "imageFilter_DrawPixel.h"
#include "drawPixel.h"
#include "ofx2d.h"
#include "ofxImGui.h"
#include "imgui_internal.h"
#include "ImHelpers.h"

ofJson If_drawPixel::getSettings() {
	ofJson settings;
	settings["name"] = name;
	settings["pixelType"] = drawPixels.v_DrawPixelsNames[selectedPixelType];
	settings["hCount"] = hCount;
	settings["vCount"] = vCount;
	settings["bMirror"] = bMirror;
	settings["colors"]["col"]["r"] = c_col.x;
	settings["colors"]["col"]["g"] = c_col.y;
	settings["colors"]["col"]["b"] = c_col.z;
	settings["colors"]["col"]["a"] = c_col.w;

	ofJson pixelSettings = drawPixels.v_DrawPixels[selectedPixelType]->getSettings();
	settings["pixelSettings"] = pixelSettings;

	return settings;
}

void If_drawPixel::loadSettings(ofJson settings) {

	selectedPixelType = ofx2d::getIndex(drawPixels.v_DrawPixelsNames, settings.value("pixelType", "Undefined"), 1);
	hCount = settings.value("hCount", hCount);
	vCount = settings.value("vCount", vCount);
	bMirror = settings.value("bMirror", bMirror);
	c_col.x = settings["colors"]["col"].value("r", c_col.x);
	c_col.y = settings["colors"]["col"].value("g", c_col.y);
	c_col.z = settings["colors"]["col"].value("b", c_col.z);
	c_col.w = settings["colors"]["col"].value("a", c_col.w);

	ofJson pixelSettings = settings.value("pixelSettings", pixelSettings);
	drawPixels.v_DrawPixels[selectedPixelType]->loadSettings(pixelSettings);
}

void If_drawPixel::renderImGuiSettings() {
	if (ImGui::CollapsingHeader(name.c_str(), &active)) {
		ImGui::AlignTextToFramePadding();

		if (ofxImGui::VectorCombo("Pixel Type ##drawPixel", &selectedPixelType, drawPixels.v_DrawPixelsNames)) {
			setFresh(true);
		};
 
		ImGui::PushItemWidth(60);
		 
		if (ImGui::ColorEdit4("Color ##drawPixel", (float*)&c_col, ImGuiColorEditFlags_NoInputs)) {
			setFresh(true);
		}

		ImGui::Text("Copies"); ImGui::SameLine(75);
		if (ImGui::DragInt("Horz ##drawpixel_hCount", &hCount, 1, 1, 100)) {
			setFresh(true);
		}
		ImGui::SameLine();
		if (ImGui::DragInt("Vert ##drawpixel_vCount", &vCount, 1, 1, 100)) {
			setFresh(true);
		}
		ImGui::SameLine();
		if (ImGui::Checkbox("Mirror", &bMirror)) {
			bFresh = true;
		}

		// Pass fresh through
		if (drawPixels.v_DrawPixels[selectedPixelType]->isFresh()) {
			setFresh(true);
		}

		drawPixels.v_DrawPixels[selectedPixelType]->renderImGuiSettings();

		ImGui::PopItemWidth();
	}
}

void If_drawPixel::apply(ofImage* img) {
	ofFbo cfbo;
	cfbo.allocate(img->getWidth(), img->getHeight(), GL_RGBA);

	float width = img->getWidth() / hCount;
	float height = img->getHeight() / vCount;

	cfbo.begin();
	cfbo.clearColorBuffer(ofColor(255, 255, 255));
	int xcount = 0;
	int ycount = 0;
	for (float y = 0; y < vCount * height; y += height) {
		// Reset for every line
		float newHeight = height;
		float newY = y;
		if (bMirror && (ycount % 2 == 1)) {
			newHeight = -height;
			newY = y + height;
		}

		for (float x = 0; x < hCount * width; x += width) {
			//drawPixels.v_DrawPixels[selectedPixelType]->draw(c_col, x + (width * 0.5), newY + (newHeight * 0.5), width, newHeight);

			if (bMirror && (xcount % 2 == 0)) {
				drawPixels.v_DrawPixels[selectedPixelType]->draw(c_col, x + (width * 0.5), newY + (newHeight * 0.5), -width, newHeight);
				//img->draw(x + width, newY, -width, newHeight);
			}
			else {
				//img->draw(x, newY, width, newHeight);
				drawPixels.v_DrawPixels[selectedPixelType]->draw(c_col, x + (width * 0.5), newY + (newHeight * 0.5), width, newHeight);
			}

			xcount++;
		}
		ycount++;
	}
	cfbo.end();

	cfbo.readToPixels(img->getPixels());
	img->update();
	bFresh = false;
}
