#pragma once

#include "ofMain.h"
#include "ofxImGui.h"
#include "ofxGui.h"
#include "ofxDropdown.h"
#include "ofxPosterize.h"

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();
		void exit();

		void onImageChange(string& file);
		void onPresetChange(string& file);
		void loadImage(string& filepath);

		string img_name = "PixelPlotted";

		int tilesX = 64;
		int tilesY = 64;
		int everynx = 4;
		int everyny = 4;
		float addonx = 0;
		float addony = 0;
		float randomOffset = 0;
		float noisepercentX = 0;
		float noisepercentY = 0;

		bool isLandscape;
		bool show_main_window = true;
		bool saveVector = false;
		bool pauseRender = false;
		bool normalise = false;
		bool showImage = false;
		bool roundPixels = false;
		bool showZoom = false;

		float percentage(float percent, float total);

		void gui_setRGB_pressed();
		void gui_setCMYK_pressed();
		void gui_setAvarage_pressed();

		void setBlendmode();
		void updateFbo();

		void callStyle(string stylename, ofVec2f size, ofVec2f loc, ofDefaultVec2 xycount, ofColor c);
		void Style_Pixelate(float w, float h, ofColor c);
		void Style_Lightness_Rotation(float w, float h, ofColor c);
		void Style_RGB_Seperation_1(float w, float h, ofColor c);
		void Style_RGB_Seperation_2(float w, float h, ofColor c);
		void Style_RGB_Seperation_3(float w, float h, ofColor c);
		void Style_RGB_Seperation_4(float w, float h, ofColor c);
		void Style_RGB_Seperation_5(float w, float h, ofColor c);
		void Style_Pixelate_Brightness_Width(float w, float h, ofColor c);
		void Style_CMYK_Seperation_1(float w, float h, ofColor c);
		void Style_CMYK_Seperation_2(float w, float h, ofColor c);
		void Style_CMYK_Seperation_3(float w, float h, ofColor c);
		void Style_CMYK_Seperation_4(float w, float h, ofColor c);
		void Style_CMYK_Seperation_5(float w, float h, ofColor c);
		void Style_CMYK_Seperation_6(float w, float h, ofColor c);
		void Style_CMYK_Seperation_7(float w, float h, ofColor c);
		void Style_CMYK_Seperation_8(float w, float h, ofColor c);
		void Style_CMYK_Seperation_9(float w, float h, ofColor c);
		void Style_CMYK_Seperation_10(float w, float h, ofColor c, ofVec2f loc);
		void Style_CMYK_Seperation_11(float w, float h, ofColor c, ofDefaultVec2 xycount);
		void Style_CMYK_Seperation_12(float w, float h, ofColor c, ofDefaultVec2 xycount);

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);

		ofImage original, img;
		ofPixels pixels;
		ofMesh mesh;
		ofColor penColor;

		std::vector<ofColor> swatches;

		int gui_width = 320;
		int currentImgFileIndex = 0;
		int currentBlendModeIndex = 0;
		int currentPlotStyleIndex = 0;
		float imgRatio;

		float rotation = 0;

		ofVec4f getCMYK(ofColor rgb);

		ofFbo fbo;
		ofFbo zoomFbo;

		float ratio = 1;
		int zoomWindowW = 300;
		int zoomWindowH = 300;
		float halfZoomWindowW = zoomWindowW * 0.5;
		float halfZoomWindowH = zoomWindowH * 0.5;
		int zoomMultiplier = 4;
		int exportCounter = 0;

		ofxImGui::Gui gui;
		ImVec4 c_background;
		ImVec4 c_paper;
		ImVec4 c_magentaRed;
		ImVec4 c_cyanBlue;
		ImVec4 c_yellowGreen;
		ImVec4 c_black;

		std::vector<std::string> imgFileNames;
		std::vector<ofFile> imgFiles;

		std::string currentBlendmode = "OF_BLENDMODE_DISABLED";
		std::vector<std::string> v_BlendModes{ "OF_BLENDMODE_DISABLED", "OF_BLENDMODE_ALPHA", "OF_BLENDMODE_ADD", "OF_BLENDMODE_SUBTRACT", "OF_BLENDMODE_MULTIPLY", "OF_BLENDMODE_SCREEN"};
		std::vector<std::string> v_PlotStyles{ 
			"Pixelate",
			"Pixelate Brightness Width",
			"Lightness Rotation",
			"RGB Seperation 1",
			"RGB Seperation 2",
			"RGB Seperation 3",
			"RGB Seperation 4",
			"RGB Seperation 5",
			"CMYK Seperation 1",
			"CMYK Seperation 2",
			"CMYK Seperation 3",
			"CMYK Seperation 4",
			"CMYK Seperation 5",
			"CMYK Seperation 6",
			"CMYK Seperation 7",
			"CMYK Seperation 8",
			"CMYK Seperation 9",
			"CMYK Seperation 10",
			"CMYK Seperation 11",
			"CMYK Seperation 12"
		};
};
