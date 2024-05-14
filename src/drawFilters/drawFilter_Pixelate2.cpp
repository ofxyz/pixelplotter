#include "drawFilter_Pixelate2.h"
#include "drawPixel.h"
#include "ofApp.h"
#include "ofx2d.h"
#include "ImGui_General.h"
#include "ImGui_Widget_Curve.h"

Df_pixelate2::Df_pixelate2()
{
	InitDefaults();
}

Df_pixelate2::Df_pixelate2(ofJson& settings)
	: Df_pixelate2()
{
	loadSettings(settings);
}

void Df_pixelate2::InitDefaults()
{
	pixelplotter = (ofApp*)ofGetAppPtr();
	name = "Pixelate2";
	selectedPixelType = 0;
	pixelMirror = false;
	tilesX = 64;
	tilesY = 64;
	useCurve = false;
	// init data so editor knows to take it from here
	xCurvePoints[0].x = ImGui::CurveTerminator;
	yCurvePoints[0].x = ImGui::CurveTerminator;
}

void Df_pixelate2::draw(ofImage* input, float width /*= 0*/, float height /*= 0*/)
{
	if (!bVisible) return;
	setFresh(false);
	setBlendMode();

	int imgW = input->getWidth();
	int imgH = input->getHeight();

	// TODO: Should draw filters have an update()?	
	vector<float> xpos;
	vector<float> ypos;

	int i;

	for (i = 0; i < tilesX; i++) {
		float posNorm = 0;
		if (useCurve) {
			if (i > 0) posNorm = ImGui::CurveValue((float)i / (float)tilesX, 11, xCurvePoints);
		}
		else {
			if (i > 0) posNorm = (float)i / (float)tilesX;
		}
		
		xpos.push_back(posNorm * (float)imgW);
	}

	//std::sort(xpos.begin(), xpos.end());

	for (i = 0; i < tilesY; i++) {
		float posNorm = 0;
		if (useCurve) {
			if (i > 0) posNorm = ImGui::CurveValue((float)i / (float)tilesY, 11, yCurvePoints);
		}
		else {
			if (i > 0) posNorm = (float)i / (float)tilesY;
		}
		ypos.push_back(posNorm * (float)imgH);
	}

	//std::sort(ypos.begin(), ypos.end());

	// Update(): This is where we start drawing :)

	for (int x = 0; x < xpos.size(); x++) {
		ofPushMatrix();
		ofTranslate(ofMap(xpos[x], 0, imgW, 0, width), 0);
		float w = 0;
		if (x < xpos.size()-1) {
			w = xpos[x + 1] - xpos[x];
		}
		else {
			w = imgW - xpos[x];
		}
		ofTranslate(ofMap(w*0.5, 0, imgW, 0, width), 0);

		for (int y = 0; y < ypos.size(); y++) {
			ofPushMatrix();
			ofTranslate(0, ofMap(ypos[y], 0, imgH, 0, height));
			float h = 0;
			if (y < ypos.size()-1) {
				h = ypos[y + 1] - ypos[y];
			}
			else {
				h = imgH - ypos[y];
			}
			ofTranslate(0, ofMap(h * 0.5, 0, imgH, 0, height));
			ofColor c = input->getPixels().getColor(floor(ofClamp(xpos[x] + (w * 0.5),0,imgW-1)), floor(ofClamp(ypos[y] + (h * 0.5), 0, imgH-1)));
			if (pixelMirror && (x % 2 == 0)) {
				ofScale(-1, 1);
			}
			if (pixelMirror && (y % 2 == 0)) {
				ofScale(1, -1);
			}
			drawPixels.v_Objects[selectedPixelType]->draw(c, { ofMap(w, 0, imgW, 0, width), ofMap(h, 0, imgH, 0, height) }, { 0, 0 }, { (float)x / (float)(tilesX - 1), (float)y / (float)(tilesY - 1) });
			ofPopMatrix();
		}
		ofPopMatrix();
	}
}

void Df_pixelate2::renderImGuiSettings()
{
	ImGui::SetNextItemOpen(_isOpen, ImGuiCond_Once);
	if (ImGui::CollapsingHeader(name.c_str(), &bAlive)) {
		_isOpen = true;
		ImGui::AlignTextToFramePadding();

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
		ImGui::SameLine();
		if (ImGui::Checkbox("Curve", &useCurve)) {
			setFresh(true);
		}

		ImGui::PopItemWidth();

		if (useCurve) {
			if (ImGui::Curve("X Horizontal", ImVec2(ImGui::GetContentRegionAvail().x - 6, 100), 11, xCurvePoints, &selectionIdx))
			{
				setFresh(true); // curve changed
			}
			if (ImGui::Curve("Y Vertical", ImVec2(ImGui::GetContentRegionAvail().x - 6, 100), 11, yCurvePoints, &selectionIdy))
			{
				setFresh(true); // curve changed
			}
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

		ImGui::PushItemWidth(200);
		if (ofxImGui::VectorCombo("##Blend Mode", &currentBlendModeIndex, v_BlendModes)) {
			setFresh(true);
		}
		ImGui::PopItemWidth();

	} else {
		_isOpen = false;
	}
}

void Df_pixelate2::loadSettings(ofJson& settings)
{
	try {
		_isOpen = settings.value("_isOpen", _isOpen);
		selectedPixelType = ofx2d::getIndex(drawPixels.v_objectNames, settings.value("pixelType", "Undefined"), selectedPixelType);
		tilesX = settings.value("tilesX", tilesX);
		tilesY = settings.value("tilesY", tilesY);
		pixelMirror = settings.value("pixelMirror", pixelMirror);
		useCurve = settings.value("useCurve", useCurve);

		for (int i = 0; i < 11; i++) {
			xCurvePoints[i].x = settings["xCurve"][i][0];
			xCurvePoints[i].y = settings["xCurve"][i][1];
			yCurvePoints[i].x = settings["yCurve"][i][0];
			yCurvePoints[i].y = settings["yCurve"][i][1];
		}

		ofJson pixelSettings = settings.value("pixelSettings", pixelSettings);
		drawPixels.v_Objects[selectedPixelType]->loadSettings(pixelSettings);
	}
	catch (...) {
		ofLogNotice() << "Failed to load settings Df_pixelate2";
	}
}

ofJson Df_pixelate2::getSettings()
{
	ofJson settings;
	settings["name"] = name;
	settings["_isOpen"] = _isOpen;
	settings["pixelType"] = drawPixels.v_objectNames[selectedPixelType];
	settings["tilesX"] = tilesX;
	settings["tilesY"] = tilesY;
	settings["pixelMirror"] = pixelMirror;
	settings["useCurve"] = useCurve;

	for (int i = 0; i < 11; i++) {
		settings["xCurve"][i][0] = xCurvePoints[i].x;
		settings["xCurve"][i][1] = xCurvePoints[i].y;
		settings["yCurve"][i][0] = yCurvePoints[i].x;
		settings["yCurve"][i][1] = yCurvePoints[i].y;
	}

	ofJson pixelSettings = drawPixels.v_Objects[selectedPixelType]->getSettings();
	settings["pixelSettings"] = pixelSettings;

	return settings;
}
