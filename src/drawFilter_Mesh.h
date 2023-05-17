#pragma once

#include "drawFilter.h"

class Df_mesh : public DrawFilter {
public:
	ofApp* pixelplotter;
	Df_mesh(ofApp* app) {
		pixelplotter = app;
		name = "Mesh";
	};

	void draw(ofImage* input, float width = 0, float height = 0, float x = 0, float y = 0);
	void renderImGuiSettings();
	virtual void loadSettings(ofxXmlSettings settings);
	ofxXmlSettings getSettings();

private:
	ofMesh mesh;
	bool nineOne = false; // Safe 9 or 1 points per pixel
};
