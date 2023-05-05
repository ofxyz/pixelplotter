#include "ofMain.h"
#include "ofApp.h"
#include "ofAppGLFWWindow.h"

#pragma comment(linker, "/SUBSYSTEM:Windows /ENTRY:mainCRTStartup")

//========================================================================
int main( ){
	ofGLFWWindowSettings settings;
	settings.decorated = true;
	settings.windowMode = OF_WINDOW;
	settings.setSize(screenW, screenH);
	settings.setPosition(glm::vec2(50, 50));
	settings.resizable = true;

	shared_ptr<ofAppBaseWindow> mainWindow = ofCreateWindow(settings);
	shared_ptr<ofApp> mainApp(new ofApp);

	mainApp->gl = static_pointer_cast<ofGLRenderer>(mainWindow->renderer());

	ofRunApp(mainWindow, mainApp);

	ofRunMainLoop();
}
