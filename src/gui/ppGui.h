#pragma once

#include "ofxImGui.h"
#include "ofJson.h"
#include "ppDrawObject.h"

class ofApp;

class ppGui {
public:
	ppGui();
	ofApp* pixelplotter{ nullptr };
	ofxImGui::Gui gui;

	void setup();
	void update();
	
	void draw();
	void showMainDock();
	void showMainMenuBar();
	void showProjectTree();
	void showZoomViewer();
	void showToolPalette();
	void showPresetPanel();
	void showCanvasWindow();
	void showPropertiesWindow();
	void showTextureBrowser();
	
	// Visibility
	bool guiVisible();
	void setGuiVisible(bool bVisible);
	bool menuBarVisible();
	void setmenuBarVisible(bool bVisible);
	
	bool renderingPaused();
	void setRenderingPaused(bool bPaused);

	//TODO Implement Global Presets and Settings collection
	//void loadSettings(ofJson& settings);
	//ofJson getSettings();

	void loadPresetDir();
	void savePresets();
	void loadPresets(ofJson settings);
	std::string getPresetAbsolutePath(int presetIndex);
	ofJson getPresets(int presetIndex);
	ofJson getCurrentPresets();

	ImVec4 availableSpace();
	
	std::shared_ptr<ppDrawObject> _drawTexture;

private:
	
	// TODO: Palette, Window, Panel, Have some definitions? Is Canvas a Window panel or Palette?

	// GUI TRACKERS
	ImGuiID MainDockNodeID;
	std::stringstream fpsStringStream;
	std::shared_ptr<ppBase> _selectedItem;
	bool _bRenderingPaused;

	// GUI Visibility Trackers
	bool _bShowGui;
	bool _bShowMainMenuBar;
	bool _bShowProjectTree;
	bool _bShowTextureBrowser;
	bool _bShowZoomViewer;
	bool _bShowCanvasWindow;
	bool _bShowPresetPanel;
	bool _bShowToolPalette;
	bool _bShowMetricsWindow;
	bool _bShowPropertiesWindow;
	bool _bShowStyleEditor;
	bool _bShowFps;

	// GUI Next Frame Action Tracker
	bool _bSavePresets;
	bool _bLoadSettingsNextFrame;
	bool _bLoadSourceFile;

	// This should be closer to the preset manager
	int _currentPresetIndex;
	char _presetSaveName[128];
	std::vector<std::string> presetFileNames;
	std::vector<ofFile> presetFiles;
	ofDirectory presetDirectory;

};
