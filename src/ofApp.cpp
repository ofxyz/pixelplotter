#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {
	ofLogToConsole();
	ofSetLogLevel(OF_LOG_VERBOSE);
	ofSetBackgroundAuto(false);
	ofSetCircleResolution(100);

	fbo.allocate(ofGetWidth(), ofGetHeight(), GL_RGBA);
	
	// Setup Listeners Before adding GUI
	// ---------------------------------
	imageDropdown.populateFromDirectory(ofToDataPath("src_img"), { "png", "jpg", "jpeg"});	
	imageDropdown.addListener(this, &ofApp::onImageChange);
	imageDropdown.setSelectedValueByIndex(ofRandom(0, 5), true);

	styleDropdown = make_unique<ofxDropdown>("Plot Style");
	styleDropdown->add("Pixelate");
	styleDropdown->add("Pixelate Brightness Width");
	styleDropdown->add("Lightness Rotation");
	styleDropdown->add("RGB Seperation 1"); // Order Control?
	styleDropdown->add("RGB Seperation 2");
	styleDropdown->add("RGB Seperation 3");
	styleDropdown->add("CMYK Seperation 1");
	styleDropdown->add("CMYK Seperation 2");

	styleDropdown->disableMultipleSelection();
	styleDropdown->enableCollapseOnSelection();
	styleDropdown->setSelectedValueByIndex(ofRandom(0,5), false);

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
	gui.setPosition(10, ofGetHeight() - 300);
	
	gui.add(&imageDropdown);
	gui.add(styleDropdown.get());
	gui.add(blendDropdown.get());

	gui.add(normalise.setup("Normalise Colours", false));

	gui.add(tilesX.setup("Tile Count X", 64, 2, ofGetWidth()/3));
	gui.add(addonx.setup("X addon", 0, -100, 100));
	gui.add(addony.setup("Y addon", 0, -100, 100));
	gui.add(paperColor.setup("Paper Color", ofColor(255, 255, 255), ofColor(0, 0, 0), ofColor(255, 255, 255)));

	gui.add(roundPixels.setup("Round Pixels", false));
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

	if (saveSVG) {
		ofBeginSaveScreenAsSVG("pixelplotted_" + ofGetTimestampString() + ".svg", false);
	}

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
			 
			callStyle(styleDropdown->selectedValue, tileSize + addonx, tileSize + addony, c);

			ofPopMatrix();
		}
	}

	ofDisableBlendMode();

	if (saveSVG) {
		ofEndSaveScreenAsSVG();
		saveSVG = false;
	}

	fbo.end();
}

//--------------------------------------------------------------
void ofApp::draw(){

	ofBackground(paperColor);

	fbo.draw(0, 0);

	if (showImage) {
		img.draw(0, 0);
	}

	gui.draw();

}

