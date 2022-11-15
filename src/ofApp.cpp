#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {
	ofLogToConsole();
	ofSetLogLevel(OF_LOG_VERBOSE);
	ofSetBackgroundAuto(false);

	oneShot   = true;
	drawImage = true;
	showImage = false;

	paperColor = ofColor(255, 255, 255);

	img_name = "treeman texture_01C_smaller.png";
	img.load("src_img/"+ img_name);

	//(img.getWidth() > img.getHeight()) ? isLandscape = true : isLandscape = false;

	imgRatio = float(img.getWidth()) / float(img.getHeight());
	//img.resize(32* imgRatio, ofGetHeight());
	img.resize(64*4, 64*4);

	tilesX = 64*4;
	tilesY = 64*4;

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
				float l = c.getLightness();

				//c.normalize();
				//c.setSaturation(255);

				ofPushMatrix();
				ofTranslate(centerx, centery, 0);
				ofRotateZDeg(ofMap(l, 0, 255, 0, 360) );
				
				ofSetColor(c);
				//ofSetLineWidth(100);
				//ofDrawLine(-halfTile, 0, halfTile, 0);
				float lineLength = halfTile + (abs(ofRandomf())* tileSize);
				float thickness = halfTile;
				ofDrawRectangle(-(lineLength*0.5), -(thickness * 0.5), lineLength, thickness);

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
