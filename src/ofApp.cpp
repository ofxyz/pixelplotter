#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {
	ofLogToConsole();
	ofSetLogLevel(OF_LOG_VERBOSE);
	ofEnableAntiAliasing();

	oneShot   = false;
	drawImage = false;

	showImage = false;

	penColor = ofColor(0, 0, 0, 200);
	paperColor = ofColor(250, 250, 250);

	img_name = "spherereference";
	img_ext  = "jpg";
	img.load("src_img/"+ img_name + "." + img_ext);

	(img.getWidth() > img.getHeight()) ? isLandscape = true : isLandscape = false;

	imgRatio = float(img.getWidth()) / float(img.getHeight());
	img.resize(ofGetHeight()* imgRatio, ofGetHeight());
	
	tilesX = 120;
	tilesY = imgRatio * tilesX;
	ofSetBackgroundAuto(false);
	ofBackground(paperColor);
	tileSize = float(ofGetWidth()) / tilesX;
	tileAddon = 4;
	
	penWidth = 1.5;

}

//--------------------------------------------------------------
void ofApp::update(){
	
}

//--------------------------------------------------------------
void ofApp::draw(){
	
	if (drawImage) {
		
		ofBackground(paperColor);
		
		if (showImage) {
			img.draw(0, 0);
		}

		if (oneShot) {
			ofBeginSaveScreenAsPDF("pixelplotted_" + img_name + "_" + ofGetTimestampString() + ".pdf", false);
			ofBeginSaveScreenAsSVG("pixelplotted_" + img_name + "_" + ofGetTimestampString() + ".svg", false);
		}
		
		ofNoFill();
		ofSetLineWidth(penWidth);
		ofSetColor(penColor);

		pixels = img.getPixels();
		int w = img.getWidth();
		int h = img.getHeight();
		int yCount = 0;
		for (int y = 0; y < h; y += tileSize) {
			yCount++;
			int xCount = 0;
			for (int x = 0; x < w; x += tileSize) {
				xCount++;
				ofColor c = pixels.getColor(x, y);
				float l = c.getLightness();

				float size = (((255.0f - l) / 255.0f)*tileSize)+tileAddon;
				float cx = (xCount * tileSize) - (tileSize / 2);
				float cy = (yCount * tileSize) - (tileSize / 2);
				ofPushMatrix();
				ofTranslate(cx, cy, 0);
				ofRotateZDeg(l);
				drawPixel(size);
				ofPopMatrix();
			}
		}

		if (oneShot) {
			ofEndSaveScreenAsPDF();
			ofEndSaveScreenAsSVG();
		}

		drawImage = false;
		oneShot = false;
	}

}

//--------------------------------------------------------------
void ofApp::drawPixel(float size) {
	int stepCount = (size / (penWidth * 4)) + 1; //  min 1;
	float stepSize = tileSize / float(stepCount);
	float startX = -(tileSize/2);
	for (int i(0); i <= stepCount; i++) {
		float x = startX + ofRandom(penWidth, stepSize-penWidth);
		ofDrawLine(x, -(tileSize / 2)+ ofRandom(-2, 2), x, size + (tileSize / 4) + ofRandom(-2, 2));
		startX += stepSize;
	}
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	drawImage = true;
	oneShot = true;
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