void ofApp::callStyle(string stylename, float w, float h, ofColor c) {
	if (stylename == "Pixelate") {
		Style_Pixelate(w, h, c);
	}
	else if (stylename == "Pixelate Brightness Width") {
		Style_Pixelate_Brightness_Width(w, h, c);
	}
	else if (stylename == "Lightness Rotation") {
		Style_Lightness_Rotation(w, h, c);
	}
	else if (stylename == "RGB Seperation 1") {
		Style_RGB_Seperation_1(w, h, c);
	}
	else if (stylename == "RGB Seperation 2") {
		Style_RGB_Seperation_2(w, h, c);
	}
	else if (stylename == "RGB Seperation 3") {
		Style_RGB_Seperation_3(w, h, c);
	}
	else if (stylename == "CMYK Seperation 1") {
		Style_CMYK_Seperation_1(w, h, c);
	}
	else if (stylename == "CMYK Seperation 2") {
		Style_CMYK_Seperation_2(w, h, c);
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
void ofApp::Style_Pixelate(float w, float h, ofColor c) {
	ofPushStyle();
	ofFill();
	ofSetColor(c);
	if (roundPixels) {
		ofDrawEllipse(0, 0, w, h);
	}
	else {
		ofDrawRectangle(-(w * 0.5), -(h * 0.5), w, h);
	}
	ofPopStyle();
}

//--------------------------------------------------------------
void ofApp::Style_Lightness_Rotation( float w, float h, ofColor c) {
	ofPushMatrix();
	ofRotateZDeg(ofMap(c.getLightness(), 0, 255, 0, 360));
	Style_Pixelate(w, h, c);
	ofPopMatrix();
}

//--------------------------------------------------------------
void ofApp::Style_RGB_Seperation_1(float w, float h, ofColor c) {

	float cHeight;

	cHeight = ofMap(c.r, 0, 255, 0, h);
	Style_Pixelate(w, cHeight, ofColor(255, 0, 0));

	cHeight = ofMap(c.g, 0, 255, 0, h);
	Style_Pixelate(w, cHeight, ofColor(0, 255, 0));

	cHeight = ofMap(c.b, 0, 255, 0, h);
	Style_Pixelate(w, cHeight, ofColor(0, 0, 255));

}

void ofApp::Style_RGB_Seperation_2(float w, float h, ofColor c) {

	float cWidth, cHeight;

	cWidth  = ofMap(c.r, 0, 255, 0, w);
	cHeight = ofMap(c.r, 0, 255, 0, h);
	Style_Pixelate(cWidth, cHeight, ofColor(255, 0, 0));

	cWidth  = ofMap(c.g, 0, 255, 0, w);
	cHeight = ofMap(c.g, 0, 255, 0, h);
	Style_Pixelate(cWidth, cHeight, ofColor(0, 255, 0));

	cWidth  = ofMap(c.b, 0, 255, 0, w);
	cHeight = ofMap(c.b, 0, 255, 0, h);
	Style_Pixelate(cWidth, cHeight, ofColor(0, 0, 255));
}

void ofApp::Style_RGB_Seperation_3(float w, float h, ofColor c) {

	float maxWidth = w / 3;
	float cWidth;

	cWidth = ofMap(c.r, 0, 255, 0, maxWidth);
	Style_Pixelate(cWidth, h, ofColor(255, 0, 0)); // Red

	ofPushMatrix(); // offset

	ofTranslate(maxWidth, 0, 0);
	cWidth = ofMap(c.g, 0, 255, 0, maxWidth);
	Style_Pixelate(cWidth, h, ofColor(0, 255, 0)); // Green

	ofTranslate(maxWidth, 0, 0);
	cWidth = ofMap(c.b, 0, 255, 0, maxWidth);
	Style_Pixelate(cWidth, h, ofColor(0, 0, 255)); // Blue

	ofPopMatrix();
}

void ofApp::Style_Pixelate_Brightness_Width(float w, float h, ofColor c) {

	float cWidth = ofMap(c.getBrightness(), 0, 255, 0, w);
	Style_Pixelate(cWidth, h, c);

}


//--------------------------------------------------------------
void ofApp::Style_CMYK_Seperation_1(float w, float h, ofColor c) {
	float cHeight;
	ofVec4f cmyk = getCMYK(c);

	cHeight = ofMap(cmyk[0], 0, 1, 0, h);
	Style_Pixelate(w, cHeight, ofColor(0, 174, 239)); // Cyan

	cHeight = ofMap(cmyk[1], 0, 1, 0, h);
	Style_Pixelate(w, cHeight, ofColor(236, 0, 140)); // Magenta

	cHeight = ofMap(cmyk[2], 0, 1, 0, h);
	Style_Pixelate(w, cHeight, ofColor(255, 242, 0)); // Yellow

	cHeight = ofMap(cmyk[3], 0, 1, 0, h);
	Style_Pixelate(w, cHeight, ofColor(0, 0, 0)); // Black

}

//--------------------------------------------------------------
void ofApp::Style_CMYK_Seperation_2(float w, float h, ofColor c) {
	float cWidth, cHeight;
	ofVec4f cmyk = getCMYK(c);

	cWidth  = ofMap(cmyk[0], 0, 1, 0, w);
	cHeight = ofMap(cmyk[0], 0, 1, 0, h);
	Style_Pixelate(cWidth, cHeight, ofColor(0, 174, 239)); // Cyan

	cWidth  = ofMap(cmyk[1], 0, 1, 0, w);
	cHeight = ofMap(cmyk[1], 0, 1, 0, h);
	Style_Pixelate(cWidth, cHeight, ofColor(236, 0, 140)); // Magenta

	cWidth  = ofMap(cmyk[2], 0, 1, 0, w);
	cHeight = ofMap(cmyk[2], 0, 1, 0, h);
	Style_Pixelate(cWidth, cHeight, ofColor(255, 242, 0)); // Yellow

	cWidth  = ofMap(cmyk[3], 0, 1, 0, w);
	cHeight = ofMap(cmyk[3], 0, 1, 0, h);
	Style_Pixelate(cWidth, cHeight, ofColor(0, 0, 0)); // Black

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
	saveSVG = true;
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	updateFbo();
	if (key == 'p') {
		saveSVG = true;
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
	if (dragInfo.files.size() > 0) {
		loadImage(dragInfo.files[0]);	
	}
}
