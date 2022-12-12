#pragma once

#include "ofMain.h"
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
		void loadImage(string& filepath);

		ofxDropdown_<string> imageDropdown{ "Source Image" };

		ofxPanel gui;
		ofxIntSlider tilesX, tilesY;
		ofxFloatSlider addonx, addony, randomOffset;
		
		ofxColorSlider paperColor;
		ofxColorSlider magentaRed;
		ofxColorSlider cyanBlue;
		ofxColorSlider yellowGreen;
		ofxColorSlider black;

		ofxToggle showImage, normalise, roundPixels, showZoom;
		ofxButton exportSVG, setRGB, setCMYK, setPosterize;

		unique_ptr<ofxDropdown> styleDropdown;
		unique_ptr<ofxDropdown> blendDropdown;

		float percentage(float percent, float total);
		void gui_exportSVG_pressed();
		void gui_setRGB_pressed();
		void gui_setCMYK_pressed();
		void gui_setPosterize_pressed();

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
		float imgRatio;

		float rotation = 0;
		bool saveSVG, isLandscape;

		ofVec4f getCMYK(ofColor rgb);

		ofFbo fbo;
		

};
