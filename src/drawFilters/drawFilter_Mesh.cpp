#include "ofApp.h"
#include "drawFilter_Mesh.h"
#include "ImGui_General.h"

ofJson Df_mesh::getSettings() {
	ofJson settings;
	settings["name"] = name;
	settings["_isOpen"] = _isOpen;
	settings["pixelType"] = drawPixels.v_objectNames[selectedPixelType];
	settings["rotationOffset"] = rotationOffset;

	return settings;
}

void Df_mesh::loadSettings(ofJson& settings) {
	try{
		//name = settings.value("name", name);
		_isOpen = settings.value("_isOpen", _isOpen);
		selectedPixelType = ofx2d::getIndex(drawPixels.v_objectNames, settings.value("pixelType", "Undefined"), selectedPixelType);
		rotationOffset = settings.value("rotationOffset", rotationOffset);
	}
	catch (...) {
		ofLogNotice() << "Failed to load settings Df_mesh";
	}
	return;
}

void Df_mesh::renderImGuiSettings() {
	ImGui::SetNextItemOpen(_isOpen, ImGuiCond_Once);
	if (ImGui::CollapsingHeader(name.c_str(), &bAlive)) {
		_isOpen = true;
		ImGui::AlignTextToFramePadding();

		renderUpDownButtons();

		ImGui::PushItemWidth(60);
		ImGui::Text("Tiles"); ImGui::SameLine(75);
		if (ImGui::DragInt("X ##pixelate2_tiles", &tilesX, 1, 1, 1200)) {
			setFresh(true);
		}
		ImGui::SameLine();
		if (ImGui::DragInt("Y ##pixelate2_tiles", &tilesY, 1, 1, 1200)) {
			setFresh(true);
		}
		ImGui::SameLine();
		if (ImGui::Checkbox("Mirror", &pixelMirror)) {
			setFresh(true);
		}
		ImGui::PopItemWidth();

		if (ImGui::DragFloat("Rotation ##Df_mesh", &rotationOffset, 0.1f, -360.0f, 360.0f, "%.3f")) {
			setFresh(true);
		}

		if (ofxImGui::VectorCombo("Pixel Type ##pixelate2", &selectedPixelType, drawPixels.v_objectNames)) {
			setFresh(true);
		};

		// Pass fresh through
		if (drawPixels.v_Objects[selectedPixelType]->isFresh()) {
			drawPixels.v_Objects[selectedPixelType]->setFresh(false);
			setFresh(true);
		}

		drawPixels.v_Objects[selectedPixelType]->renderImGuiSettings();
	} else {
		_isOpen = false;
	}
	ImGui::PopID();
}

Df_mesh::Df_mesh()
{
	pixelplotter = (ofApp*)ofGetAppPtr();
	name = "Mesh";
	selectedPixelType = 0;
	tilesX = 64;
	tilesY = 64;
}

Df_mesh::Df_mesh(ofJson& settings)
	: Df_mesh()
{
	loadSettings(settings);
}

void Df_mesh::draw(ofImage* input, float width, float height) {
	setFresh(false);
	if (!bVisible) return;

	// This is too slow for real time graphics.
	// Add update()
	// Set Mesh Division.
	mesh.clear();
	mesh.setMode(OF_PRIMITIVE_POINTS);
	//mesh.setMode(OF_PRIMITIVE_TRIANGLES);
	// Quads are not supported any longer we can build from triangles ...

	mesh.enableColors();

	int imgW = input->getWidth();
	int imgH = input->getHeight();

	float tileW = (float)imgW / (float)tilesX;
	float tileH = (float)imgH / (float)tilesY;
	float halfTileW = tileW * 0.5;
	float halfTileH = tileH * 0.5;

	for (float y = 0; y < imgH - halfTileH; y += tileH) {
		for (float x = 0; x < imgW - halfTileW; x += tileW) {
			ofColor c = input->getColor(floor(x+ halfTileW), floor(y+ halfTileH));
			glm::vec3 pos(ofMap(x+ halfTileW, 0, imgW, 0, width), ofMap(y+ halfTileH, 0, imgH, 0, height), 0.0);
			mesh.addVertex(pos);
			mesh.addColor(c);
		}
	}

	/*
	float centerOffsetW = ((width - imgW) / imgW) * 0.5;
	float centerOffsetH = ((height - imgH) / imgH) * 0.5;

	for (int x = 0; x < imgW; x++) {
		for (int y = 0; y < imgH; y++) {
			ofColor c = input->getColor(x, y);
			glm::vec3 pos(ofMap(x, 0, imgW, 0, width) + centerOffsetW, ofMap(y, 0, imgH, 0, height) + centerOffsetH, 0.0);
			mesh.addVertex(pos);
			mesh.addColor(c);
		}
	}
	*/

	// Transform
	// We should make a skewed pixel implementation so we can keep the mesh right
	size_t numVerts = mesh.getNumVertices();
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

	for (int v = 0; v < mesh.getNumVertices(); v++) {
		ofVec3f mv = mesh.getVertex(v);
		ofColor vc = mesh.getColor(v);
		ofPushMatrix();
		ofTranslate(mv.x, mv.y);

		drawPixels.v_Objects[selectedPixelType]->draw(vc, { tileW, tileH });
		
		ofPopMatrix();
	}

	//mesh.draw();
	//mesh.drawFaces();
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
