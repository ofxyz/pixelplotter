#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {
	ofLogToConsole();
	ofSetLogLevel(OF_LOG_VERBOSE);
	ofSetBackgroundAuto(false);

	oneShot   = false;
	drawImage = true;

	showImage = false;

	penColor = ofColor(0, 0, 0);
	paperColor = ofColor(250, 250, 250);

	img_name = "32-32";
	img_ext  = "png";
	img.load("src_img/"+ img_name + "." + img_ext);

	//(img.getWidth() > img.getHeight()) ? isLandscape = true : isLandscape = false;

	//imgRatio = float(img.getWidth()) / float(img.getHeight());
	//img.resize(ofGetHeight()* imgRatio, ofGetHeight());
	//img.resize(ofGetHeight(), ofGetHeight());

	tilesX = 32;
	tilesY = 32;//imgRatio * tilesX;
	ofSetBackgroundAuto(false);
	//ofBackground(paperColor);
	tileSize = ofGetWidth() / tilesX;
	ofLog() << "Tilesize: " << tileSize;
}

//--------------------------------------------------------------
void ofApp::update(){
	
}

//--------------------------------------------------------------
void ofApp::draw(){
	
	if (drawImage) {
		
		//ofBackground(paperColor);
		
		if (showImage) {
			img.draw(0, 0);
		}

		if (oneShot) {
			ofBeginSaveScreenAsPDF("pixelplotted_" + img_name + "_" + ofGetTimestampString() + ".pdf", false);
			ofBeginSaveScreenAsSVG("pixelplotted_" + img_name + "_" + ofGetTimestampString() + ".svg", false);
		}
		
		ofNoFill();

		pixels = img.getPixels();
		int w = img.getWidth();
		int h = img.getHeight();

		float halfTile = (float)tileSize / 2.0;

		int yCount = 0;
		for (int y = 0; y < h; y += 1) {
			yCount++;
			int xCount = 0;
			for (int x = 0; x < w; x += 1) {
				xCount++;

				int aX = (x * tileSize);
				int aY = (y * tileSize);

				float centerx = aX + halfTile;
				float centery = aY + halfTile;

				ofColor c = pixels.getColor( x, y);
				float l = c.getHueAngle();
				
				ofSetColor(c);
				ofSetLineWidth(halfTile);

				ofPushMatrix();
				ofTranslate(centerx, centery, 0);
				ofRotateZDeg(c.getHueAngle());
				ofDrawLine(-halfTile, 0, halfTile, 0);
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
