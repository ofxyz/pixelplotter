#pragma once
#include "ofMain.h"
#include "ofx2d.h"
#include "ofxImGui.h"
#include "ofxPosterize.h"
#include "ofxXmlSettings.h"
#include "ofxOpenCv.h"

#include "sourceController.h"
#include "canvas.h"
 
const int gui_width = 350;
const int img_area_WH = 1200;

extern ofx2d x2d;

class ofApp : public ofBaseApp{

	public:
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

		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);

		void saveSettings(string& filepath);
		void loadSettings(string& filepath);

		void resetImageOffset();
		void resetZoom();

		SourceController sourceController;
		Canvas canvas;

		char presetSaveName[128] = "";
		int exportCount = 0;

		bool cleanDrawFilters = false;
		bool cleanImageFilters = false;
		bool reorderDrawFilters = false;
		bool bSavePreset = false;
		bool show_main_window = true;
		bool pauseRender = false;
		bool showImage = false;
		bool bDragCanvas = false;

		int currentBlendModeIndex = 0;
		int currentPresetIndex = 0;
		int currentDrawFilterIndex = 0;
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
		void gui_draw();
		void gui_update();
		void gui_loadPresets();
		// -------------------------------------------------  End ofUI.cpp
};
