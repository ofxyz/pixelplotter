#include "ofMain.h"
#include "ofApp.h"

bool fullscreen = false;
//========================================================================
int main( ){
	if (fullscreen) {
		ofGLFWWindowSettings settings;
		settings.decorated = false;
		settings.windowMode = OF_FULLSCREEN;
		settings.setSize(screenW, screenH);
		ofCreateWindow(settings);
	}
	else {
		ofSetupOpenGL(screenW, screenH, OF_WINDOW);
	}
	ofRunApp(new ofApp());
}
