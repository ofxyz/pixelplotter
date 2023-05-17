#include "ofApp.h"
#include "drawFilter_Mesh.h"

ofxXmlSettings Df_mesh::getSettings() {
	ofxXmlSettings settings;
	settings.setValue("name", name);
	settings.setValue("nineOne", nineOne);

	return settings;
}


void Df_mesh::loadSettings(ofxXmlSettings settings) {
	name = settings.getValue("name", name);
	nineOne = settings.getValue("nineOne", nineOne);

	return;
}

void Df_mesh::renderImGuiSettings() {
	if (ImGui::CollapsingHeader(name.c_str(), &active)) {
		ImGui::AlignTextToFramePadding();

		renderUpDownButtons();

		if (ImGui::Checkbox("nineOne", &nineOne)) {
			setFresh(true);
		}
	}
}

void Df_mesh::draw(ofImage* input, float width, float height, float x, float y) {
	setFresh(false);
	if (!visible) return;
	mesh.clear();
	mesh.setMode(OF_PRIMITIVE_POINTS);
	//mesh.setMode(OF_PRIMITIVE_);
	// Quads are not suported any longer we can build from triangles ...
	// Main thing is to use it as more flexible data type for transformation

	mesh.enableColors();

	int imgW = input->getWidth();
	int imgH = input->getHeight();

	float centerOffsetW = ((width - imgW) / imgW) * 0.5;
	float centerOffsetH = ((height - imgH) / imgH) * 0.5;

	for (int _x = 0; _x < imgW; _x++) {
		for (int _y = 0; _y < imgH; _y++) {
			ofColor c = input->getColor(_x, _y);
			ofVec3f pos(ofMap(_x, 0, imgW, 0, width) + centerOffsetW, ofMap(_y, 0, imgH, 0, height) + centerOffsetH, 0.0);
			mesh.addVertex(pos);
			mesh.addColor(c);
		}
	}

	// Transform



	ofPushStyle();
	ofFill();

	for (int v = 0; v < mesh.getNumVertices(); v++) {
		auto mv = mesh.getVertex(v);
		ofColor vc = mesh.getColor(v);
		ofSetColor(vc);
		ofDrawRectangle(x+mv.x- centerOffsetW, y+ mv.y- centerOffsetH, 1+centerOffsetW*2, 1+centerOffsetH * 2);	
	}
	ofPopStyle();

	//mesh.draw();
}
