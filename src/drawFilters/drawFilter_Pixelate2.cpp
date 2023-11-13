#include "drawFilter_Pixelate2.h"
#include "drawPixel.h"
#include "ofApp.h"
#include "ofx2d.h"
#include "ofxImGui.h"
#include "imgui_internal.h"
#include "ImHelpers.h"

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
}

void Df_pixelate2::draw(ofImage* input, float width /*= 0*/, float height /*= 0*/, float x /*= 0*/, float y /*= 0*/)
{
	if (!visible) return;
	setFresh(false);
	setBlendMode();

	// Maybe this should happen with update() not draw?
	
	int imgW = input->getWidth();
	int imgH = input->getHeight();
	float tileW = (float)imgW / (float)tilesX;
	float tileH = (float)imgH / (float)tilesY;
	float halfTileW = tileW * 0.5;
	float halfTileH = tileH * 0.5;

	for (int _y = 0; _y < tilesY; _y++) {
		ofPushMatrix();
		ofTranslate(0, ofMap(_y * tileH, 0, imgH, 0, height));
		for (int _x = 0; _x < tilesX; _x++) {
			ofPushMatrix();
			ofTranslate(ofMap(_x * tileW, 0, imgW, 0, width), 0);
			ofTranslate(ofMap(halfTileW, 0, imgW, 0, width), ofMap(halfTileH, 0, imgH, 0, height));
			ofColor c = input->getPixels().getColor(floor( (_x * tileW) + halfTileW), floor((_y * tileH) + halfTileH));
			if (pixelMirror && (_x % 2 == 0)) {
				ofScale(-1, 1);
			}
			if (pixelMirror && (_y % 2 == 0)) {
				ofScale(1, -1);
			}
			drawPixels.v_DrawPixels[selectedPixelType]->draw(c, { ofMap(tileW, 0, imgW, 0, width), ofMap(tileH, 0, imgH, 0, height) }, { 0, 0 }, { ofx2d::percent(_x,tilesX), ofx2d::percent(_y,tilesY) });
			ofPopMatrix();
		}
		ofPopMatrix();
	}
	ofPopMatrix();

}

void Df_pixelate2::renderImGuiSettings()
{
	if (ImGui::CollapsingHeader(name.c_str(), &active)) {
		ImGui::PushID("Df_pixelate2");
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

		if (ofxImGui::VectorCombo("Pixel Type ##pixelate2", &selectedPixelType, drawPixels.v_DrawPixelsNames)) {
			setFresh(true);
		};

		// Pass fresh through
		if (drawPixels.v_DrawPixels[selectedPixelType]->isFresh()) {
			drawPixels.v_DrawPixels[selectedPixelType]->setFresh(false);
			setFresh(true);
		}

		drawPixels.v_DrawPixels[selectedPixelType]->renderImGuiSettings();

		ImGui::PushItemWidth(200);
		if (ofxImGui::VectorCombo("##Blend Mode", &currentBlendModeIndex, v_BlendModes)) {
			setFresh(true);
		}
		ImGui::PopItemWidth();

		ImGui::PopID();
	}
}

void Df_pixelate2::loadSettings(ofJson& settings)
{
	try {
		selectedPixelType = ofx2d::getIndex(drawPixels.v_DrawPixelsNames, settings.value("pixelType", "Undefined"), selectedPixelType);
		tilesX = settings.value("tilesX", tilesX);
		tilesY = settings.value("tilesY", tilesY);
		pixelMirror = settings.value("pixelMirror", pixelMirror);

		ofJson pixelSettings = settings.value("pixelSettings", pixelSettings);
		drawPixels.v_DrawPixels[selectedPixelType]->loadSettings(pixelSettings);
	}
	catch (...) {
		ofLogNotice() << "Failed to load settings Df_pixelate2";
	}
}

ofJson Df_pixelate2::getSettings()
{
	ofJson settings;
	settings["name"] = name;
	settings["pixelType"] = drawPixels.v_DrawPixelsNames[selectedPixelType];
	settings["tilesX"] = tilesX;
	settings["tilesY"] = tilesY;
	settings["pixelMirror"] = pixelMirror;

	ofJson pixelSettings = drawPixels.v_DrawPixels[selectedPixelType]->getSettings();
	settings["pixelSettings"] = pixelSettings;

	return settings;
}
