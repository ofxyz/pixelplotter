#pragma once

#include "canvas.h"
#include "ofApp.h"
#include "ImGui_General.h"
#include "ofx2d.h"

Canvas::Canvas()
{
	pixelplotter = (ofApp*)ofGetAppPtr();
}

void Canvas::renderImGuiSettings() {
	ImGui::Indent();

	ImGui::InputText("Title", &canvasTitle);

	if (ImGui::ColorEdit4("Canvas Colour", (float*)&c_canvas, ImGuiColorEditFlags_NoInputs)) {
		setFresh(true);
	}

	ImGui::Separator(); // Start Size

	// TODO: Add ratio's (1:1, 1:1.25, Custom)
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
	if (ImGui::Button("Source")) {
		canvasWidth = sourceController.frameBuffer.getWidth();
		canvasHeight = sourceController.frameBuffer.getHeight();
		resizeRequest = true;
	}
	ImGui::SameLine();
	if (ImGui::Button("Fit")) {
		;
		if (pixelplotter != nullptr) {
			ImVec4 as = pixelplotter->ofGui.availableSpace();
			canvasWidth  = abs(as.x);
			canvasHeight = abs(as.y);
			pixelplotter->resetZoom();
			pixelplotter->resetImageOffset();
			resizeRequest = true;
		}
	}
	ImGui::SameLine();
	if (ImGui::Button("x2")) {
		canvasWidth  *= 2;
		canvasHeight *= 2;
		resizeRequest = true;
	}
	ImGui::PopItemWidth();
	ImGui::Separator(); // End Size 

	string sSourceFilterCount = "Plot Source (" + ofToString(sourceController.iF.v_Objects.size() + 1) + ")###Source";
	ImGui::SetNextItemOpen(true, ImGuiCond_Once);
	if (ImGui::CollapsingHeader(sSourceFilterCount.c_str()))
	{
		sourceController.renderImGuiSettings();
		sourceController.iF.renderImGuiSettings();
	} // End Plot Source

	dF.renderImGuiSettings();

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
			setFresh(true);
			isRecording = true;
		}
	}
	*/

	if (ImGui::Button("Export Vector")) {
		saveVector = true;
		setFresh(true);
	}
	ImGui::SameLine();
	if (ImGui::Button("Export PNG")) {
		savePixels = true;
		setFresh(true);
	}

	ImGui::SameLine();
	ImGui::Checkbox("Add Timestamp", &bExportWithTimeStamp);

	ImGui::Unindent();
	ImGui::Separator(); // End Size 
}

void Canvas::loadSettings(ofJson& settings) {
	canvasTitle  = settings.value("canvasTitle", canvasTitle);
	canvasWidth  = settings.value("canvasWidth", canvasWidth);
	canvasHeight = settings.value("canvasHeight", canvasHeight);
	canvasTitle  = settings.value("canvasTitle", canvasTitle);
	bExportWithTimeStamp = settings.value("exportWithTimeStamp", bExportWithTimeStamp);

	c_canvas.x = settings["colors"]["plotcanvas"].value("r", c_canvas.x);
	c_canvas.y = settings["colors"]["plotcanvas"].value("g", c_canvas.y);
	c_canvas.z = settings["colors"]["plotcanvas"].value("b", c_canvas.z);
	c_canvas.w = settings["colors"]["plotcanvas"].value("a", c_canvas.w);

	dF.loadSettings(settings.value("drawFilters", ofJson::array()));

	resizeRequest = true;
	setFresh(true);
}

ofJson Canvas::getSettings() {
	ofJson settings;

	settings["canvasTitle"] = canvasTitle;
	settings["canvasWidth"] = canvasWidth;
	settings["canvasHeight"] = canvasHeight;
	settings["canvasTitle"] = canvasTitle;
	settings["exportWithTimeStamp"] = bExportWithTimeStamp;

	settings["colors"]["plotcanvas"]["r"] = c_canvas.x;
	settings["colors"]["plotcanvas"]["g"] = c_canvas.y;
	settings["colors"]["plotcanvas"]["b"] = c_canvas.z;
	settings["colors"]["plotcanvas"]["a"] = c_canvas.w;

	settings["drawFilters"] = dF.getSettings();

	return settings;
}

void Canvas::setup(string canvas_title) {
	pixelplotter = (ofApp*)ofGetAppPtr();
	canvasTitle = canvas_title;
	sourceController.setup();
	canvasFbo.allocate(canvasWidth, canvasHeight, GL_RGBA, 8);
	canvasFbo.getTexture().setTextureMinMagFilter(GL_NEAREST, GL_NEAREST);
	dF.addRandom();
}

void Canvas::setDimensions(ofImage* img) {
	canvasWidth = img->getWidth();
	canvasHeight = img->getHeight();
	resizeRequest = true;
}

void Canvas::setDimensions(float w, float h) {
	canvasWidth = w;
	canvasHeight = h;
	resizeRequest = true;
}

bool Canvas::isFresh() {
	return bFresh;
}

void Canvas::setFresh(bool fresh) {
	bFresh = fresh;
}

void Canvas::update() {
	sourceController.update();
	dF.update();

	if (dF.isFresh()) {
		dF.setFresh(false);
		setFresh(true);
	}

	if (resizeRequest) {

		canvasFbo.allocate(canvasWidth, canvasHeight, GL_RGBA, 8);
		canvasFbo.getTexture().setTextureMinMagFilter(GL_NEAREST, GL_NEAREST);

		resizeRequest = false;
		setFresh(true);

	}

	/*
	if (isRecording) {
		setFresh(true);
		return;
	}
	*/

	if (isFresh() || sourceController.frameBuffer.isFresh()) {
		updateFbo(&sourceController.frameBuffer.getFrame());
	}
	// TODO: Update() always returns freshness?
	// return isFresh();
}

void Canvas::updateFbo(ofImage* img) {
	canvasFbo.begin();
	if (saveVector) {
		// This swaps out the gl renderer for the Ciaro renderer
		string stamp = to_string(++exportCount);
		if (bExportWithTimeStamp) {
			stamp = ofGetTimestampString();
		}
		ofBeginSaveScreenAsPDF("export//" + canvasTitle + "_" + stamp + ".pdf", false);
	}

	ofClear(c_canvas);

	dF.draw(img, canvasWidth, canvasHeight);

	if (saveVector) {
		ofEndSaveScreenAsPDF();
		saveVector = false;
	}

	canvasFbo.end();

	if (isRecording || savePixels) {
		canvasFbo.readToPixels(canvasPix);
	}

	if (isRecording) {
		ofSaveImage(canvasPix, "export//frames//" + canvasTitle + "_" + ofx2d::with_leading_zero(++recFrameCount, 8) + ".png");
	}
	if (savePixels) {
		string stamp = to_string(++exportCount);
		if (bExportWithTimeStamp) {
			stamp = ofGetTimestampString();
		}
		ofSaveImage(canvasPix, "export//" + canvasTitle + "_" + stamp + ".png");
		savePixels = false;
	}

	setFresh(true);
};

void Canvas::draw(float x, float y, float w, float h) {
	canvasFbo.draw(x, y, w, h);
	setFresh(false);
}
