#include "ofMain.h"
#include "ofApp.h"

bool fullscreen = false;
//========================================================================
int main( ){
	if (fullscreen) {
		ofGLFWWindowSettings settings;
		settings.decorated = false;
		settings.setSize(img_area_WH + gui_width, img_area_WH);
		ofCreateWindow(settings);
	}
	else {
		ofSetupOpenGL(img_area_WH + gui_width, img_area_WH, OF_WINDOW);
	}
	ofRunApp(new ofApp());
}
