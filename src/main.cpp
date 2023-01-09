#include "ofMain.h"
#include "ofApp.h"

//========================================================================
int main( ){
	ofSetupOpenGL(img_area_WH + gui_width, img_area_WH, OF_WINDOW);
	ofRunApp(new ofApp());
}
