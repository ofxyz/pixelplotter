#pragma once
#include "ofMain.h"
#include "ofGui.h"
#include "canvas.h"
#include "vc_SoundDevice.h"

class ofApp : public ofBaseApp {

public:
	static std::string getWindowTitle() {
		return "PixelPlotter v0.61";
	}

	void setup();
	void update();
	void draw();
	void exit();

	void keyPressed(int key);
	void keyReleased(int key);

	void mouseMoved(int x, int y);
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void mouseEntered(int x, int y);
	void mouseExited(int x, int y);
	void mouseScrolled(ofMouseEventArgs& mouse);

	void audioIn(ofSoundBuffer& input);

	void windowResized(int w, int h);
	void dragEvent(ofDragInfo dragInfo);
	void gotMessage(ofMessage msg);

	void resetImageOffset();
	void centerImage();
	void resetZoom();

	OfGui ofGui;
	Canvas plotCanvas;
	soundDeviceController soundManager;

	bool vSync = true;
	bool bDragCanvas = false;

	int exportCount = 0;

	float zoomLevel = 1;

	glm::vec2 offset;
	glm::vec2 userOffset;
	glm::vec2 lastDraggedPos;

	int _targetFps = 60;
};
