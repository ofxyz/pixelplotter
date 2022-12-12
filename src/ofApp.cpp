#include "ofApp.h"

/*
   
   Make some dots
   
   Posterise to a pallette. Set high mid low to start?
   Or dumb down the colours then ability to remap?

   // Add Polkadots

   // RGB needs K
   // Add zoom function for closeup view bases on mouse x/y

   // GUI update on every change. ofxGUI doesn't seem to be able to do that.
   // Use external functions or change to ImGUI?

   // Add CMYK rotate

*/

//--------------------------------------------------------------
void ofApp::setup() {
	ofLogToConsole();
	ofSetLogLevel(OF_LOG_VERBOSE);
	ofSetBackgroundAuto(false);
	ofSetCircleResolution(100);

	
	
	// Setup Listeners Before adding GUI
	// ---------------------------------
	imageDropdown.populateFromDirectory(ofToDataPath("src_img"), { "gif", "png", "jpg", "jpeg"});	
	imageDropdown.addListener(this, &ofApp::onImageChange);
	imageDropdown.setSelectedValueByIndex(ofRandom(0, 5), true);

	styleDropdown = make_unique<ofxDropdown>("Plot Style");
	styleDropdown->add("Pixelate");
	styleDropdown->add("Pixelate Brightness Width");
	styleDropdown->add("Lightness Rotation");
	styleDropdown->add("RGB Seperation 1"); // Order Control?
	styleDropdown->add("RGB Seperation 2");
	styleDropdown->add("RGB Seperation 3");
	styleDropdown->add("RGB Seperation 4");
	styleDropdown->add("RGB Seperation 5");

	styleDropdown->add("CMYK Seperation 1");
	styleDropdown->add("CMYK Seperation 2");
	styleDropdown->add("CMYK Seperation 3");
	styleDropdown->add("CMYK Seperation 4");
	styleDropdown->add("CMYK Seperation 5");
	styleDropdown->add("CMYK Seperation 6");
	styleDropdown->add("CMYK Seperation 7");
	styleDropdown->add("CMYK Seperation 8");
	styleDropdown->add("CMYK Seperation 9");
	styleDropdown->add("CMYK Seperation 10");
	styleDropdown->add("CMYK Seperation 11");

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

	setRGB.addListener(this, &ofApp::gui_setRGB_pressed);
	setCMYK.addListener(this, &ofApp::gui_setCMYK_pressed);
	setPosterize.addListener(this, &ofApp::gui_setPosterize_pressed);
	exportSVG.addListener(this, &ofApp::gui_exportSVG_pressed);

	// ---------------------------------

	gui.setup("Pixel Plotter", "guiSettings.xml");
	gui.setPosition(10, 10);
	
	gui.add(&imageDropdown);
	gui.add(styleDropdown.get());
	gui.add(blendDropdown.get());

	gui.add(normalise.setup("Normalise Colours", false));

	gui.add(tilesX.setup("Tile Count X", 64, 2, ofGetWidth() / 3));
	gui.add(tilesY.setup("Tile Count Y", 64, 2, ofGetWidth() / 3));

	gui.add(addonx.setup("X addon", 0, -100, 100));
	gui.add(addony.setup("Y addon", 0, -100, 100));

	gui.add(paperColor.setup("Paper Color", ofColor(255, 255, 255), ofColor(0, 0, 0), ofColor(255, 255, 255)));
	gui.add(magentaRed.setup("Magenta / Red", ofColor(236, 0, 140), ofColor(0, 0, 0), ofColor(255, 255, 255)));
	gui.add(cyanBlue.setup("Cyan / Blue", ofColor(0, 174, 239), ofColor(0, 0, 0), ofColor(255, 255, 255)));
	gui.add(yellowGreen.setup("Yellow / Green", ofColor(255, 242, 0), ofColor(0, 0, 0), ofColor(255, 255, 255)));
	gui.add(black.setup("Black", ofColor(0, 0, 0), ofColor(0, 0, 0), ofColor(255, 255, 255)));

	gui.add(setRGB.setup("Set RGB"));
	gui.add(setCMYK.setup("Set CMYK"));
	gui.add(setPosterize.setup("Set Posterize"));

	gui.add(roundPixels.setup("Round Pixels", false));

	gui.add(randomOffset.setup("Random Offset", 0, 0, 10));

	gui.add(showImage.setup("Show Image", false));
	gui.add(showZoom.setup("Show Zoom", false));
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
	if (ofGetFrameNum() % 500 == 0) updateFbo();
}

