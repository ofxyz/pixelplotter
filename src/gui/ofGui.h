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
	
	// Visibility
	bool guiVisible();
	void setGuiVisible(bool bVisible);
	bool menuBarVisible();
	void setmenuBarVisible(bool bVisible);
	
	bool renderingPaused();
	void setRenderingPaused(bool bPaused);


	// Presets and Settings
	// Implement this ... work out what are settings and what are presets
	//void loadSettings(ofJson& settings);
	//ofJson getSettings();

	void loadPresetDir();
	void savePresets();
	void loadPresets(ofJson settings);

	std::string getPresetAbsolutePath(int presetIndex);
	ofJson getPresets(int presetIndex);
	ofJson getCurrentPresets();

	ImVec4 availableSpace();
	

private:
	bool _bGuiVisible;
	bool _bMenuBarVisible;

	bool _bRenderingPaused;
	bool _bLoadSourceFile;

	// TODO: Palette, Window, Panel, Have some definitions? Is Canvas a Window panel or Palette?
	// Also do we Use `Show` or `Visible`

	bool _bShowCanvas;
	bool _bShowPlotCanvas;
	bool _bShowInfoPanel;
	bool _bShowToolPalette;
	bool _bShowImGuiMetricsDebugger;

	bool _bShowFps;
	bool _bSavePresets;

	bool _bLoadSettingsNextFrame;
	int _currentPresetIndex;

	char _presetSaveName[128];

	ImGuiID MainDockNodeID;

	std::stringstream fpsStringStream;

	std::vector<std::string> presetFileNames;
	std::vector<ofFile> presetFiles;

	ofDirectory presetDirectory;
};
