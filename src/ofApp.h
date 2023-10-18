#pragma once
#include "ofMain.h"

#include "ofxImGui.h"
#include "imgui_internal.h"
#include "ImHelpers.h"
#include "imgui_stdlib.h"
#include "IconsFontAwesome5.h"

#include "canvas.h"
#include "drawPixels.h"
#include "soundManager.h"

class ofApp : public ofBaseApp {

public:
	std::string windowTitle = "PixelPlotter v0.3";

	ImGuiID dockNodeID;

	void setup();
	void update();
	void draw();
	void exit();

	void keyPressed(int key);
	void keyReleased(int key);

	void mouseMoved(int x, int y);
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void mouseEntered(int x, int y);
	void mouseExited(int x, int y);
	void mouseScrolled(ofMouseEventArgs& mouse);

	void audioIn(ofSoundBuffer& input);

	void windowResized(int w, int h);
	void dragEvent(ofDragInfo dragInfo);
	void gotMessage(ofMessage msg);

	void saveSettings(string& filepath);
	void loadSettings(string& filepath);

	void resetImageOffset();
	void centerImage();
	void resetZoom();

	Canvas plotCanvas;
	DrawPixels drawPixels;
	SoundManager soundManager;

	char presetSaveName[128] = "";
	int exportCount = 0;

	bool vSync = false;
	bool cleanImageFilters = false;
	bool bSavePreset = false;
	bool bShowMenuBar = true;
	bool bShowGui = true;
	bool bShowPlotCanvas = true;
	bool bShowToolPalette = true;
	bool bShowInfoPanel = true;
	bool bShowImGuiMetricsDebugger = false;
	bool bShowCanvas = true;
	bool pauseRender = false;
	bool bDragCanvas = false;
	bool bShowFps = true;
	bool bTryLoadSource = false;
	int currentPresetIndex = 0;

	float zoomLevel = 1;
	glm::vec2 userOffset;
	glm::vec2 lastDraggedPos;

	glm::vec2 workSpaceWidthHeight;
	glm::vec2 workSpaceTopLeft;
	glm::vec2 workSpaceCentre;

	ofxImGui::Gui gui;

	ofColor c_background = ofColor(50, 50, 50, 255);

	std::vector<std::string> presetFileNames;
	std::vector<ofFile> presetFiles;

	glm::vec2 offset;

	std::stringstream fpsStringStream;

	// -------------------------------------------------  Start ofUI.cpp
	void gui_setup();
	void gui_update();
	void gui_draw();
	void gui_drawMenuBar();
	void gui_drawMainDock();
	void gui_drawCanvasWindow();
	void gui_drawInfoPanel();
	void gui_drawToolPalette();
	void gui_loadPresets();
	void gui_renderCanvas();
	void gui_renderBezierWidget();

	bool bLoadSettingsNextFrame;

	// -------------------------------------------------  End ofUI.cpp
};
