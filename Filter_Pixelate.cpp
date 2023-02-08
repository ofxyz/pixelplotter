#include "ofApp.h"

void ofApp::Df_pixelate::renderImGuiSettings() {
	if (ImGui::CollapsingHeader(name.c_str(), &closable)) {

		ImGui::AlignTextToFramePadding();

		ImGui::PushItemWidth(100);

		ImGui::Text("Tiles"); ImGui::SameLine(75);
		ImGui::DragInt("X ##pixelate_tiles", &tilesX, 1, 1, 1200);
		ImGui::SameLine();
		ImGui::DragInt("Y ##pixelate_tiles", &tilesY, 1, 1, 1200);

		ImGui::Text("Addon"); ImGui::SameLine(75);
		ImGui::DragFloat("X ##pixelate_addon", &addonx, 0.1f, -100.0f, 100.0f, "%.3f");
		ImGui::SameLine();
		ImGui::DragFloat("Y ##pixelate_addon", &addony, 0.1f, -100.0f, 100.0f, "%.3f");

		ImGui::Text("Offset"); ImGui::SameLine(75);
		ImGui::DragFloat("Random ##pixelate_random", &randomOffset, 0.1f, 0.0f, 500.0f, "%.3f");

		ImGui::PopItemWidth();

		ImGui::Text("Rotation"); ImGui::SameLine(75);
		if (ofxImGui::VectorCombo("##pixelate_rotation", &ui_currentRotationMap, v_pixelDataMapOptions))
		{
			// Done
		}

		ImGui::PushItemWidth(100);

		if (ui_currentRotationMap > 0) {
			ImGui::Text(" "); ImGui::SameLine(75);
			ImGui::DragFloat("Min ##pixelate_rotation", &rotationMinMax[0], 0.1f, -360.0f, 360.0f, "%.3f");
			ImGui::SameLine();
			ImGui::DragFloat("Max ##pixelate_rotation", &rotationMinMax[1], 0.1f, -360.0f, 360.0f, "%.3f");
		}

		ImGui::Checkbox("Polka", &polka);
		ImGui::Checkbox("Round Pixels", &roundPixels); // TODO: Add force proportion?

		ImGui::PopItemWidth();
	}
}

void ofApp::Df_pixelate::drawPixel(float w, float h, ofColor c) {
	
	//if ((w > -0.25 && w < 0.25) || (h > -0.25 && h < 0.25)) {
	//	return;
	//}

	float offsetX = ofRandom(-randomOffset, randomOffset);
	float offsetY = ofRandom(-randomOffset, randomOffset);

	ofPushStyle();
	ofFill();
	ofSetColor(c);
	// Draw PixelTypes // RGB Seperation 01, 02, etc
	if (roundPixels) {
		ofDrawEllipse(offsetX, offsetY, w, h);
	}
	else { // Square Pixels
		ofDrawRectangle(-(w * 0.5) + offsetX, -(h * 0.5) + offsetY, w, h);
	}
	ofPopStyle();
};

float ofApp::Df_pixelate::getRotation(ofColor c, float x, float y) {
	if (v_pixelDataMapOptions[ui_currentRotationMap] == "Between") {
		return ofRandom(rotationMinMax[0], rotationMinMax[1]);
	}
	if (v_pixelDataMapOptions[ui_currentRotationMap] == "Color Lightness") {
		return ofMap(c.getLightness(), 0, 255, rotationMinMax[0], rotationMinMax[1]);
	}
	return 0;
}

void ofApp::Df_pixelate::draw(ofImage* input) {
	int imgW = input->getWidth();
	int imgH = input->getHeight();
	float tileW = (float)imgW / (float)tilesX;
	float tileH = (float)imgH / (float)tilesY;
	float halfTileW = tileW * 0.5;
	float halfTileH = tileH * 0.5;

	int ycount = 0;
	int xcount = 0;
	int ydiv   = 0;

	// This needs some control...
	//int rSeed = 0;

	for (float y = 0; y < imgH - halfTileH; y += tileH) {
		//rSeed += 100;
		//ofSeedRandom(rSeed++);
		(ycount % 2 == 0) ? ydiv = 0 : ydiv = 1;
		for (float x = 0; x < imgW - halfTileW; x += tileW) {
			//rSeed += 200;
			//ofSeedRandom(rSeed++);
			if (!polka || ((xcount + ydiv) % 2 == 0)) {
				float fx = x + halfTileW;
				float fy = y + halfTileH;
				ofColor c = input->getPixels().getColor(floor(fx), floor(fy));

				ofPushMatrix();
				ofTranslate(fx * drawScale, fy * drawScale, 0);
				if(rotationMapTo >= 0) {
					ofRotateZDeg(getRotation(c,x,y));
				}
				drawPixel( (tileW + addonx) * drawScale, (tileH + addony) * drawScale, c);
				ofPopMatrix();
			}
			xcount++;
		}
		ycount++;
		xcount = 0;
	}
}
