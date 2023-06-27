#include "ofApp.h";
#include "imageFilter_DrawPixel.h"

ofxXmlSettings If_drawPixel::getSettings() {
	ofxXmlSettings settings;
	settings.setValue("name", name);
	settings.setValue("pixelType", drawPixels.v_DrawPixelsNames[selectedPixelType]);
	settings.setValue("hCount", hCount);
	settings.setValue("vCount", vCount);

	settings.setValue("colors:col:r", c_col.x);
	settings.setValue("colors:col:g", c_col.y);
	settings.setValue("colors:col:b", c_col.z);
	settings.setValue("colors:col:a", c_col.w);

	return settings;
}

void If_drawPixel::loadSettings(ofxXmlSettings settings) {

	selectedPixelType = x2d.getIndex(drawPixels.v_DrawPixelsNames, settings.getValue("pixelType", "Undefined"), 1);
	hCount = settings.getValue("hCount", hCount);
	vCount = settings.getValue("vCount", vCount);

	c_col.x = settings.getValue("colors:col:r", c_col.x);
	c_col.y = settings.getValue("colors:col:g", c_col.y);
	c_col.z = settings.getValue("colors:col:b", c_col.z);
	c_col.w = settings.getValue("colors:col:a", c_col.w);
	return;
}

void If_drawPixel::renderImGuiSettings() {
	if (ImGui::CollapsingHeader(name.c_str(), &active)) {
		ImGui::AlignTextToFramePadding();
		ImGui::PushItemWidth(60);

		if (ofxImGui::VectorCombo("Pixel Type ##drawPixel", &selectedPixelType, drawPixels.v_DrawPixelsNames)) {
			setFresh(true);
		};

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

		for (float x = 0; x < hCount * width; x += width) {
			drawPixels.v_DrawPixels[selectedPixelType]->draw(c_col, x+(width*0.5), newY + (height * 0.5), width, height);
			xcount++;
		}
		ycount++;
	}
	cfbo.end();

	cfbo.readToPixels(img->getPixelsRef());
	img->update();
	bFresh = false;
}
