#include "imageFilter_DrawPixel.h"
#include "drawPixel.h"
#include "ofx2d.h"
#include "ofxImGui.h"
#include "imgui_internal.h"
#include "ImHelpers.h"

ofJson If_drawPixel::getSettings() {
	ofJson settings;
	settings["name"] = name;
	// TODO: It shouldn't matter but ... isn't it better to get the actual source name
	//settings["pixelType"] = drawPixels.v_Objects[selectedPixelType]->name;
	settings["pixelType"] = drawPixels.v_objectNames[selectedPixelType];
	settings["hCount"] = hCount;
	settings["vCount"] = vCount;
	settings["bMirror"] = bMirror;
	settings["colors"]["col"]["r"] = c_col.x;
	settings["colors"]["col"]["g"] = c_col.y;
	settings["colors"]["col"]["b"] = c_col.z;
	settings["colors"]["col"]["a"] = c_col.w;

	ofJson pixelSettings = drawPixels.v_Objects[selectedPixelType]->getSettings();
	settings["pixelSettings"] = pixelSettings;

	return settings;
}

void If_drawPixel::loadSettings(ofJson& settings) {
	// TODO: It shouldn't matter but ... isn't it better to get the actual source name
	//ofx2d::getIndex(drawPixels.v_Objects*, settings.value("pixelType", "Undefined"), 1)
	selectedPixelType = ofx2d::getIndex(drawPixels.v_objectNames, settings.value("pixelType", "Undefined"), 1);
	hCount = settings.value("hCount", hCount);
	vCount = settings.value("vCount", vCount);
	bMirror = settings.value("bMirror", bMirror);
	c_col.x = settings["colors"]["col"].value("r", c_col.x);
	c_col.y = settings["colors"]["col"].value("g", c_col.y);
	c_col.z = settings["colors"]["col"].value("b", c_col.z);
	c_col.w = settings["colors"]["col"].value("a", c_col.w);

	ofJson pixelSettings = settings.value("pixelSettings", pixelSettings);
	drawPixels.v_Objects[selectedPixelType]->loadSettings(pixelSettings);
}

void If_drawPixel::renderImGuiSettings() {
	if (ImGui::CollapsingHeader(name.c_str(), &bAlive)) {
		ImGui::AlignTextToFramePadding();

		if (ofxImGui::VectorCombo("Pixel Type ##drawPixel", &selectedPixelType, drawPixels.v_menuValues)) {
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
			setFresh(true);
		}

		// Pass fresh through
		if (drawPixels.v_Objects[selectedPixelType]->isFresh()) {
			setFresh(true);
		}

		drawPixels.v_Objects[selectedPixelType]->renderImGuiSettings();

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
		ofPushMatrix();
		ofTranslate(0, height * 0.5);
		ofTranslate(0, y);
		
		for (float x = 0; x < hCount * width; x += width) {
			ofPushMatrix();
			ofTranslate((width * 0.5), 0);
			ofTranslate(x, 0);

			if (bMirror && (xcount % 2 == 0)) {
				ofScale(-1, 1);
			}
			if (bMirror && (ycount % 2 == 0)) {
				ofScale(1, -1);
			}

			float xNorm = (float)xcount / (float)(hCount-1);
			float yNorm = (float)ycount / (float)(vCount-1);

			drawPixels.v_Objects[selectedPixelType]->draw(c_col, { width, height }, { xcount, ycount }, { xNorm, yNorm });
			
			xcount++;
			ofPopMatrix();
		}
		xcount = 0;
		ycount++;
		ofPopMatrix();
	}

	cfbo.end();

	cfbo.readToPixels(img->getPixels());
	img->update();
	setFresh(false);
}
