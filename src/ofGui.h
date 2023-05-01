#pragma once

#include "ofMain.h"
class ofApp;

class ofGui: public ofBaseApp {
public:
	shared_ptr<ofApp> mainApp;
	shared_ptr<ofGLRenderer> gl;
	void setup();
	void update();
	void draw();
	void exit();
};