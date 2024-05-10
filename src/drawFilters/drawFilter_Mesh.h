#pragma once

#include "drawFilter.h"
#include "vc_drawPixel.h"

class Df_mesh : public DrawFilter {
public:
	Df_mesh();
	Df_mesh(ofJson& settings);

	void draw(ofImage* input, float width = 0, float height = 0) override;
	void renderImGuiSettings() override;
	void loadSettings(ofJson& settings) override;
	ofJson getSettings() override;

private:
	bool pixelMirror;
	int tilesX;
	int tilesY;
	int selectedPixelType;
	DrawPixelController drawPixels;
	ofMesh mesh;
	float rotationOffset = 0;
	ofVec3f getMeshBounds(const ofMesh& mesh);
};
