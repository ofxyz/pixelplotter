#pragma once
#include "drawFilter_Controller.h"
#include "ofx2d.h"

class ofApp;

class Canvas {
public:
	ofApp* pixelplotter;
	ofCamera cam;
	DrawFilterController dF;
	ofFbo canvasFbo;
	ofPixels canvasPix;
	ImVec4 c_canvas = ofColor(255, 255, 255, 255);
	string canvasTitle = "Untitled";
	int exportCount = 0;
	int sourceWidth = 640;
	int sourceHeight = 480;
	int canvasWidth = 640;
	int canvasHeight = 480;
	bool saveVector = false;
	bool savePixels = false;
	bool fresh = false;
	bool isRecording = false;
	bool resizeRequest = false;
	int recFrameCount = 0;

	bool isFresh() {
		return fresh;
	}

	void renderImGuiSettings();
	void loadSettings(ofxXmlSettings settings);
	ofxXmlSettings getSettings();

	std::string with_leading_zero(int value, int width)
	{
		std::ostringstream oss;
		oss << std::setw(width) << std::setfill('0') << value;
		return oss.str();
	}

	void setup(ofApp* app, ofImage* img, string canvas_title = "Untitled");
	void update();
	void update(ofImage* img);
	void draw(float x, float y, float w, float h) {
		canvasFbo.draw(x, y, w, h);
		fresh = false;
	};

	void setSourceDimension();
	void setSourceDimension(ofImage* img);
	void setSourceDimension(float w, float h) {
		sourceWidth = w;
		sourceHeight = h;
		setSourceDimension();
	}
};