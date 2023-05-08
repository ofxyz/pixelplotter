#pragma once
#include "canvas.h"
#include "ofApp.h"

void Canvas::renderImGuiSettings() {
	ImGui::Indent();

	ImGui::InputText("Title", &canvasTitle);

	if (ImGui::ColorEdit4("Canvas Colour", (float*)&c_canvas, ImGuiColorEditFlags_NoInputs)) {
		setFresh(true);
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
	ImGui::SameLine();
	if (ImGui::Button("Fit")) {
		canvasWidth = screenW;
		canvasHeight = screenH;
		pixelplotter->resetZoom();
		pixelplotter->resetImageOffset();
		resizeRequest = true;
	}
	ImGui::PopItemWidth();
	ImGui::Separator(); // End Size 

	// Start DrawFilters
	//----------------------------------------------------------------------------------------------------------------------
	string sDrawFilterCount = "Plotters (" + ofToString(dF.v_DrawFilters.size()) + ")###DrawFiltersHolder";
	if (ImGui::CollapsingHeader(sDrawFilterCount.c_str()))
	{
		ImGui::PushStyleColor(ImGuiCol_Header, (ImVec4)ImColor::HSV(0, 0, 0.2));
		ImGui::PushStyleColor(ImGuiCol_HeaderActive, (ImVec4)ImColor::HSV(0, 0, 0.4));
		ImGui::PushStyleColor(ImGuiCol_HeaderHovered, (ImVec4)ImColor::HSV(0, 0, 0.7));

		ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(0, 0, 0.2));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(0, 0, 0.2));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(0, 0, 0.7));

		ImGui::PushStyleColor(ImGuiCol_FrameBg, (ImVec4)ImColor::HSV(0, 0, 0.2));
		ImGui::PushStyleColor(ImGuiCol_FrameBgActive, (ImVec4)ImColor::HSV(0, 0, 0.4));
		ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, (ImVec4)ImColor::HSV(0, 0, 0.5));

		ImGui::PushStyleColor(ImGuiCol_CheckMark, (ImVec4)ImColor::HSV(0, 0, 0.8));

		cleanDrawFilters = false;
		reorderDrawFilters = false;
		for (int i = 0; i < dF.v_DrawFilters.size(); i++) {
			ImGui::PushID(i);
			if (dF.v_DrawFilters[i]->active) {
				ImGui::Indent();
				dF.v_DrawFilters[i]->renderImGuiSettings();
				ImGui::Unindent();
			}
			else {
				cleanDrawFilters = true;
			}
			if (dF.v_DrawFilters[i]->moveUp || dF.v_DrawFilters[i]->moveDown) {
				reorderDrawFilters = true;
			}
			ImGui::PopID();
		}

		ImGui::PopStyleColor(10);

		ImGui::Indent(); // ADD PLOTTERS
		if (ofxImGui::VectorCombo("##Draw Filter Selector", &currentDrawFilterIndex, dF.v_DrawFilterNames))
		{
			dF.addFilter(currentDrawFilterIndex);
			currentDrawFilterIndex = 0;
		}
		ImGui::Unindent();

	}// End Draw Filters
	//----------------------------------------------------------------------------------------------------------------------

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

void Canvas::loadSettings(ofxXmlSettings settings) {
	canvasTitle = settings.getValue("canvasTitle", canvasTitle);
	canvasWidth = settings.getValue("canvasWidth", canvasWidth);
	canvasHeight = settings.getValue("canvasHeight", canvasHeight);
	canvasTitle = settings.getValue("canvasTitle", canvasTitle);
	bExportWithTimeStamp = settings.getValue("exportWithTimeStamp", bExportWithTimeStamp);

	c_canvas.x = settings.getValue("colors:plotCanvas:r", c_canvas.x);
	c_canvas.y = settings.getValue("colors:plotCanvas:g", c_canvas.y);
	c_canvas.z = settings.getValue("colors:plotCanvas:b", c_canvas.z);
	c_canvas.w = settings.getValue("colors:plotCanvas:a", c_canvas.w);

	resizeRequest = true;
	setFresh(true);
}

ofxXmlSettings Canvas::getSettings() {
	ofxXmlSettings settings;
	settings.setValue("canvasTitle", canvasTitle);
	settings.setValue("canvasWidth", canvasWidth);
	settings.setValue("canvasHeight", canvasHeight);
	settings.setValue("canvasTitle", canvasTitle);
	settings.setValue("exportWithTimeStamp", bExportWithTimeStamp);

	settings.setValue("colors:plotCanvas:r", c_canvas.x);
	settings.setValue("colors:plotCanvas:g", c_canvas.y);
	settings.setValue("colors:plotCanvas:b", c_canvas.z);
	settings.setValue("colors:plotCanvas:a", c_canvas.w);

	return settings;
}

void Canvas::setup(ofApp* app, ofImage* img, string canvas_title) {
	pixelplotter = app;
	dF = DrawFilterController(pixelplotter);
	canvasTitle  = canvas_title;
	setSourceDimension(img);
	canvasFbo.allocate(canvasWidth, canvasHeight, GL_RGBA, 8);
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
	dF.update();
	if (cleanDrawFilters) {
		dF.cleanFilters();
	}
	if (reorderDrawFilters) {
		dF.reorder();
	}
	if (dF.isFresh()) {
		dF.setFresh(false);
		setFresh(true);
	}

	if (resizeRequest) {
		canvasFbo.allocate(canvasWidth, canvasHeight, GL_RGBA, 8);
		canvasFbo.getTextureReference().setTextureMinMagFilter(GL_NEAREST, GL_NEAREST);
		resizeRequest = false;
		setFresh(true);
		return;
	}
	if (isRecording) {
		setFresh(true);
		return;
	}
}

void Canvas::update(ofImage* img) {
	update();
	canvasFbo.begin();
	if (saveVector) {
		// This swaps out the gl renderer for the ciaro renderer
		string stamp = to_string(++exportCount);
		if (bExportWithTimeStamp) {
			stamp = ofGetTimestampString();
		}
		ofBeginSaveScreenAsPDF("export//" + canvasTitle + "_" + stamp + ".pdf", false);
	}

	ofClear(c_canvas);

	// This needs to be a filter controller update
	for (const auto& filter : dF.v_DrawFilters) {
		// update one filter per frame to keep things speeedy?
		// Each filter draws to it's own fbo and are drawn here?
		// filter->update(img); filter->update(settings)
		filter->draw(img, canvasWidth, canvasHeight);
	}
	dF.setFresh(false); // See

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
		string stamp = to_string(++exportCount);
		if (bExportWithTimeStamp) {
			stamp = ofGetTimestampString();
		}
		ofSaveImage(canvasPix, "export//" + canvasTitle + "_" + stamp + ".png");
		savePixels = false;
	}

	setFresh(false);
};
