#pragma once

#include "drawFilter.h"

class Df_mesh : public DrawFilter {
public:
	Df_mesh() {
		pixelplotter = (ofApp*)ofGetAppPtr();
		name = "Mesh";
	};

	Df_mesh(ofxXmlSettings settings) {
		Df_mesh();
		loadSettings(settings);
	};

	virtual void draw(ofImage* input, float width = 0, float height = 0, float x = 0, float y = 0);
	virtual void renderImGuiSettings();
	virtual void loadSettings(ofxXmlSettings settings);
	virtual ofxXmlSettings getSettings();

private:
	ofMesh mesh;
	float rotationOffset = 0;
	ofVec3f getMeshBounds(const ofMesh& mesh);
};