void ofApp::updateFbo() {
	fbo.allocate(ofGetWidth(), ofGetHeight(), GL_RGBA);
	fbo.begin();
	ofClear(paperColor);

	if (saveSVG) {
		ofBeginSaveScreenAsPDF("pixelplotted_" + ofGetTimestampString() + ".pdf", false);
	}

	setBlendmode();

	int w = img.getWidth();
	int h = img.getHeight();
	float tileW = (float)w / (float)tilesX;
	float tileH = (float)h / (float)tilesY;
	float halfTileW = tileW / 2.0;
	float halfTileH = tileH / 2.0;

	/* Process All Pixels of Original Image 
	int o_w = original.getWidth();
	int o_h = original.getHeight();
	
	for (int y = 0; y < o_h; y += 1) {
		for (int x = 0; x < o_w; x += 1) {
			ofColor c = original.getPixels().getColor(x, y);
			if (normalise) {
				c.normalize();
			}

			ofPushMatrix();
			ofTranslate((x * tileW) + halfTileW, (y * tileH) + halfTileH, 0);

			float rotation = ofMap(x, 0, o_w, -360, 360);
			callStyle(styleDropdown->selectedValue, tileW + addonx, tileH + addony, c, rotation);

			ofPopMatrix();

		}
	}
	*/
	int ycount = 0;
	int xcount = 0;
	for (float y = 0; y < h - halfTileW; y += tileH) {
		for (float x = 0; x < w - halfTileW; x += tileW) {
			int cx = floor(x + halfTileW);
			int cy = floor(y + halfTileW);
			ofColor c = img.getPixels().getColor(cx, cy);

			if (normalise) {
				c.normalize();
			}

			ofPushMatrix();
			ofTranslate(cx, cy, 0);
			callStyle(styleDropdown->selectedValue, ofVec2f(tileW + addonx, tileH + addony), ofVec2f(cx, cy), ofVec2f(xcount, ycount), c);
			ofPopMatrix();
			xcount++;
		}
		ycount++;
	}

	ofDisableBlendMode();

	if (saveSVG) {
		ofEndSaveScreenAsPDF();
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

void ofApp::callStyle(string stylename, ofVec2f size, ofVec2f loc, ofDefaultVec2 xycount, ofColor c) {
	float w = size[0];
	float h = size[1];

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
	else if (stylename == "RGB Seperation 4") {
		Style_RGB_Seperation_4(w, h, c);
	}
	else if (stylename == "RGB Seperation 5") {
		Style_RGB_Seperation_5(w, h, c);
	}
	else if (stylename == "CMYK Seperation 1") {
		Style_CMYK_Seperation_1(w, h, c);
	}
	else if (stylename == "CMYK Seperation 2") {
		Style_CMYK_Seperation_2(w, h, c);
	}
	else if (stylename == "CMYK Seperation 3") {
		Style_CMYK_Seperation_3(w, h, c);
	}
	else if (stylename == "CMYK Seperation 4") {
		Style_CMYK_Seperation_4(w, h, c);
	}
	else if (stylename == "CMYK Seperation 5") {
		Style_CMYK_Seperation_5(w, h, c);
	}
	else if (stylename == "CMYK Seperation 6") {
		Style_CMYK_Seperation_6(w, h, c);
	}
	else if (stylename == "CMYK Seperation 7") {
		Style_CMYK_Seperation_7(w, h, c);
	}
	else if (stylename == "CMYK Seperation 8") {
		Style_CMYK_Seperation_8(w, h, c);
	}
	else if (stylename == "CMYK Seperation 9") {
		Style_CMYK_Seperation_9(w, h, c);
	}
	else if (stylename == "CMYK Seperation 10") {
		Style_CMYK_Seperation_10(w, h, c, loc);
	}
	else if (stylename == "CMYK Seperation 11") {
		Style_CMYK_Seperation_11(w, h, c, xycount);
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

	float offsetX = ofRandom(-randomOffset, randomOffset);
	float offsetY = ofRandom(-randomOffset, randomOffset);

	ofPushStyle();
	ofFill();
	ofSetColor(c);
	if (roundPixels) {
		if (abs(w) < 2 || abs(h) < 2) {
			ofPopStyle();
			return;
		}
		ofDrawEllipse(offsetX, offsetY, w, h);
	}
	else {
		if (abs(w) < 0.25 || abs(h) < 0.25) {
			ofPopStyle();
			return;
		}
		ofDrawRectangle(-(w * 0.5)+ offsetX, -(h * 0.5)+ offsetX, w, h);
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
	Style_Pixelate(w, cHeight, magentaRed);

	cHeight = ofMap(c.g, 0, 255, 0, h);
	Style_Pixelate(w, cHeight, yellowGreen);

	cHeight = ofMap(c.b, 0, 255, 0, h);
	Style_Pixelate(w, cHeight, cyanBlue);

}

void ofApp::Style_RGB_Seperation_2(float w, float h, ofColor c) {

	float cWidth, cHeight;

	cWidth  = ofMap(c.r, 0, 255, 0, w);
	cHeight = ofMap(c.r, 0, 255, 0, h);
	Style_Pixelate(cWidth, cHeight, magentaRed);

	cWidth  = ofMap(c.g, 0, 255, 0, w);
	cHeight = ofMap(c.g, 0, 255, 0, h);
	Style_Pixelate(cWidth, cHeight, yellowGreen);

	cWidth  = ofMap(c.b, 0, 255, 0, w);
	cHeight = ofMap(c.b, 0, 255, 0, h);
	Style_Pixelate(cWidth, cHeight, cyanBlue);
}

void ofApp::Style_RGB_Seperation_3(float w, float h, ofColor c) {

	float maxWidth = w / 3;
	float cWidth;

	cWidth = ofMap(c.r, 0, 255, 0, maxWidth);
	Style_Pixelate(cWidth, h, magentaRed); // Red

	ofPushMatrix(); // offset

	ofTranslate(maxWidth, 0, 0);
	cWidth = ofMap(c.g, 0, 255, 0, maxWidth);
	Style_Pixelate(cWidth, h, yellowGreen); // Green

	ofTranslate(maxWidth, 0, 0);
	cWidth = ofMap(c.b, 0, 255, 0, maxWidth);
	Style_Pixelate(cWidth, h, cyanBlue); // Blue

	ofPopMatrix();
}

//--------------------------------------------------------------
void ofApp::Style_RGB_Seperation_4(float w, float h, ofColor c) {

	float cWidth;
	float maxC = 765; // 255 * 3;

	float left = maxC - (c.r + c.g + c.b);
	float addon = ceil(left / 3);

	ofPushMatrix(); // offset

	cWidth = ofMap(c.r + addon, 0, maxC, 0, w);
	ofTranslate(-(w*0.5)+(cWidth*0.5), 0, 0);
	Style_Pixelate(cWidth, h, magentaRed);

	ofTranslate(cWidth*0.5, 0, 0);
	cWidth = ofMap(c.g + addon, 0, maxC, 0, w);
	ofTranslate(cWidth * 0.5, 0, 0);
	Style_Pixelate(cWidth, h, yellowGreen);

	ofTranslate(cWidth * 0.5, 0, 0);
	cWidth = ofMap(c.b + addon, 0, maxC, 0, w);
	ofTranslate(cWidth * 0.5, 0, 0);
	Style_Pixelate(cWidth, h, ofColor(0, 0, 255));

	ofPopMatrix();

}

void ofApp::Style_RGB_Seperation_5(float w, float h, ofColor c) {
	float cHeight;
	ofVec4f cmyk = getCMYK(c);
	Style_Pixelate(w, h, c);

	cHeight = ofMap(cmyk[3], 0, 1, 0, h);
	Style_Pixelate(w, cHeight, black); // Black

	cHeight = ofMap(c.r, 0, 255, 0, h / 4);
	ofPushMatrix();
	ofTranslate(0, (-h * 0.5) + (cHeight * 0.5), 0);
	Style_Pixelate(w, cHeight, ofColor(255, 0, 0, c.getBrightness())); // Red

	ofTranslate(0, cHeight * 0.5, 0);

	cHeight = ofMap(c.g, 0, 255, 0, h / 4);
	ofTranslate(0, cHeight * 0.5, 0);
	Style_Pixelate(w, cHeight, ofColor(0, 255, 0, c.getBrightness())); // Green

	ofTranslate(0, cHeight * 0.5, 0);

	cHeight = ofMap(c.b, 0, 255, 0, h / 4);
	ofTranslate(0, cHeight * 0.5, 0);
	Style_Pixelate(w, cHeight, ofColor(0, 0, 255, c.getBrightness())); // Blue

	ofPopMatrix();

}

void ofApp::Style_Pixelate_Brightness_Width(float w, float h, ofColor c) {

	float cWidth = ofMap(c.getBrightness(), 0, 255, 0, w);
	Style_Pixelate(cWidth, h, c);

}

//--------------------------------------------------------------
void ofApp::Style_CMYK_Seperation_2(float w, float h, ofColor c) {
	float cHeight;
	ofVec4f cmyk = getCMYK(c);

	float maxOffset = w*0.5;
	float minOffset = -maxOffset;

	ofPushMatrix();
	ofTranslate(ofRandom(minOffset, maxOffset), ofRandom(minOffset, maxOffset), 0);
	cHeight = ofMap(cmyk[0], 0, 1, 0, h);
	Style_Pixelate(cHeight, cHeight, cyanBlue); // Cyan
	ofPopMatrix();

	ofPushMatrix();
	ofTranslate(ofRandom(minOffset, maxOffset), ofRandom(minOffset, maxOffset), 0);
	cHeight = ofMap(cmyk[1], 0, 1, 0, h);
	Style_Pixelate(cHeight, cHeight, magentaRed); // Magenta
	ofPopMatrix();

	ofPushMatrix();
	ofTranslate(ofRandom(minOffset, maxOffset), ofRandom(minOffset, maxOffset), 0);
	cHeight = ofMap(cmyk[2], 0, 1, 0, h);
	Style_Pixelate(cHeight, cHeight, yellowGreen); // Yellow
	ofPopMatrix();

	ofPushMatrix();
	ofTranslate(ofRandom(minOffset, maxOffset), ofRandom(minOffset, maxOffset), 0);
	cHeight = ofMap(cmyk[3], 0, 1, 0, h);
	Style_Pixelate(cHeight, cHeight, black); // Black
	ofPopMatrix();
}

//--------------------------------------------------------------
void ofApp::Style_CMYK_Seperation_1(float w, float h, ofColor c) {
	float cWidth, cHeight;
	ofVec4f cmyk = getCMYK(c);

	cWidth = ofMap(cmyk[2], 0, 1, 0, w);
	cHeight = ofMap(cmyk[2], 0, 1, 0, h);

	Style_Pixelate(cWidth, cHeight, yellowGreen); // Yellow

	cWidth  = ofMap(cmyk[0], 0, 1, 0, w);
	cHeight = ofMap(cmyk[0], 0, 1, 0, h);
	Style_Pixelate(cWidth, cHeight,cyanBlue); // Cyan

	cWidth  = ofMap(cmyk[1], 0, 1, 0, w);
	cHeight = ofMap(cmyk[1], 0, 1, 0, h);
	Style_Pixelate(cWidth, cHeight, magentaRed); // Magenta

	cWidth  = ofMap(cmyk[3], 0, 1, 0, w);
	cHeight = ofMap(cmyk[3], 0, 1, 0, h);
	Style_Pixelate(cWidth, cHeight, black); // Black

}

//--------------------------------------------------------------
void ofApp::Style_CMYK_Seperation_3(float w, float h, ofColor c) {
	float cWidth;
	ofVec4f cmyk = getCMYK(c);

	cWidth = ofMap(cmyk[0], 0, 1, 0, w);
	Style_Pixelate(cWidth, cWidth, cyanBlue); // Cyan

	cWidth = ofMap(cmyk[1], 0, 1, 0, w);
	Style_Pixelate(cWidth, cWidth, magentaRed); // Magenta

	cWidth = ofMap(cmyk[2], 0, 1, 0, w);
	Style_Pixelate(cWidth, cWidth, yellowGreen); // Yellow

	cWidth = ofMap(cmyk[3], 0, 1, 0, w);
	Style_Pixelate(cWidth, cWidth, black); // Black

}

//--------------------------------------------------------------
void ofApp::Style_CMYK_Seperation_4(float w, float h, ofColor c) {
	float cWidth;
	ofVec4f cmyk = getCMYK(c);

	cWidth = ofMap(cmyk[3], 0, 1, 0, w/2);

	if (cWidth > w * 0.1) {
		Style_Pixelate(cWidth, h, black); // Black
	}
	

	ofPushMatrix(); // offset

	cWidth = ofMap(cmyk[0], 0, 1, 0, w/2);
	ofTranslate(-(w * 0.5) + (cWidth * 0.5), 0, 0);
	Style_Pixelate(cWidth, h, cyanBlue); // Cyan
	
	ofTranslate(cWidth * 0.5, 0, 0);

	cWidth = ofMap(cmyk[1], 0, 1, 0, w/2);
	ofTranslate(cWidth * 0.5, 0, 0);
	Style_Pixelate(cWidth, h, magentaRed); // Magenta
	
	ofTranslate(cWidth * 0.5, 0, 0);

	cWidth = ofMap(cmyk[2], 0, 1, 0, w/2);
	ofTranslate(cWidth * 0.5, 0, 0);
	Style_Pixelate(cWidth, h, yellowGreen); // Yellow

	ofPopMatrix();
}

//--------------------------------------------------------------
void ofApp::Style_CMYK_Seperation_5(float w, float h, ofColor c) {
	float cHeight;
	ofVec4f cmyk = getCMYK(c);

	cHeight = ofMap(cmyk[3], 0, 1, 0, h);
	if (cHeight > h * 0.1) {
		Style_Pixelate(w, cHeight, black); // Black
	}
	
	cHeight = ofMap(cmyk[2], 0, 1, 0, h);
	Style_Pixelate(w, cHeight, yellowGreen); // Yellow

	cHeight = ofMap(cmyk[0], 0, 1, 0, h);
	ofPushMatrix();
	ofTranslate(0, (h * 0.5)-(cHeight*0.5), 0);
	Style_Pixelate(w, cHeight, cyanBlue); // Cyan
	ofPopMatrix();

	cHeight = ofMap(cmyk[1], 0, 1, 0, h);
	ofPushMatrix();
	ofTranslate(0, (-h * 0.5) + (cHeight * 0.5), 0);
	Style_Pixelate(w, cHeight, magentaRed); // Magenta
	ofPopMatrix();

}

//--------------------------------------------------------------
void ofApp::Style_CMYK_Seperation_6(float w, float h, ofColor c) {
	float cHeight;
	ofVec4f cmyk = getCMYK(c);

	cHeight = ofMap(cmyk[3], 0, 1, 2, h-2);
	if (cHeight > h * 0.5) {
		Style_Pixelate(w, cHeight, black); // Black
	}

	cHeight = ofMap(cmyk[0], 0, 1, 2, h-2);
	ofPushMatrix();
	ofTranslate(0, ((-h - 2) * 0.5) - (cHeight * 0.5), 0);
	ofColor cc = cyanBlue;
	cc.setBrightness(130);
	Style_Pixelate(w, cHeight, cc); // Cyan
	ofPopMatrix();

	cHeight = ofMap(cmyk[1], 0, 1, 2, h-2);
	ofPushMatrix();
	ofTranslate(0, ((-h-2) * 0.5) + (cHeight * 0.5), 0);
	cc = magentaRed;
	cc.setBrightness(130);
	Style_Pixelate(w, cHeight, cc); // Magenta
	ofPopMatrix();

	cHeight = ofMap(cmyk[2], 0, 1, 2, h / 2);
	cc = yellowGreen;
	cc.setBrightness(130);
	Style_Pixelate(w, cHeight, cc);
}

float ofApp::percentage(float percent, float total) {
	return (percent / 100) * total;
}

//--------------------------------------------------------------
void ofApp::Style_CMYK_Seperation_7(float w, float h, ofColor c) {
	float cHeight;
	ofVec4f cmyk = getCMYK(c);

	float total = cmyk[0] + cmyk[1] + cmyk[2];
	float pc = (100 * cmyk[0]) / total;
	float pm = (100 * cmyk[1]) / total;
	float py = (100 * cmyk[2]) / total;

	cHeight = percentage(pc, h);
	ofPushMatrix();
	ofTranslate(0, (-h * 0.5) + (cHeight * 0.5), 0);
	Style_Pixelate(w, cHeight, cyanBlue); // Cyan
	
	ofTranslate(0, cHeight * 0.5, 0);

	cHeight = percentage(pm, h);
	ofTranslate(0, cHeight * 0.5, 0);
	Style_Pixelate(w, cHeight, magentaRed); // Magenta

	ofTranslate(0, cHeight * 0.5, 0);

	cHeight = percentage(py, h);
	ofTranslate(0, cHeight * 0.5, 0);
	Style_Pixelate(w, cHeight, yellowGreen); // Yellow

	ofPopMatrix();

	cHeight = ofMap(cmyk[3], 0, 1, 0, h);
	Style_Pixelate(w, cHeight, black); // Black
}

void ofApp::Style_CMYK_Seperation_8(float w, float h, ofColor c) {
	float cHeight;
	ofVec4f cmyk = getCMYK(c);

	float total = cmyk[0] + cmyk[1] + cmyk[2];
	float pc = (100 * cmyk[0]) / total;
	float pm = (100 * cmyk[1]) / total;
	float py = (100 * cmyk[2]) / total;

	cHeight = ofMap(cmyk[3], 0, 1, 0, h);
	Style_Pixelate(w, cHeight, black); // Black

	cHeight = ofMap(cmyk[0], 0, 1, 0, h/3);
	ofPushMatrix();
	ofTranslate(0, (-h * 0.5) + (cHeight * 0.5), 0);
	Style_Pixelate(w, cHeight, cyanBlue); // Cyan

	ofTranslate(0, cHeight * 0.5, 0);

	cHeight = ofMap(cmyk[1], 0, 1, 0, h/3);
	ofTranslate(0, cHeight * 0.5, 0);
	Style_Pixelate(w, cHeight, magentaRed); // Magenta

	ofTranslate(0, cHeight * 0.5, 0);

	cHeight = ofMap(cmyk[2], 0, 1, 0, h/3);
	ofTranslate(0, cHeight * 0.5, 0);
	Style_Pixelate(w, cHeight, yellowGreen); // Yellow

	ofPopMatrix();

}

void ofApp::Style_CMYK_Seperation_9(float w, float h, ofColor c) {
	// Devide height in 3 and add colour at percentage.

	ofVec4f cmyk = getCMYK(c);

	Style_Pixelate(w, h, c);

	
	float cHeight = h / 3;
	
	ofPushMatrix();
	
	int brightnessThreshhold = 20;

	ofTranslate(0, (-h * 0.5) + (cHeight * 0.5), 0);
	float brightness = ofMap(cmyk[0], 0, 1, 0, 255);
	if (brightness > brightnessThreshhold) {
		ofColor cc = cyanBlue;
		Style_Pixelate(w, cHeight, ofColor(cc.r, cc.g, cc.b, brightness)); // Cyan 
	}
	
	ofTranslate(0, cHeight, 0);
	brightness = ofMap(cmyk[1], 0, 1, 0, 255);
	if (brightness > brightnessThreshhold) {
		ofColor cc = magentaRed;
		Style_Pixelate(w, cHeight, ofColor(cc.r, cc.g, cc.b, brightness)); // Magenta
	}
	
	ofTranslate(0, cHeight, 0);
	brightness = ofMap(cmyk[2], 0, 1, 0, 255);
	if (brightness > brightnessThreshhold) {
		ofColor cc = yellowGreen;
		Style_Pixelate(w, cHeight, ofColor(cc.r, cc.g, cc.b, brightness)); // Yellow
	}

	ofPopMatrix();

	cHeight = ofMap(cmyk[3], 0, 1, 0, h);
	Style_Pixelate(w, cHeight, black); // Black

}


void ofApp::Style_CMYK_Seperation_10(float w, float h, ofColor c, ofVec2f loc) {
	// Devide height in 3 and add colour at percentage.
	float rot = ofMap(loc[0], 0, img.getWidth(), -360, 360);

	ofVec4f cmyk = getCMYK(c);

	ofPushMatrix();
	//rotation = rotation + 0.25;
	//if (rotation > 360) rotation = rotation - 360;

	ofRotateZDeg(rot);
	
	Style_Pixelate(w, h, c);

	float cHeight = h / 3;

	ofPushMatrix();

	int brightnessThreshhold = 20;

	ofTranslate(0, (-h * 0.5) + (cHeight * 0.5), 0);
	float brightness = ofMap(cmyk[0], 0, 1, 0, 255);
	if (brightness > brightnessThreshhold) {
		ofColor cc = cyanBlue;
		Style_Pixelate(w, cHeight, ofColor(cc.r, cc.g, cc.b, brightness)); // Cyan
	}


	ofTranslate(0, cHeight, 0);
	brightness = ofMap(cmyk[1], 0, 1, 0, 255);
	if (brightness > brightnessThreshhold) {
		ofColor cc = magentaRed;
		Style_Pixelate(w, cHeight, ofColor(cc.r, cc.g, cc.b, brightness)); // Magenta
	}

	ofTranslate(0, cHeight, 0);
	brightness = ofMap(cmyk[2], 0, 1, 0, 255);
	if (brightness > brightnessThreshhold) {
		ofColor cc = yellowGreen;
		Style_Pixelate(w, cHeight, ofColor(cc.r, cc.g, cc.b, brightness)); // Yellow
	}

	ofPopMatrix();

	cHeight = ofMap(cmyk[3], 0, 1, 0, h);
	Style_Pixelate(w, cHeight, black); // Black

	ofPopMatrix();

}

void ofApp::Style_CMYK_Seperation_11(float w, float h, ofColor c, ofDefaultVec2 xycount) {

	//Style_Pixelate(w, h, c);
	
	int xcount = xycount[0];
	int ycount = xycount[1];
	int rc = 1 + rand() % 8;
	if (xcount % 2 == 0 && ycount %rc == 0) {
		c.setHueAngle(c.getHueAngle() + (ofRandomf() * 40));
		c.normalize();
		c.setBrightness(c.getBrightness() + ofRandom(-10,20));
	}
	Style_Pixelate(w, h, c);
	ofVec4f cmyk = getCMYK(c);

	float cHeight = h / 3;

	ofPushMatrix();

	int brightnessThreshhold = 20;

	ofTranslate(0, (-h * 0.5) + (cHeight * 0.5), 0);
	float brightness = ofMap(cmyk[0], 0, 1, 0, 255);
	if (brightness > brightnessThreshhold) {
		ofColor cc = cyanBlue;
		Style_Pixelate(w, cHeight, ofColor(cc.r, cc.g, cc.b, brightness)); // Cyan 
	}

	ofTranslate(0, cHeight, 0);
	brightness = ofMap(cmyk[1], 0, 1, 0, 255);
	if (brightness > brightnessThreshhold) {
		ofColor cc = magentaRed;
		Style_Pixelate(w, cHeight, ofColor(cc.r, cc.g, cc.b, brightness)); // Magenta
	}

	ofTranslate(0, cHeight, 0);
	brightness = ofMap(cmyk[2], 0, 1, 0, 255);
	if (brightness > brightnessThreshhold) {
		ofColor cc = yellowGreen;
		Style_Pixelate(w, cHeight, ofColor(cc.r, cc.g, cc.b, brightness)); // Yellow
	}

	ofPopMatrix();

	cHeight = ofMap(cmyk[3], 0, 1, 0, h);
	Style_Pixelate(w, cHeight, black); // Black

}

void ofApp::loadImage(string& filepath) {
	float ratio = 1;
	original.load(filepath);
	img.load(filepath);

	//(img.getWidth() >= img.getHeight()) ? isLandscape = true : isLandscape = false;

	// Resize to always fit screen
	ratio = img.getHeight() / img.getWidth();
	img.resize(ofGetWidth(), ofGetWidth() * ratio);	
	if (img.getHeight() > ofGetHeight()) {
		ratio = img.getWidth() / img.getHeight();
		img.resize(ofGetHeight() * ratio, ofGetHeight());
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
void ofApp::gui_setRGB_pressed() {
	magentaRed = ofColor(255, 0, 0);
	cyanBlue = ofColor(0, 0, 255);
	yellowGreen = ofColor(0,255,0);
	black = ofColor(0,0,0);
}

//--------------------------------------------------------------
void ofApp::gui_setCMYK_pressed() {
	magentaRed = ofColor(236, 0, 140);
	cyanBlue = ofColor(0, 174, 239);
	yellowGreen = ofColor(255, 242, 0);
	black = ofColor(0, 0, 0);
}

//--------------------------------------------------------------
void ofApp::gui_setPosterize_pressed() {
	swatches = ofxPosterize::getClusterColors(img, 4);
	if (swatches.size() > 3) {
		magentaRed = swatches[0];
		cyanBlue = swatches[1];
		yellowGreen = swatches[2];
		black = swatches[3];
	}
}

//--------------------------------------------------------------
void ofApp::gui_exportSVG_pressed() {
	saveSVG = true;
}

//-------------------------------------------------------------
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
