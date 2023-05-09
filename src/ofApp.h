#pragma once
#include "ofMain.h"

#include "ofx2d.h"
#include "ofxImGui.h"
#include "imgui_internal.h"
#include "ImHelpers.h"
#include "imgui_stdlib.h"

#include "ofxPosterize.h"
#include "ofxXmlSettings.h"
#include "ofxOpenCv.h"

#include "soundManager.h"
#include "sourceController.h"
#include "canvas.h"

const int gui_width = 400;
const int screenW = 1920;
const int screenH = 1200;

extern ofx2d x2d;

static void HelpMarker(const char* desc) {
	ImGui::TextDisabled("(?)");
	if (ImGui::IsItemHovered()) {
		ImGui::BeginTooltip();
		ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
		ImGui::TextUnformatted(desc);
		ImGui::PopTextWrapPos();
		ImGui::EndTooltip();
	}
}

class ofApp : public ofBaseApp{

	public:
		shared_ptr<ofGLRenderer> gl;

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
		void resetZoom();

		SoundManager soundManager;
		SourceController sourceController;
		Canvas plotCanvas;

		char presetSaveName[128] = "";
		int exportCount = 0;

		bool cleanImageFilters = false;
		
		bool bSavePreset = false;
		bool bShowMenuBar = true;
		bool bShowGui = true;
		bool pauseRender = false;
		bool bDragCanvas = false;

		int currentPresetIndex = 0;
		int currentImageFilterIndex = 0;

		float zoomLevel = 1;
		ofVec2f userOffset;
		glm::vec2 lastDraggedPos;

		ofxImGui::Gui gui;
		ImVec4 c_background = ofColor(50, 50, 50, 255);
		
		std::vector<std::string> presetFileNames;
		std::vector<ofFile> presetFiles;

		ofVec2f offset;

		// -------------------------------------------------  Start ofUI.cpp
		void gui_setup();
		void gui_update();
		void gui_draw();
		void gui_drawMenuBar();
		void gui_drawCanvasWindow();
		void gui_loadPresets();
		
		bool bLoadSettingsNextFrame;

		// -------------------------------------------------  End ofUI.cpp
};
