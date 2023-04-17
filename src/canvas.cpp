#pragma once
#include "canvas.h"
#include "ofApp.h"

void Canvas::renderImGuiSettings() {
	if (ImGui::ColorEdit4("Canvas Colour", (float*)&c_canvas, ImGuiColorEditFlags_NoInputs)) {
		fresh = true;
	}

	ImGui::Separator(); // Start Size

	ImGui::PushItemWidth(60);
	ImGui::Text("Size"); ImGui::SameLine(75);
	if (ImGui::DragInt("W ##canvas_W", &canvasWidth, 1, 16, 2400)) {
		resizeRequest = true;
	}
	ImGui::SameLine();
	if (ImGui::DragInt("H ##canvas_H", &canvasHeight, 1, 16, 2400)) {
		resizeRequest = true;
	}
	ImGui::SameLine();
	if (ImGui::Button("Reset")) {
		canvasWidth = sourceWidth;
		canvasHeight = sourceHeight;
		resizeRequest = true;
	}

	ImGui::Separator(); // End Size 

	/* // Start Rec
	if (isRecording) {
		if (ImGui::Button("Stop Recording"))
		{
			isRecording = false;
		}
	}
	else {
		if (ImGui::Button("Start Recording"))
		{
			fresh = true;
			isRecording = true;
		}
	}
	*/

	if (ImGui::Button("Export Vector")) {
		saveVector = true;
		fresh = true;
	}
	ImGui::SameLine();
	if (ImGui::Button("Export PNG")) {
		savePixels = true;
		fresh = true;
	}
}

void Canvas::loadSettings(ofxXmlSettings settings) {
	canvasTitle = settings.getValue("canvasTitle", canvasTitle);
	canvasWidth = settings.getValue("canvasWidth", canvasWidth);
	canvasHeight = settings.getValue("canvasHeight", canvasHeight);

	c_canvas.x = settings.getValue("colors:canvas:r", c_canvas.x);
	c_canvas.y = settings.getValue("colors:canvas:g", c_canvas.y);
	c_canvas.z = settings.getValue("colors:canvas:b", c_canvas.z);
	c_canvas.w = settings.getValue("colors:canvas:a", c_canvas.w);

	resizeRequest = true;
	fresh = true;
}

ofxXmlSettings Canvas::getSettings() {
	ofxXmlSettings settings;
	settings.setValue("canvasTitle", canvasTitle);
	settings.setValue("canvasWidth", canvasWidth);
	settings.setValue("canvasHeight", canvasHeight);

	settings.setValue("colors:canvas:r", c_canvas.x);
	settings.setValue("colors:canvas:g", c_canvas.y);
	settings.setValue("colors:canvas:b", c_canvas.z);
	settings.setValue("colors:canvas:a", c_canvas.w);

	return settings;
}

void Canvas::setup(ofApp* app, ofImage* img, string canvas_title) {
	pixelplotter = app;
	canvasTitle  = canvas_title;
	setSourceDimension(img);
	canvasFbo.allocate(canvasWidth, canvasHeight, GL_RGB, 8);
	canvasFbo.getTextureReference().setTextureMinMagFilter(GL_NEAREST, GL_NEAREST);
	update(img);
}

void Canvas::setSourceDimension() {
	canvasWidth = sourceWidth;
	canvasHeight = sourceHeight;
	resizeRequest = true;
}

void Canvas::setSourceDimension(ofImage* img) {
	sourceWidth = img->getWidth();
	sourceHeight = img->getHeight();
	setSourceDimension();
}

void Canvas::update() {

	if (resizeRequest) {
		canvasFbo.allocate(canvasWidth, canvasHeight, GL_RGB, 8);
		canvasFbo.getTextureReference().setTextureMinMagFilter(GL_NEAREST, GL_NEAREST);
		resizeRequest = false;
		fresh = true;
		return;
	}
	if (isRecording) {
		fresh = true;
		return;
	}
	for (const auto& filter : dF.v_DrawFilters) {
		if (filter->isFresh()) {
			fresh = true;
			return;
		}
	}
}

void Canvas::update(ofImage* img) {
	canvasFbo.begin();
	if (saveVector) {
		// This swaps out the gl renderer for the ciaro renderer
		ofBeginSaveScreenAsPDF("export//" + canvasTitle + "_" + to_string(++exportCount) + ".pdf", false);
	}

	ofClear(c_canvas);

	for (const auto& filter : dF.v_DrawFilters) {
		// update one filter per frame to keep things speeedy?
		// Each filter draws to it's own fbo and are drawn here?
		// filter->update(img); filter->update(settings)
		filter->draw(img, canvasWidth, canvasHeight);
	}

	if (saveVector) {
		ofEndSaveScreenAsPDF();
		saveVector = false;
	}

	canvasFbo.end();

	if (isRecording || savePixels) {
		canvasFbo.readToPixels(canvasPix);
	}

	if (isRecording) {
		ofSaveImage(canvasPix, "export//frames//" + canvasTitle + "_" + with_leading_zero(++recFrameCount, 8) + ".png");
	}
	if (savePixels) {
		ofSaveImage(canvasPix, "export//" + canvasTitle + "_" + to_string(++exportCount) + ".png");
		savePixels = false;
	}

	fresh = true;
};
