#include "ofMain.h"
#include "ofApp.h"
#include "ofGui.h"
#include "ofAppGLFWWindow.h"

//========================================================================
int main( ){
	ofGLFWWindowSettings settings;

	settings.decorated = true;
	settings.resizable = true;
	settings.windowMode = OF_WINDOW;
	settings.monitor = 1;
	settings.setSize(screenW, screenH);
	settings.setPosition(glm::vec2(100, 100));
	shared_ptr<ofAppBaseWindow> guiWindow = ofCreateWindow(settings);

	settings.decorated = false;
	settings.windowMode = OF_FULLSCREEN;
	settings.monitor = 2;
	settings.setSize(screenW, screenH);
	settings.setPosition(glm::vec2(2560, 0)); // Monitor 1 width....
	settings.resizable = true;
	shared_ptr<ofAppBaseWindow> mainWindow = ofCreateWindow(settings);

	shared_ptr<ofGui> mainGui(new ofGui);
	shared_ptr<ofApp> mainApp(new ofApp);
	
	mainApp->mainGui = mainGui;
	mainGui->mainApp = mainApp;

	ofRunApp(mainWindow, mainApp);
	ofRunApp(guiWindow, mainGui);

	ofRunMainLoop();
}
