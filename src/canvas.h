#pragma once
 
#include "drawFilter_Controller.h"
#include "source_Controller.h"

class Canvas {
public:
	Canvas();
	ofApp* pixelplotter;
	
	ofFbo canvasFbo;
	ofPixels canvasPix;
	ofCamera cam;
	ImVec4 c_canvas = ofColor(255, 255, 255, 255);
	std::string canvasTitle = "Untitled";

	DrawFilterController dF;
	SourceController sourceController;
	
	// TODO: These should be in DrawFilterController / SourceController
	// Need ImGui Same as sourceController
	int currentImageFilterIndex = 0;

	void renderImGuiSettings();
	void loadSettings(ofJson& settings);
	ofJson getSettings();

	void setup(string canvas_title = "Untitled");
	void update();
	void updateFbo(ofImage* img);
	void draw(float x, float y, float w, float h) {
		canvasFbo.draw(x, y, w, h);
		setFresh(false);
	}

	void setDimensions(ofImage* img);
	void setDimensions(float w, float h);

	int canvasWidth = 640;
	int canvasHeight = 480;

	bool isRecording = false;
	bool resizeRequest = false;

	// TODO: Wrap these in functions and make private?
	bool saveVector = false;
	bool savePixels = false;

	bool isFresh() {
		return bFresh;
	}
	void setFresh(bool fresh) {
		bFresh = fresh;
	}

private:
	bool bFresh = false;
	bool redrawFBO = false;
	bool bExportWithTimeStamp = true;
	int exportCount = 0;
	int recFrameCount = 0;
};
