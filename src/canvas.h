#pragma once
#include "drawFilter_Controller.h"
#include "sourceController.h"
#include "ofx2d.h"

class ofApp;

class Canvas {
public:
	ofApp* pixelplotter;
	ofCamera cam;
	DrawFilterController dF;
	SourceController sourceController;
	ofFbo canvasFbo;
	ofPixels canvasPix;
	ImVec4 c_canvas = ofColor(255, 255, 255, 255);
	std::string canvasTitle = "Untitled";
	int currentDrawFilterIndex = 0;
	int currentImageFilterIndex = 0;
	int exportCount = 0;
	int sourceWidth = 640;
	int sourceHeight = 480;
	int canvasWidth = 640;
	int canvasHeight = 480;
	bool saveVector = false;
	bool savePixels = false;
	bool isRecording = false;
	bool resizeRequest = false;
	bool cleanDrawFilters = false;
	bool reorderDrawFilters = false;
	bool bExportWithTimeStamp = true;
	int recFrameCount = 0;

	void renderImGuiSettings();
	void loadSettings(ofxXmlSettings settings);
	ofxXmlSettings getSettings();

	std::string with_leading_zero(int value, int width)
	{
		std::ostringstream oss;
		oss << std::setw(width) << std::setfill('0') << value;
		return oss.str();
	}

	void setup(ofApp* app, string canvas_title = "Untitled");
	void update();
	void update(ofImage* img);
	void draw(float x, float y, float w, float h) {
		canvasFbo.draw(x, y, w, h);
		setFresh(false);
	}

	void setSourceDimension();
	void setSourceDimension(ofImage* img);
	void setSourceDimension(float w, float h) {
		sourceWidth = w;
		sourceHeight = h;
		setSourceDimension();
	}

	bool isFresh() {
		return bFresh;
	}
	void setFresh(bool fresh) {
		bFresh = fresh;
	}

private:
	bool bFresh = false;
};
