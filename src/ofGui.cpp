#include "ofGui.h"
#include "ofApp.h"

void ofGui::setup(){
	ofBackground(0);
	ofSetVerticalSync(false);
	mainApp->gui_setup();

}

void ofGui::update(){
	mainApp->gui_update();
}

void ofGui::draw(){
	mainApp->gui_draw();
}