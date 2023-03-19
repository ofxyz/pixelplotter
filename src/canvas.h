#pragma once

#include "drawFilter_Controller.h"

class Canvas {
public:
	ofCamera cam;
	DrawFilterController dF;
	ofFbo canvasFbo;
	ImVec4 c_canvas = ofColor(255, 255, 255, 255);
	string canvasTitle = "Untitled";
	int zoomMultiplier = 4;
	int exportCount = 0;
	int canvasWidth = 640;
	int canvasHeight = 480;
	bool saveVector = false;
	bool fresh = false;

	void loadSettings(ofxXmlSettings settings) {
		canvasTitle = settings.getValue("canvasTitle", canvasTitle);
		canvasWidth = settings.getValue("canvasWidth", canvasWidth);
		canvasHeight = settings.getValue("canvasHeight", canvasHeight);

		zoomMultiplier = settings.getValue("zoomMultiplier", zoomMultiplier);

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
		settings.setValue("zoomMultiplier", zoomMultiplier);

		settings.setValue("colors:canvas:r", c_canvas.x);
		settings.setValue("colors:canvas:g", c_canvas.y);
		settings.setValue("colors:canvas:b", c_canvas.z);
		settings.setValue("colors:canvas:a", c_canvas.w);

		return settings;
	}

	void setup(ofImage* img, string canvas_title = "Untitled") {
		canvasTitle  = canvas_title;
		canvasWidth  = img->getWidth() * zoomMultiplier;
		canvasHeight = img->getHeight() * zoomMultiplier;
		canvasFbo.allocate(canvasWidth, canvasHeight, GL_RGB, 8);
		canvasFbo.getTextureReference().setTextureMinMagFilter(GL_NEAREST, GL_NEAREST);
	}

	void update(ofImage* img) {
		canvasFbo.begin();
		if (saveVector) {
			ofBeginSaveScreenAsPDF("export//" + canvasTitle + "_" + to_string(++exportCount) + ".pdf", false);
		}

		ofClear(c_canvas);

		for (const auto& filter : dF.v_DrawFilters) {
			filter->draw(img); // update one filter per frame to keep things speeedy? Seperate Export function?
		}

		if (saveVector) {
			ofEndSaveScreenAsPDF();
			saveVector = false;
		}

		canvasFbo.end();
		fresh = true;
	}

	void draw(float x, float y, float w, float h) {
		canvasFbo.draw(x, y, w, h);
		fresh = false;
	};
};