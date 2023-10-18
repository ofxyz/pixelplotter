#include "ofMain.h"
#include "ofApp.h"

#pragma comment(linker, "/SUBSYSTEM:Windows /ENTRY:mainCRTStartup")

int main() {
	ofGLFWWindowSettings settings;

	settings.title = "PixelPlotter V0.3";
	// Until we can drag main menu and can switch between full screen
	settings.decorated = true;
	settings.windowMode = OF_WINDOW;
	settings.setSize(1920, 1200);
	settings.setPosition(glm::vec2(50, 50));
	settings.resizable = true;
	settings.multiMonitorFullScreen = false;
	// ofxShadertoy NEEDS the GL Programmable Renderer
	// settings.setGLVersion(3, 2);
	// settings.setGLVersion(4, 1); 
	
	ofCreateWindow(settings);
	
	ofRunApp(new ofApp());
}
