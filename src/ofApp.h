#pragma once
#include "ofMain.h"
#include "ofx2d.h"
#include "ofxImGui.h"
#include "ofxPosterize.h"
#include "ofxXmlSettings.h"
#include "ofxOpenCv.h"

#include "drawFilter_Pixelate.h"
#include "drawFilter_Rings.h"

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
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);

		ofVideoPlayer videoPlayer;
		ofVideoGrabber videoGrabber;

		// Add to cam source settings ...
		int camWidth = 640;
		int camHeight= 480;

		void addDrawFilter(int index);
		void saveSettings(string& filepath);
		void loadSettings(string& filepath);
		void onImageChange(string& file);
		void loadImage(string& filepath);
		void loadVideo(string& filepath);

		string img_name = "PixelPlotted";
		void prep_img();

		char presetSaveName[128] = "";

		int exportCount = 0;
		
		bool cleanFilters = false;
		bool isLandscape;
		bool bSavePreset = false;
		bool show_main_window = true;
		bool saveVector = false;
		bool pauseRender = false;
		bool showImage = false;
		bool showZoom = false;
		bool bUseVideo = false;
		bool bUseVideoDevice = false;

		void updateFbo();

		float percentage(float percent, float total);

		ofImage original, img;

		ofPixels pixels;
		ofMesh mesh;

		std::vector<ofColor> swatches;

		int currentSourceIndex = 0;
		int currentBlendModeIndex = 0;
		int currentPresetIndex = 0;
		int currentDrawFilterIndex = 0;
		float imgRatio;

		ofFbo canvasFbo;
		ofFbo zoomFbo;
		int currentVideoFrame = 1;

		float ratio = 1;
		int zoomWindowW = 300;
		int zoomWindowH = 300;
		float halfZoomWindowW = zoomWindowW * 0.5;
		float halfZoomWindowH = zoomWindowH * 0.5;
		int zoomMultiplier = 4;
		int exportCounter = 0;
		float zoomLevel = 1;
		ofVec2f userOffset;
 
		ofxImGui::Gui gui;
		ImVec4 c_background = ofColor(50, 50, 50, 255);
		ImVec4 c_paper = ofColor(255, 255, 255, 255);

		std::vector<std::string> sourceNames;
		
		std::vector<std::string> videoDeviceNames;
		std::vector<ofVideoDevice> videoDevices;

		std::vector<std::string> videoFileNames;
		std::vector<ofFile> videoFiles;

		std::vector<std::string> imgFileNames;
		std::vector<ofFile> imgFiles;
		
		std::vector<std::string> presetFileNames;
		std::vector<ofFile> presetFiles;

		ofVec2f offset;

		void plotIt();

		// -------------------------------------------------  Start ofUI.cpp
		void gui_draw();
		void gui_update();
		void gui_loadPresets();
		void gui_loadSourceIndex();
		void gui_buildSourceNames();
		void gui_setRGB_pressed();
		void gui_setCMYK_pressed();
		void gui_setAvarage_pressed();
		void gui_setBlendmode();
		// -------------------------------------------------  Start ofUI.cpp

		// ------------------------------------------------- Start Interfaces

		std::vector<DrawFilter*> v_DrawFilters;
		
		std::vector<std::string> v_DrawFilterNames {
			"Add Draw Filter ...",
			"Pixelate",
			"Rings"
		};

		// ------------------------------------------------- End Interfaces

		string to_lower(string s) {
			for (char& c : s)
				c = tolower(c);
			return s;
		}

		std::vector<std::string> img_ext{
				"jpg",
				"jpeg",
				"png",
				"bmp",
				"gif"
		};

		std::vector<std::string> vid_ext{
				"mpeg",
				"mpeg-2",
				"mpeg2",
				"mpg",
				"mp4",
				"mkv",
				"m4v",
				"mov",
				"wmv",
				"avi"
		};
};
