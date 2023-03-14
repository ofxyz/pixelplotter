#pragma once

#include "drawFilter_Controller.h"

class Canvas {
public:
	DrawFilterController dF;
	ofFbo canvasFbo;
	ImVec4 c_canvas = ofColor(255, 255, 255, 255);
	string canvasTitle = "Untitled";
	int zoomMultiplier = 4;
	int exportCount = 0;
	int currentDrawFilterIndex = 0;
	bool saveVector = false;
	bool fresh = false;

	void setup(ofImage* img, string canvas_title = "Untitled") {
		canvasTitle = canvas_title;
		canvasFbo.allocate(img->getWidth() * zoomMultiplier, img->getHeight() * zoomMultiplier, GL_RGB, 8);
		canvasFbo.getTextureReference().setTextureMinMagFilter(GL_NEAREST, GL_NEAREST);
	}

	void update(ofImage* img) {
		canvasFbo.begin();
		if (saveVector) {
			ofBeginSaveScreenAsPDF("export//" + canvasTitle + "_" + to_string(++exportCount) + ".pdf", false);
		}

		ofClear(c_canvas);

		for (const auto& filter : dF.v_DrawFilters) {
			filter->draw(img); // update one filter per frame to keep is speeedy?
		}

		if (saveVector) {
			ofEndSaveScreenAsPDF();
			saveVector = false;
		}

		canvasFbo.end();
		fresh = true;
	}

	void draw(float x, float y, float w, float h) {
		if (fresh) canvasFbo.draw(x, y, w, h);
		fresh = false;
	};
};