#include "ofApp.h"
#include "drawFilter_Mesh.h"

ofxXmlSettings Df_mesh::getSettings() {
	ofxXmlSettings settings;
	settings.setValue("name", name);
	settings.setValue("rotationOffset", rotationOffset);

	return settings;
}


void Df_mesh::loadSettings(ofxXmlSettings settings) {
	name = settings.getValue("name", name);
	rotationOffset = settings.getValue("rotationOffset", rotationOffset);

	return;
}

void Df_mesh::renderImGuiSettings() {
	if (ImGui::CollapsingHeader(name.c_str(), &active)) {
		ImGui::AlignTextToFramePadding();

		renderUpDownButtons();

		if (ImGui::DragFloat("Rotation ##Df_mesh", &rotationOffset, 0.1f, -360.0f, 360.0f, "%.3f")) {
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
	int numVerts = mesh.getNumVertices();
	ofVec3f meshCentroid = mesh.getCentroid();
	ofVec3f topLeft = mesh.getVertex(0);
	float maxDist = topLeft.distance(meshCentroid);

	for (int i = 0; i < numVerts; ++i) {
		ofVec3f vert = mesh.getVertex(i);
		float distance = vert.distance(meshCentroid);
		float angle = atan2(vert.y - meshCentroid.y, vert.x - meshCentroid.x);
		//float angle = meshCentroid.angle(vert);
		float speed = ofMap(distance, 0, maxDist, 0, rotationOffset, true);
		float rotatedAngle = speed + angle;

		vert.x = distance * cos(rotatedAngle) + meshCentroid.x;
		vert.y = distance * sin(rotatedAngle) + meshCentroid.y;

		mesh.setVertex(i, vert);
	}

	/*
	ofVec3f bounds = getMeshBounds(mesh);
	ofVec3f meshCenter = mesh.getCentroid();

	for (int i = 0; i < numVerts; ++i) {
		ofVec3f vert = mesh.getVertex(i);
		if(vert.x > meshCenter.x){
			vert.x = ofMap(vert.x, meshCenter.x, bounds.x, width*0.5, width - centerOffsetW);
		} else {
			vert.x = ofMap(vert.x, 0, meshCenter.x, 0, width * 0.5);
		}

		if (vert.y > meshCenter.y) {
			vert.y = ofMap(vert.y, meshCenter.y, bounds.y, height * 0.5, height);
		}
		else {
			vert.y = ofMap(vert.y, 0, meshCenter.y, 0, height * 0.5);
		}
		
		mesh.setVertex(i, vert);
	}
	*/

	ofPushStyle();
	ofFill();

	for (int v = 0; v < mesh.getNumVertices(); v++) {
		ofVec3f mv = mesh.getVertex(v);
		ofColor vc = mesh.getColor(v);
		ofSetColor(vc);
		ofDrawRectangle(x+mv.x- centerOffsetW, y+ mv.y- centerOffsetH, 1+centerOffsetW*2, 1+centerOffsetH * 2);	
	}
	ofPopStyle();

	//mesh.draw();
}

ofVec3f Df_mesh::getMeshBounds(const ofMesh& mesh) {

	auto xExtremes = minmax_element(mesh.getVertices().begin(), mesh.getVertices().end(),
		[](const ofPoint& lhs, const ofPoint& rhs) {
			return lhs.x < rhs.x;
		});
	auto yExtremes = minmax_element(mesh.getVertices().begin(), mesh.getVertices().end(),
		[](const ofPoint& lhs, const ofPoint& rhs) {
			return lhs.y < rhs.y;
		});
	auto zExtremes = minmax_element(mesh.getVertices().begin(), mesh.getVertices().end(),
		[](const ofPoint& lhs, const ofPoint& rhs) {
			return lhs.z < rhs.z;
		});
	return ofVec3f(xExtremes.second->x - xExtremes.first->x,
		yExtremes.second->y - yExtremes.first->y,
		zExtremes.second->z - zExtremes.first->z);
}