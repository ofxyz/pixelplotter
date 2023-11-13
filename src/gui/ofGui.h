#pragma once

#include "ofxImGui.h"
#include "ofJson.h"

class ofApp;

class OfGui {
public:
	OfGui();
	ofApp* pixelplotter;
	ofxImGui::Gui gui;

	void setup();
	void update();
	void draw();

	void drawMainDock();
	void drawMenuBar();
	void drawCanvas();
	void drawToolPalette();
	void drawInfoPanel();
	void drawCanvasWindow();
	void drawBezierWidget();
	
	void savePreset();
	void loadPresets();

	//TODO: Make private
	bool bShowMenuBar;
	bool bTryLoadSource;
	bool bRenderingPaused;

	int currentPresetIndex;
	std::string getPresetAbsolutePath(int presetIndex);
	ofJson getPreset(int presetIndex);
	ofJson getCurrentPreset();

	ImVec4 availableSpace();
	bool bShowGui;

private:
	bool bShowPlotCanvas;
	bool bShowInfoPanel;
	bool bShowToolPalette;
	bool bShowImGuiMetricsDebugger;
	bool bShowCanvas;

	bool bShowFps;
	bool bSavePreset;

	char presetSaveName[128];

	ImGuiID MainDockNodeID;

	std::stringstream fpsStringStream;

	std::vector<std::string> presetFileNames;
	std::vector<ofFile> presetFiles;

	ofDirectory presetDirectory;
};
