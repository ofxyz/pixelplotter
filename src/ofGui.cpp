#include "ofGui.h"
#include "ofApp.h"

void ofGui::setup(){
	ofBackground(0);
	ofSetVerticalSync(false);
	//ofSetFrameRate(30);
	ofSetWindowTitle("Pixel Plotter");
	mainApp->gui_setup();
}

void ofGui::update(){
	mainApp->gui_update();
}

void ofGui::draw(){
	mainApp->gui_draw();
}

void ofGui::exit() {
	//mainApp->exit();
	OF_EXIT_APP(0);
}
