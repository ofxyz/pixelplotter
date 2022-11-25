#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {
	ofLogToConsole();
	ofSetLogLevel(OF_LOG_VERBOSE);
	ofSetBackgroundAuto(false);

	fbo.allocate(ofGetWidth(), ofGetHeight(), GL_RGBA);
	
	// Setup Listeners Before adding GUI
	// ---------------------------------
	imageDropdown.populateFromDirectory(ofToDataPath("src_img"), { "png", "jpg", "jpeg"});	
	imageDropdown.addListener(this, &ofApp::onImageChange);
	imageDropdown.setSelectedValueByIndex(0, true);

	styleDropdown = make_unique<ofxDropdown>("Plot Style");
	for (int i = 0; i <= 5; i++) {
		styleDropdown->add("Style " + std::to_string(i));
	}
	styleDropdown->disableMultipleSelection();
	styleDropdown->enableCollapseOnSelection();
	styleDropdown->setSelectedValueByIndex(0, false);

	blendDropdown = make_unique<ofxDropdown>("Blend Mode");
	blendDropdown->add("OF_BLENDMODE_DISABLED");
	blendDropdown->add("OF_BLENDMODE_ALPHA");
	blendDropdown->add("OF_BLENDMODE_ADD");
	blendDropdown->add("OF_BLENDMODE_SUBTRACT");
	blendDropdown->add("OF_BLENDMODE_MULTIPLY");
	blendDropdown->add("OF_BLENDMODE_SCREEN");
	blendDropdown->disableMultipleSelection();
	blendDropdown->enableCollapseOnSelection();
	blendDropdown->setSelectedValueByIndex(0, false);

	exportSVG.addListener(this, &ofApp::gui_exportSVG_pressed);
	// ---------------------------------

	gui.setup("Pixel Plotter", "guiSettings.xml");
	gui.setPosition(ofGetWidth() - gui_width, ofGetHeight() - 200);
	
	gui.add(&imageDropdown);
	gui.add(styleDropdown.get());
	gui.add(blendDropdown.get());

	gui.add(normalise.setup("Normalise Colours", false));

	gui.add(tilesX.setup("Tile Count X", 64, 2, ofGetWidth()/3));
	gui.add(paperColor.setup("Paper Color", ofColor(200, 200, 200), ofColor(0, 0, 0), ofColor(255, 255, 255)));

	gui.add(showImage.setup("Show Image", false));
	gui.add(exportSVG.setup("Export SVG"));
	
	// ---------------------------------

	ofBackground(paperColor);
}

//--------------------------------------------------------------
void ofApp::exit() {
	//clean
	exportSVG.removeListener(this, &ofApp::gui_exportSVG_pressed);
	imageDropdown.removeListener(this, &ofApp::onImageChange);
	return;
}

//--------------------------------------------------------------
void ofApp::update() {
	if (ofGetFrameNum() % 250 == 0) updateFbo();
}

void ofApp::updateFbo() {
	fbo.begin();
	ofClear(paperColor);
	setBlendmode();

	int w = img.getWidth();
	int h = img.getHeight();
	float tileSize = w / tilesX;

	float halfTile = (float)tileSize / 2.0;

	int yCount = 0;
	for (int y = 0; y < h; y += tileSize) {
		yCount++;
		int xCount = 0;
		for (int x = 0; x < w; x += tileSize) {
			xCount++;

			float centerx = x + halfTile;
			float centery = y + halfTile;

			ofColor c = img.getPixels().getColor(x, y);

			//if (l >= 250) continue;

			if (normalise) {
				c.normalize();
			}

			//c.setSaturation(255);

			ofPushMatrix();
			ofTranslate(centerx, centery, 0);

			//float lineLength = halfTile + (abs(ofRandomf())* tileSize);

			callStyle(styleDropdown->selectedValue, tileSize, tileSize, c);

			ofPopMatrix();
		}
	}

	ofDisableBlendMode();
	fbo.end();
}

//--------------------------------------------------------------
void ofApp::draw(){

	ofBackground(paperColor);
		
	if (showImage) {
		img.draw(0, 0);
	}

	if (oneShot) {
		ofBeginSaveScreenAsSVG("pixelplotted_" + ofGetTimestampString() + ".svg", false);
	}

	fbo.draw(0, 0);

	if (oneShot) {
		ofEndSaveScreenAsSVG();
		oneShot = false;
	}

	gui.draw();
}

void ofApp::callStyle(string stylename, float w, float h, ofColor c) {

	if (stylename == "Style 1") {
		Style_1(w, h, c);
	}
	else if (stylename == "Style 2") {
		Style_2(w, h, c);
	}
	else if (stylename == "Style 3") {
		Style_3(w, h, c);
	}
	else if (stylename == "Style 4") {
		Style_4(w, h, c);
	}
	else if (stylename == "Style 5") {
		Style_5(w, h, c);
	}
	else {
		Style_0(w, h, c);
	}
}

