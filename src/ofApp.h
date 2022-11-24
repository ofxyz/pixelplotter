#pragma once

#include "ofMain.h"
#include "ofxGui.h"
#include "ofxDropdown.h"

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();
		void exit();

		void onImageChange(string& file);
		void loadImage(string& filepath);

		ofxDropdown_<string> imageDropdown{ "Source Image" };
		
		int gui_width = 320;

		ofxPanel gui;
		ofxIntSlider tilesX;
		ofxFloatSlider gui_radial_noise_step, gui_radial_noise_amount;
		ofxColorSlider gui_bar_colour;
		ofxToggle showImage;
		ofxButton exportSVG, updateScreen;

		ofParameter<string> styleParameter;
		unique_ptr<ofxDropdown> styleDropdown;

		void gui_updateScreen_pressed();
		void gui_exportSVG_pressed();

		void callStyle(string stylename, float w, float h, ofColor c);
		void Style_0(float w, float h, ofColor c);
		void Style_1(float w, float h, ofColor c);
		void Style_2(float w, float h, ofColor c);
		void Style_3(float w, float h, ofColor c);
		void Style_4(float w, float h, ofColor c);
		void Style_5(float w, float h, ofColor c);

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

		ofImage img;
		ofPixels pixels;
		ofMesh mesh;
		ofColor penColor, paperColor;

		float imgRatio, tileSize;

		bool drawScreen, oneShot, isLandscape;

		string style = "Style 0";

};
