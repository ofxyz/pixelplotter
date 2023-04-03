#pragma once

#include "drawFilter_Controller.h"
#include "ofx2d.h"

class Canvas {
public:
	ofCamera cam;
	DrawFilterController dF;
	ofFbo canvasFbo;
	ofPixels canvasPix;
	ImVec4 c_canvas = ofColor(255, 255, 255, 255);
	string canvasTitle = "Untitled";
	int exportCount = 0;
	int canvasWidth = 640;
	int canvasHeight = 480;
	bool saveVector = false;
	bool savePixels = false;
	bool fresh = false;
	bool isRecording = false;
	int recFrameCount = 0;

	void renderImGuiSettings() {
		ImGui::ColorEdit4("Canvas Colour", (float*)&c_canvas, ImGuiColorEditFlags_NoInputs);

		if (isRecording) {
			if (ImGui::Button("Stop Recording"))
			{
				isRecording = false;
			}
		}
		else {
			if (ImGui::Button("Start Recoding"))
			{
				fresh = true;
				isRecording = true;
			}
		}

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

	bool isFresh() {
		return fresh;
	}

	std::string with_leading_zero(int value, int width)
	{
		std::ostringstream oss;
		oss << std::setw(width) << std::setfill('0') << value;
		return oss.str();
	}

	void loadSettings(ofxXmlSettings settings) {
		canvasTitle = settings.getValue("canvasTitle", canvasTitle);
		canvasWidth = settings.getValue("canvasWidth", canvasWidth);
		canvasHeight = settings.getValue("canvasHeight", canvasHeight);

		c_canvas.x = settings.getValue("colors:canvas:r", c_canvas.x);
		c_canvas.y = settings.getValue("colors:canvas:g", c_canvas.y);
		c_canvas.z = settings.getValue("colors:canvas:b", c_canvas.z);
		c_canvas.w = settings.getValue("colors:canvas:a", c_canvas.w);
	}

	ofxXmlSettings getSettings() {
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

	void setup(ofImage* img, string canvas_title = "Untitled") {
		canvasTitle  = canvas_title;
		canvasWidth  = img->getWidth();
		canvasHeight = img->getHeight();
		canvasFbo.allocate(canvasWidth, canvasHeight, GL_RGB, 8);
		canvasFbo.getTextureReference().setTextureMinMagFilter(GL_NEAREST, GL_NEAREST);
		update(img);
	}

	void update() {
		if (isRecording) fresh = true;
		for (const auto& filter : dF.v_DrawFilters) {
			if (filter->isFresh()) {
				fresh = true;
				return;
			}
		}
	}

	void update(ofImage* img) {
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
			filter->draw(img);
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
			ofSaveImage(canvasPix, "export//frames//" + canvasTitle + "_" + with_leading_zero(++recFrameCount,8) +".png");
		}
		if (savePixels) {
			ofSaveImage(canvasPix, "export//" + canvasTitle + "_" + to_string(++exportCount) + ".png");
			savePixels = false;
		}

		fresh = true;
	}

	void draw(float x, float y, float w, float h) {
		canvasFbo.draw(x, y, w, h);
		fresh = false;
	};
};