void ofApp::setBlendmode() {
	string currentBlendmode = blendDropdown->selectedValue;

	if (currentBlendmode == "OF_BLENDMODE_ALPHA") {
		ofEnableBlendMode(OF_BLENDMODE_ALPHA);
	}
	else if (currentBlendmode == "OF_BLENDMODE_ADD") {
		ofEnableBlendMode(OF_BLENDMODE_ADD);
	}
	else if (currentBlendmode == "OF_BLENDMODE_SUBTRACT") {
		ofEnableBlendMode(OF_BLENDMODE_SUBTRACT);
	}
	else if (currentBlendmode == "OF_BLENDMODE_MULTIPLY") {
		ofEnableBlendMode(OF_BLENDMODE_MULTIPLY);
	}
	else if (currentBlendmode == "OF_BLENDMODE_SCREEN") {
		ofEnableBlendMode(OF_BLENDMODE_SCREEN);
	}
	else {
		ofEnableBlendMode(OF_BLENDMODE_DISABLED);
	}
}

//--------------------------------------------------------------
void ofApp::Style_0(float w, float h, ofColor c) {
	ofPushStyle();
	ofFill();
	ofSetColor(c);
	ofDrawRectangle(-(w * 0.5), -(h * 0.5), w, h);
	ofPopStyle();
}

//--------------------------------------------------------------
void ofApp::Style_1( float w, float h, ofColor c) {
	ofPushMatrix();
	ofRotateZDeg(ofMap(c.getLightness(), 0, 255, 0, 360));
	Style_0(w, h, c);
	ofPopMatrix();
}

//--------------------------------------------------------------
void ofApp::Style_2(float w, float h, ofColor c) {

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

void ofApp::Style_3(float w, float h, ofColor c) {

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

void ofApp::Style_4(float w, float h, ofColor c) {

	ofPushStyle();
	ofFill();

	float cWidth;

	ofSetColor(c);
	cWidth = ofMap(c.getBrightness(), 0, 255, 0, w-2);
	ofDrawRectangle(-(cWidth * 0.5), -(h * 0.5), cWidth, h);

	ofPopStyle();
}


//--------------------------------------------------------------
void ofApp::Style_5(float w, float h, ofColor c) {
	ofPushStyle();
	ofFill();

	float cHeight;
	ofVec4f cmyk = getCMYK(c);

	ofSetColor(0, 174, 239); // Cyan
	cHeight = ofMap(cmyk[0], 0, 1, 0, h);
	ofDrawRectangle(-(w * 0.5), -(cHeight * 0.5), w, cHeight);

	ofSetColor(236, 0, 140); // Magenta
	cHeight = ofMap(cmyk[1], 0, 1, 0, h);
	ofDrawRectangle(-(w * 0.5), -(cHeight * 0.5), w, cHeight);

	ofSetColor(255, 242, 0); // Yellow
	cHeight = ofMap(cmyk[2], 0, 1, 0, h);
	ofDrawRectangle(-(w * 0.5), -(cHeight * 0.5), w, cHeight);

	ofSetColor(0, 0, 0); // Black
	cHeight = ofMap(cmyk[3], 0, 1, 0, h);
	ofDrawRectangle(-(w * 0.5), -(cHeight * 0.5), w, cHeight);

	ofPopStyle();
}

void ofApp::loadImage(string& filepath) {
	img.load(filepath);

	(img.getWidth() >= img.getHeight()) ? isLandscape = true : isLandscape = false;

	if (isLandscape) {
		imgRatio = float(img.getHeight()) / float(img.getWidth());
		img.resize(ofGetWidth(), ofGetHeight() * imgRatio);
	}
	else {
		imgRatio = float(img.getWidth()) / float(img.getHeight());
		img.resize(ofGetWidth() * imgRatio, ofGetHeight());
	}

}

void ofApp::onImageChange(string& filepath) {
	loadImage(filepath);
}

ofVec4f ofApp::getCMYK(ofColor rgb) {
	double dr = (double)rgb.r / 255;
	double dg = (double)rgb.g / 255;
	double db = (double)rgb.b / 255;
	double k = 1 - max(max(dr, dg), db);
	double c = (1 - dr - k) / (1 - k);
	double m = (1 - dg - k) / (1 - k);
	double y = (1 - db - k) / (1 - k);

	return ofVec4f(c, m, y, k);
}

//--------------------------------------------------------------
void ofApp::gui_exportSVG_pressed() {
	oneShot = true;
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	updateFbo();
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
