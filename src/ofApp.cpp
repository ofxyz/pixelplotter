#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {
	ofLogToConsole();
	ofSetLogLevel(OF_LOG_VERBOSE);
	ofSetBackgroundAuto(false);

	oneShot   = true;
	drawImage = true;
	showImage = false;

	paperColor = ofColor(200, 200, 200);

	img_name = "32-32.png";
	img.load("src_img/" + img_name);

	(img.getWidth() >= img.getHeight()) ? isLandscape = true : isLandscape = false;

	if (isLandscape) {
		imgRatio = float(img.getHeight()) / float(img.getWidth());
		img.resize(ofGetWidth(), ofGetHeight() * imgRatio);
	} else {
		imgRatio = float(img.getWidth()) / float(img.getHeight());
		img.resize(ofGetWidth() * imgRatio, ofGetHeight());
	}
	 
	tilesX = 64*2;
	tilesY = 64*2;

	ofSetBackgroundAuto(false);
	//ofBackground(paperColor);
	tileSize = ofGetWidth() / tilesX;

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
			ofBeginSaveScreenAsSVG("pixelplotted_" + img_name + "_" + ofGetTimestampString() + ".svg", false);
		}
		
		//ofNoFill();

		pixels = img.getPixels();
		int w = img.getWidth();
		int h = img.getHeight();

		float halfTile = (float)tileSize / 2.0;

		int yCount = 0;
		for (int y = 0; y < h; y += tileSize) {
			yCount++;
			int xCount = 0;
			for (int x = 0; x < w; x += tileSize) {
				xCount++;

				float centerx = x + halfTile;
				float centery = y + halfTile;

				ofColor c = pixels.getColor( x, y);

				//if (l >= 250) continue;

				//c.normalize();
				//c.setSaturation(255);

				ofPushMatrix();
				ofTranslate(centerx, centery, 0);
				
				//float lineLength = halfTile + (abs(ofRandomf())* tileSize);
				
				drawPixel_style004(tileSize, tileSize, c);
			
				ofPopMatrix();
			}
		}

		if (oneShot) {
			ofEndSaveScreenAsSVG();
		}

		drawImage = false;
		oneShot = false;
	}
}

//--------------------------------------------------------------
void ofApp::drawPixel_style000(float w, float h, ofColor c) {
	ofPushStyle();
	ofFill();
	ofSetColor(c);
	ofDrawRectangle(-(w * 0.5), -(h * 0.5), w, h);
	ofPopStyle();
}

//--------------------------------------------------------------
void ofApp::drawPixel_style001( float w, float h, ofColor c) {
	ofPushMatrix();
	ofRotateZDeg(ofMap(c.getLightness(), 0, 255, 0, 360));
	drawPixel_style000(w, h, c);
	ofPopMatrix();
}

//--------------------------------------------------------------
void ofApp::drawPixel_style002(float w, float h, ofColor c) {

	// Needs to be CMY!

	ofPushStyle();
	ofFill();
	
	float cHeight;

	ofSetColor(255, 0, 0); // Red
	cHeight = ofMap(c.r, 0, 255, 0, h);
	ofDrawRectangle(-(w * 0.5), -(cHeight * 0.5), w, cHeight);

	ofSetColor(0, 255, 0); // green
	cHeight = ofMap(c.g, 0, 255, 0, h);
	ofDrawRectangle(-(w * 0.5), -(cHeight * 0.5), w, cHeight);

	ofSetColor(0, 0, 255); // Blue
	cHeight = ofMap(c.b, 0, 255, 0, h);
	ofDrawRectangle(-(w * 0.5), -(cHeight * 0.5), w, cHeight);

	ofPopStyle();
}

void ofApp::drawPixel_style003(float w, float h, ofColor c) {

	ofPushStyle();
	ofFill();

	float maxWidth = w / 3;
	float cWidth;

	ofSetColor(255, 0, 0); // Red
	cWidth = ofMap(c.r, 0, 255, 0, maxWidth);
	ofDrawRectangle(-(cWidth * 0.5), -(h * 0.5), cWidth, h);

	ofSetColor(0, 255, 0); // green
	cWidth = ofMap(c.g, 0, 255, 0, maxWidth);
	ofDrawRectangle(-(maxWidth * 0.5) + maxWidth, -(h * 0.5), cWidth, h);

	ofSetColor(0, 0, 255); // Blue
	cWidth = ofMap(c.b, 0, 255, 0, maxWidth);
	ofDrawRectangle(-(cWidth * 0.5) + (maxWidth*2), -(h * 0.5), cWidth, h);

	ofPopStyle();
}

void ofApp::drawPixel_style004(float w, float h, ofColor c) {

	ofPushStyle();
	ofFill();

	float cWidth;

	ofSetColor(c);
	cWidth = ofMap(c.getBrightness(), 0, 255, 0, w-2);
	ofDrawRectangle(-(cWidth * 0.5), -(h * 0.5), cWidth, h);

	ofPopStyle();
}


//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	drawImage = true;
	if (key == 'p') {
		oneShot = true;
	}
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
