#include "ofApp.h"
#include "drawFilter_Lumes.h"

ofJson Df_lumes::getSettings() {
	ofJson settings;
	settings["name"] = name;
	settings["_isOpen"] = _isOpen;
	settings["tilesX"] = tilesX;
	settings["tilesY"] = tilesY;
	return settings;
}

void Df_lumes::loadSettings(ofJson& settings) {
	try {
		_isOpen = settings.value("_isOpen", _isOpen);
		tilesX = settings.value("tilesX", 64);
		tilesY = settings.value("tilesY", 64);
	}
	catch (...) {
		ofLogNotice() << "Failed to load settings Df_lumes";
	}
	return;
}

void Df_lumes::renderImGuiSettings() {
	ImGui::SetNextItemOpen(_isOpen, ImGuiCond_Once);
	if (ImGui::CollapsingHeader(name.c_str(), &bAlive)) {
		_isOpen = true;
		ImGui::AlignTextToFramePadding();
		renderUpDownButtons();

		ImGui::PushItemWidth(60);

		ImGui::Text("Tiles"); ImGui::SameLine(75);
		if (ImGui::DragInt("X ##lumes_tiles", &tilesX, 1, 1, 1200)) {
			setFresh(true);
		}
		ImGui::SameLine();
		if (ImGui::DragInt("Y ##lumes_tiles", &tilesY, 1, 1, 1200)) {
			setFresh(true);
		}

		ImGui::Text("kiteShape"); ImGui::SameLine(75);
		if (ImGui::DragFloat("% ##lumes_percent", &centerPercent, 0.01f, 0, 1, "%.2f")) {
			setFresh(true);
		}
		/*
		ImGui::Text("Rotation"); ImGui::SameLine(75);
		if (ImGui::DragFloat("Degrees ##lumes_rotDeg", &rotDeg, 0.1f, 0, 360, "%.1f")) {
			setFresh(true);
		}
		*/

	} else {
		_isOpen = false;
	}
}

void Df_lumes::drawKite(ofColor c, float offsetX, float offsetY, float width, float height, float centerPercent, glm::ivec2 pos) {

	/*
	if (rotDeg > 0) {
		float otherAngle = 180 - (rotDeg + 90);
		float hypo = height / sin(otherAngle);
		float base = height * tan(rotDeg);
		height = hypot(height, base);
	}
	*/

	// Interlocking shapes
	offsetX = offsetX - (width * 0.5);
	width *= 2;

	// Draw from intersect center makes angles easier on rotation
	float vertCenter = centerPercent * height;
	float topY = -vertCenter;
	float botY = height - vertCenter;
	float rightX = width * 0.5;
	float leftX  = -rightX;

	ofPath s;
	s.moveTo(0, topY);
	s.lineTo(rightX, 0);
	s.lineTo(0, botY);
	s.lineTo(leftX, 0);
	s.close();
	s.setFillColor(c);

	ofPushMatrix();
	
	// XY offset for rotating kite
	//float topx = vertCenter * sin(PI * 2 * rotDeg / 360);
	//float topy = vertCenter * cos(PI * 2 * rotDeg / 360);
	//float rightx = rightX * sin(PI * 2 * rotDeg / 360);
	//float righty = rightX * cos(PI * 2 * rotDeg / 360);
	//float botx = botY * sin(PI * 2 * rotDeg / 360);
	//float boty = botY * cos(PI * 2 * rotDeg / 360);

	float diffY = (height * 0.5) - abs(topY);
	if (pos.x % 2 == 0) {
		ofTranslate(offsetX, offsetY);
		ofRotateDeg(rotDeg);
		ofTranslate(0, -diffY);
		s.draw();
	}
	else {
		ofTranslate(-offsetX, -offsetY);
		ofTranslate(-(width*0.5), -(height * 0.5));
		ofRotateDeg(180 + rotDeg);
		//ofTranslate(-topx, (topy- vertCenter)*2);
		s.draw();
	}

	ofPopMatrix();
}

void Df_lumes::draw(ofImage* input, float width, float height) {
	setFresh(false);
	if (!bVisible) return;

	int imgW = input->getWidth();
	int imgH = input->getHeight();

	float tileW = (float)imgW / (float)tilesX;
	float tileH = (float)imgH / (float)tilesY;
	float halfTileW = tileW * 0.5;
	float halfTileH = tileH * 0.5;

	glm::ivec2 pos(0,0);

	for (float y = 0; y <= imgH; y += tileH) {

		for (float x = 0; x <= imgW; x += tileW) {
			
			float fx = x + halfTileW;
			float fy = y + halfTileH;
			ofColor c = input->getPixels().getColor(ofClamp(fx,0,imgW-1), ofClamp(fy,0,imgH-1));

			// Draw Kite
			ofPushMatrix();
			ofTranslate(fx, fy);
			drawKite(c, 0, 0, tileW, tileH, centerPercent, pos );
			ofPopMatrix();
			pos.x++;
		}
		pos.x = 0;
		pos.y++;
	}
}
