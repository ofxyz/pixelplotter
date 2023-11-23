#pragma once
#include "ofFbo.h"
#include "ofCamera.h"
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

	void renderImGuiSettings();
	void loadSettings(ofJson& settings);
	ofJson getSettings();

	void setup(std::string canvas_title = "Untitled");
	void update();
	void updateFbo(ofImage* img);
	void draw(float x, float y, float w, float h);

	void setDimensions(ofImage* img);
	void setDimensions(float w, float h);

	int canvasWidth = 640;
	int canvasHeight = 480;
	bool isRecording = false;
	bool resizeRequest = false;
	bool saveVector = false;
	bool savePixels = false;

	bool isFresh();
	void setFresh(bool fresh);

private:
	bool bFresh = false;
	bool redrawFBO = false;
	bool bExportWithTimeStamp = true;
	int exportCount = 0;
	int recFrameCount = 0;
};
