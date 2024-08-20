#include "ofMain.h"
#include "ofApp.h"

//#pragma comment(linker, "/SUBSYSTEM:Windows /ENTRY:mainCRTStartup")

int main() {
	ofGLFWWindowSettings settings;

#ifdef TARGET_OPENGLES
	settings.setGLESVersion(2);
#else
	// Programmable pipeline (3, 2)
	settings.setGLVersion(4, 6);
#endif

	settings.title = "PixelPlotter v0.610";
	settings.decorated = false;
	settings.windowMode = OF_WINDOW;
	settings.setSize(1600, 900);
	settings.setPosition(glm::vec2(50, 50));
	settings.resizable = true;
	settings.multiMonitorFullScreen = false;

	ofCreateWindow(settings);

	ofRunApp(new ofApp());
}
