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

		ofxPanel gui;
		ofxIntSlider tilesX;
		ofxFloatSlider addonx, addony, randomOffset;
		
		ofxColorSlider paperColor;
		ofxColorSlider magentaRed;
		ofxColorSlider cyanBlue;
		ofxColorSlider yellowGreen;
		ofxColorSlider black;

		ofxToggle showImage, normalise, roundPixels;
		ofxButton exportSVG, setRGB, setCMYK;

		unique_ptr<ofxDropdown> styleDropdown;
		unique_ptr<ofxDropdown> blendDropdown;

		float percentage(float percent, float total);
		void gui_exportSVG_pressed();
		void gui_setRGB_pressed();
		void gui_setCMYK_pressed();

		void setBlendmode();
		void updateFbo();

		void callStyle(string stylename, float w, float h, ofColor c, float r);
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
		void Style_CMYK_Seperation_10(float w, float h, ofColor c, float rot);

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
		ofColor penColor;

		int gui_width = 320;
		float imgRatio;

		float rotation = 0;
		bool saveSVG, isLandscape;

		ofVec4f getCMYK(ofColor rgb);

		ofFbo fbo;
		

};
