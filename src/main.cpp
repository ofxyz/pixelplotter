#include "ofMain.h"
#include "ofApp.h"

//#pragma comment(linker, "/SUBSYSTEM:Windows /ENTRY:mainCRTStartup")

int main() {
	ofGLFWWindowSettings settings;

#ifdef TARGET_OPENGLES
	settings.setGLESVersion(2);
#else
	// Programmable pipeline
	//settings.setGLVersion(3, 2); 
#endif

	settings.title = ofApp::getWindowTitle();
	// Until we can drag main menu and can switch between full screen
	settings.decorated = true;
	settings.windowMode = OF_WINDOW;
	settings.setSize(1600, 900);
	settings.setPosition(glm::vec2(50, 50));
	settings.resizable = true;
	settings.multiMonitorFullScreen = false;

	ofCreateWindow(settings);
	ofRunApp(new ofApp());
}
