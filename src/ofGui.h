#pragma once

#include "ofMain.h"
class ofApp;

class ofGui: public ofBaseApp {
public:
	shared_ptr<ofApp> mainApp;
	void setup();
	void update();
	void draw();
};