#include "ofApp.h"

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
	else if (stylename == "CMYK Seperation 12") {
		Style_CMYK_Seperation_12(w, h, c, xycount);
	}
}

//--------------------------------------------------------------
void ofApp::Style_Pixelate(float w, float h, ofColor c) {
	//w = abs(w);
	//h = abs(h);

	if ((w > -0.25 && w < 0.25) || (h > -0.25 && h < 0.25)) {
		return;
	}

	float offsetX = ofRandom(-randomOffset, randomOffset);
	float offsetY = ofRandom(-randomOffset, randomOffset);

	ofPushStyle();
	ofFill();
	ofSetColor(c);
	if (roundPixels) {
		ofDrawEllipse(offsetX, offsetY, w, h);
	}
	else {
		ofDrawRectangle(-(w * 0.5) + offsetX, -(h * 0.5) + offsetY, w, h);
	}
	ofPopStyle();
}

//--------------------------------------------------------------
void ofApp::Style_Pixelate_Brightness_Width(float w, float h, ofColor c) {
	float cWidth = ofMap(c.getBrightness(), 0, 255, 0, w);
	Style_Pixelate(cWidth, h, c);
}

//--------------------------------------------------------------
void ofApp::Style_Lightness_Rotation(float w, float h, ofColor c) {
	ofPushMatrix();
	ofRotateZDeg(ofMap(c.getLightness(), 0, 255, 0, 360));
	Style_Pixelate(w, h, c);
	ofPopMatrix();
}

//--------------------------------------------------------------
void ofApp::Style_RGB_Seperation_1(float w, float h, ofColor c) {

	float cHeight;

	cHeight = ofMap(c.r, 0, 255, 0, h);
	Style_Pixelate(w, cHeight, c_magentaRed);

	cHeight = ofMap(c.g, 0, 255, 0, h);
	Style_Pixelate(w, cHeight, c_yellowGreen);

	cHeight = ofMap(c.b, 0, 255, 0, h);
	Style_Pixelate(w, cHeight, c_cyanBlue);

}

//--------------------------------------------------------------
void ofApp::Style_RGB_Seperation_2(float w, float h, ofColor c) {

	float cWidth, cHeight;

	cWidth = ofMap(c.r, 0, 255, 0, w);
	cHeight = ofMap(c.r, 0, 255, 0, h);
	Style_Pixelate(cWidth, cHeight, c_magentaRed);

	cWidth = ofMap(c.g, 0, 255, 0, w);
	cHeight = ofMap(c.g, 0, 255, 0, h);
	Style_Pixelate(cWidth, cHeight, c_yellowGreen);

	cWidth = ofMap(c.b, 0, 255, 0, w);
	cHeight = ofMap(c.b, 0, 255, 0, h);
	Style_Pixelate(cWidth, cHeight, c_cyanBlue);
}

//--------------------------------------------------------------
void ofApp::Style_RGB_Seperation_3(float w, float h, ofColor c) {

	float maxWidth = w / 3;
	float cWidth;

	cWidth = ofMap(c.r, 0, 255, 0, maxWidth);
	Style_Pixelate(cWidth, h, c_magentaRed); // Red

	ofPushMatrix(); // offset

	ofTranslate(maxWidth, 0, 0);
	cWidth = ofMap(c.g, 0, 255, 0, maxWidth);
	Style_Pixelate(cWidth, h, c_yellowGreen); // Green

	ofTranslate(maxWidth, 0, 0);
	cWidth = ofMap(c.b, 0, 255, 0, maxWidth);
	Style_Pixelate(cWidth, h, c_cyanBlue); // Blue

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
	ofTranslate(-(w * 0.5) + (cWidth * 0.5), 0, 0);
	Style_Pixelate(cWidth, h, c_magentaRed);

	ofTranslate(cWidth * 0.5, 0, 0);
	cWidth = ofMap(c.g + addon, 0, maxC, 0, w);
	ofTranslate(cWidth * 0.5, 0, 0);
	Style_Pixelate(cWidth, h, c_yellowGreen);

	ofTranslate(cWidth * 0.5, 0, 0);
	cWidth = ofMap(c.b + addon, 0, maxC, 0, w);
	ofTranslate(cWidth * 0.5, 0, 0);
	Style_Pixelate(cWidth, h, ofColor(0, 0, 255));

	ofPopMatrix();

}

//--------------------------------------------------------------
void ofApp::Style_RGB_Seperation_5(float w, float h, ofColor c) {
	float cHeight;
	ofVec4f cmyk = getCMYK(c);
	Style_Pixelate(w, h, c);

	cHeight = ofMap(cmyk[3], 0, 1, 0, h);
	Style_Pixelate(w, cHeight, c_black); // Black

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

//--------------------------------------------------------------
void ofApp::Style_CMYK_Seperation_1(float w, float h, ofColor c) {
	float cWidth, cHeight;
	ofVec4f cmyk = getCMYK(c);

	cWidth = ofMap(cmyk[2], 0, 1, 0, w);
	cHeight = ofMap(cmyk[2], 0, 1, 0, h);

	Style_Pixelate(cWidth, cHeight, c_yellowGreen); // Yellow

	cWidth = ofMap(cmyk[0], 0, 1, 0, w);
	cHeight = ofMap(cmyk[0], 0, 1, 0, h);
	Style_Pixelate(cWidth, cHeight, c_cyanBlue); // Cyan

	cWidth = ofMap(cmyk[1], 0, 1, 0, w);
	cHeight = ofMap(cmyk[1], 0, 1, 0, h);
	Style_Pixelate(cWidth, cHeight, c_magentaRed); // Magenta

	cWidth = ofMap(cmyk[3], 0, 1, 0, w);
	cHeight = ofMap(cmyk[3], 0, 1, 0, h);
	Style_Pixelate(cWidth, cHeight, c_black); // Black

}

//--------------------------------------------------------------
void ofApp::Style_CMYK_Seperation_2(float w, float h, ofColor c) {
	float cHeight;
	ofVec4f cmyk = getCMYK(c);

	float maxOffset = w * 0.5;
	float minOffset = -maxOffset;

	ofPushMatrix();
	ofTranslate(ofRandom(minOffset, maxOffset), ofRandom(minOffset, maxOffset), 0);
	cHeight = ofMap(cmyk[0], 0, 1, 0, h);
	Style_Pixelate(cHeight, cHeight, c_cyanBlue); // Cyan
	ofPopMatrix();

	ofPushMatrix();
	ofTranslate(ofRandom(minOffset, maxOffset), ofRandom(minOffset, maxOffset), 0);
	cHeight = ofMap(cmyk[1], 0, 1, 0, h);
	Style_Pixelate(cHeight, cHeight, c_magentaRed); // Magenta
	ofPopMatrix();

	ofPushMatrix();
	ofTranslate(ofRandom(minOffset, maxOffset), ofRandom(minOffset, maxOffset), 0);
	cHeight = ofMap(cmyk[2], 0, 1, 0, h);
	Style_Pixelate(cHeight, cHeight, c_yellowGreen); // Yellow
	ofPopMatrix();

	ofPushMatrix();
	ofTranslate(ofRandom(minOffset, maxOffset), ofRandom(minOffset, maxOffset), 0);
	cHeight = ofMap(cmyk[3], 0, 1, 0, h);
	Style_Pixelate(cHeight, cHeight, c_black); // Black
	ofPopMatrix();
}

//--------------------------------------------------------------
void ofApp::Style_CMYK_Seperation_3(float w, float h, ofColor c) {
	float cWidth;
	ofVec4f cmyk = getCMYK(c);

	cWidth = ofMap(cmyk[0], 0, 1, 0, w);
	Style_Pixelate(cWidth, cWidth, c_cyanBlue); // Cyan

	cWidth = ofMap(cmyk[1], 0, 1, 0, w);
	Style_Pixelate(cWidth, cWidth, c_magentaRed); // Magenta

	cWidth = ofMap(cmyk[2], 0, 1, 0, w);
	Style_Pixelate(cWidth, cWidth, c_yellowGreen); // Yellow

	cWidth = ofMap(cmyk[3], 0, 1, 0, w);
	Style_Pixelate(cWidth, cWidth, c_black); // Black

}

//--------------------------------------------------------------
void ofApp::Style_CMYK_Seperation_4(float w, float h, ofColor c) {
	float cWidth;
	ofVec4f cmyk = getCMYK(c);

	cWidth = ofMap(cmyk[3], 0, 1, 0, w / 2);

	if (cWidth > w * 0.1) {
		Style_Pixelate(cWidth, h, c_black); // Black
	}


	ofPushMatrix(); // offset

	cWidth = ofMap(cmyk[0], 0, 1, 0, w / 2);
	ofTranslate(-(w * 0.5) + (cWidth * 0.5), 0, 0);
	Style_Pixelate(cWidth, h, c_cyanBlue); // Cyan

	ofTranslate(cWidth * 0.5, 0, 0);

	cWidth = ofMap(cmyk[1], 0, 1, 0, w / 2);
	ofTranslate(cWidth * 0.5, 0, 0);
	Style_Pixelate(cWidth, h, c_magentaRed); // Magenta

	ofTranslate(cWidth * 0.5, 0, 0);

	cWidth = ofMap(cmyk[2], 0, 1, 0, w / 2);
	ofTranslate(cWidth * 0.5, 0, 0);
	Style_Pixelate(cWidth, h, c_yellowGreen); // Yellow

	ofPopMatrix();
}

//--------------------------------------------------------------
void ofApp::Style_CMYK_Seperation_5(float w, float h, ofColor c) {
	float cHeight;
	ofVec4f cmyk = getCMYK(c);

	cHeight = ofMap(cmyk[3], 0, 1, 0, h);
	if (cHeight > h * 0.1) {
		Style_Pixelate(w, cHeight, c_black); // Black
	}

	cHeight = ofMap(cmyk[2], 0, 1, 0, h);
	Style_Pixelate(w, cHeight, c_yellowGreen); // Yellow

	cHeight = ofMap(cmyk[0], 0, 1, 0, h);
	ofPushMatrix();
	ofTranslate(0, (h * 0.5) - (cHeight * 0.5), 0);
	Style_Pixelate(w, cHeight, c_cyanBlue); // Cyan
	ofPopMatrix();

	cHeight = ofMap(cmyk[1], 0, 1, 0, h);
	ofPushMatrix();
	ofTranslate(0, (-h * 0.5) + (cHeight * 0.5), 0);
	Style_Pixelate(w, cHeight, c_magentaRed); // Magenta
	ofPopMatrix();

}

//--------------------------------------------------------------
void ofApp::Style_CMYK_Seperation_6(float w, float h, ofColor c) {
	float cHeight;
	ofVec4f cmyk = getCMYK(c);

	cHeight = ofMap(cmyk[3], 0, 1, 2, h - 2);
	if (cHeight > h * 0.5) {
		Style_Pixelate(w, cHeight, c_black); // Black
	}

	cHeight = ofMap(cmyk[0], 0, 1, 2, h - 2);
	ofPushMatrix();
	ofTranslate(0, ((-h - 2) * 0.5) - (cHeight * 0.5), 0);
	ofColor cc = c_cyanBlue;
	cc.setBrightness(130);
	Style_Pixelate(w, cHeight, cc); // Cyan
	ofPopMatrix();

	cHeight = ofMap(cmyk[1], 0, 1, 2, h - 2);
	ofPushMatrix();
	ofTranslate(0, ((-h - 2) * 0.5) + (cHeight * 0.5), 0);
	cc = c_magentaRed;
	cc.setBrightness(130);
	Style_Pixelate(w, cHeight, cc); // Magenta
	ofPopMatrix();

	cHeight = ofMap(cmyk[2], 0, 1, 2, h / 2);
	cc = c_yellowGreen;
	cc.setBrightness(130);
	Style_Pixelate(w, cHeight, cc);
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
	Style_Pixelate(w, cHeight, c_cyanBlue); // Cyan

	ofTranslate(0, cHeight * 0.5, 0);

	cHeight = percentage(pm, h);
	ofTranslate(0, cHeight * 0.5, 0);
	Style_Pixelate(w, cHeight, c_magentaRed); // Magenta

	ofTranslate(0, cHeight * 0.5, 0);

	cHeight = percentage(py, h);
	ofTranslate(0, cHeight * 0.5, 0);
	Style_Pixelate(w, cHeight, c_yellowGreen); // Yellow

	ofPopMatrix();

	cHeight = ofMap(cmyk[3], 0, 1, 0, h);
	Style_Pixelate(w, cHeight, c_black); // c_black
}

//--------------------------------------------------------------
void ofApp::Style_CMYK_Seperation_8(float w, float h, ofColor c) {
	float cHeight;
	ofVec4f cmyk = getCMYK(c);

	float total = cmyk[0] + cmyk[1] + cmyk[2];
	float pc = (100 * cmyk[0]) / total;
	float pm = (100 * cmyk[1]) / total;
	float py = (100 * cmyk[2]) / total;

	cHeight = ofMap(cmyk[3], 0, 1, 0, h);
	Style_Pixelate(w, cHeight, c_black); // c_black

	cHeight = ofMap(cmyk[0], 0, 1, 0, h / 3);
	ofPushMatrix();
	ofTranslate(0, (-h * 0.5) + (cHeight * 0.5), 0);
	Style_Pixelate(w, cHeight, c_cyanBlue); // Cyan

	ofTranslate(0, cHeight * 0.5, 0);

	cHeight = ofMap(cmyk[1], 0, 1, 0, h / 3);
	ofTranslate(0, cHeight * 0.5, 0);
	Style_Pixelate(w, cHeight, c_magentaRed); // Magenta

	ofTranslate(0, cHeight * 0.5, 0);

	cHeight = ofMap(cmyk[2], 0, 1, 0, h / 3);
	ofTranslate(0, cHeight * 0.5, 0);
	Style_Pixelate(w, cHeight, c_yellowGreen); // Yellow

	ofPopMatrix();

}

//--------------------------------------------------------------
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
		ofColor cc = c_cyanBlue;
		Style_Pixelate(w, cHeight, ofColor(cc.r, cc.g, cc.b, brightness)); // Cyan 
	}

	ofTranslate(0, cHeight, 0);
	brightness = ofMap(cmyk[1], 0, 1, 0, 255);
	if (brightness > brightnessThreshhold) {
		ofColor cc = c_magentaRed;
		Style_Pixelate(w, cHeight, ofColor(cc.r, cc.g, cc.b, brightness)); // Magenta
	}

	ofTranslate(0, cHeight, 0);
	brightness = ofMap(cmyk[2], 0, 1, 0, 255);
	if (brightness > brightnessThreshhold) {
		ofColor cc = c_yellowGreen;
		Style_Pixelate(w, cHeight, ofColor(cc.r, cc.g, cc.b, brightness)); // Yellow
	}

	ofPopMatrix();

	cHeight = ofMap(cmyk[3], 0, 1, 0, h);
	Style_Pixelate(w, cHeight, c_black); // c_black

}

//--------------------------------------------------------------
void ofApp::Style_CMYK_Seperation_10(float w, float h, ofColor c, ofVec2f loc) {
	// Devide height in 3 and add colour at percentage.
	float rot = ofMap(loc[0], 0, img.getWidth(), -360, 360);

	ofVec4f cmyk = getCMYK(c);

	ofPushMatrix();

	ofRotateZDeg(rot);

	Style_Pixelate(w, h, c);

	float cHeight = h / 3;

	ofPushMatrix();

	int brightnessThreshhold = 20;

	ofTranslate(0, (-h * 0.5) + (cHeight * 0.5), 0);
	float brightness = ofMap(cmyk[0], 0, 1, 0, 255);
	if (brightness > brightnessThreshhold) {
		ofColor cc = c_cyanBlue;
		Style_Pixelate(w, cHeight, ofColor(cc.r, cc.g, cc.b, brightness)); // Cyan
	}


	ofTranslate(0, cHeight, 0);
	brightness = ofMap(cmyk[1], 0, 1, 0, 255);
	if (brightness > brightnessThreshhold) {
		ofColor cc = c_magentaRed;
		Style_Pixelate(w, cHeight, ofColor(cc.r, cc.g, cc.b, brightness)); // Magenta
	}

	ofTranslate(0, cHeight, 0);
	brightness = ofMap(cmyk[2], 0, 1, 0, 255);
	if (brightness > brightnessThreshhold) {
		ofColor cc = c_yellowGreen;
		Style_Pixelate(w, cHeight, ofColor(cc.r, cc.g, cc.b, brightness)); // Yellow
	}

	ofPopMatrix();

	cHeight = ofMap(cmyk[3], 0, 1, 0, h);
	Style_Pixelate(w, cHeight, c_black); // c_black

	ofPopMatrix();
}

//--------------------------------------------------------------
void ofApp::Style_CMYK_Seperation_11(float w, float h, ofColor c, ofDefaultVec2 xycount) {

	//Style_Pixelate(w, h, c);

	int xcount = xycount[0];
	int ycount = xycount[1];
	int rc = 1 + rand() % 8;
	if (xcount % 4 == 0 && ofRandom(0, 100) > 30) {
		c.setHueAngle(c.getHueAngle() + (ofRandomf() * 40));
		c.normalize();
		c.setBrightness(c.getBrightness() + ofRandom(-10, 20));
	}
	Style_Pixelate(w, h, c);
	ofVec4f cmyk = getCMYK(c);

	float cHeight = h / 3;

	ofPushMatrix();

	int brightnessThreshhold = 20;

	ofTranslate(0, (-h * 0.5) + (cHeight * 0.5), 0);
	float brightness = ofMap(cmyk[0], 0, 1, 0, 255);
	if (brightness > brightnessThreshhold) {
		ofColor cc = c_cyanBlue;
		Style_Pixelate(w, cHeight, ofColor(cc.r, cc.g, cc.b, brightness)); // Cyan 
	}

	ofTranslate(0, cHeight, 0);
	brightness = ofMap(cmyk[1], 0, 1, 0, 255);
	if (brightness > brightnessThreshhold) {
		ofColor cc = c_magentaRed;
		Style_Pixelate(w, cHeight, ofColor(cc.r, cc.g, cc.b, brightness)); // Magenta
	}

	ofTranslate(0, cHeight, 0);
	brightness = ofMap(cmyk[2], 0, 1, 0, 255);
	if (brightness > brightnessThreshhold) {
		ofColor cc = c_yellowGreen;
		Style_Pixelate(w, cHeight, ofColor(cc.r, cc.g, cc.b, brightness)); // Yellow
	}

	ofPopMatrix();

	cHeight = ofMap(cmyk[3], 0, 1, 0, h);
	Style_Pixelate(w, cHeight, c_black); // c_black
}

//--------------------------------------------------------------
void ofApp::Style_CMYK_Seperation_12(float w, float h, ofColor c, ofDefaultVec2 xycount) {

	vector<ofColor> cCMY;
	cCMY.push_back(c_cyanBlue);
	cCMY.push_back(c_magentaRed);
	cCMY.push_back(c_yellowGreen);

	vector<int> cIndex;
	int xcount = xycount[0];
	int ycount = xycount[1];

	if (ofRandom(0, 100) < noisepercentY) {
		//if (ycount % everyny == 0) {
		c.setHueAngle(c.getHueAngle() + (ofRandomf() * 35));
		if (ofRandom(0, 100) > 50) {
			c.normalize();
		}
		c.setBrightness(c.getBrightness() + ofRandom(-10, 20));
	}

	//if (ofRandom(0, 100) < noisepercentX) {
	if (xcount % everynx == 0) {
		//c.normalize();
		cIndex.push_back(1);
		cIndex.push_back(2);
		cIndex.push_back(0);

	}
	else {
		cIndex.push_back(0);
		cIndex.push_back(1);
		cIndex.push_back(2);
	}

	Style_Pixelate(w, h, c);
	ofVec4f cmyk = getCMYK(c);

	float cHeight = h / 3;

	ofPushMatrix();

	int brightnessThreshhold = 20;

	ofTranslate(0, (-h * 0.5) + (cHeight * 0.5), 0);
	float brightness = ofMap(cmyk[cIndex[0]], 0, 1, 0, 255);
	if (brightness > brightnessThreshhold) {
		ofColor cc = cCMY[cIndex[0]];
		Style_Pixelate(w, cHeight, ofColor(cc.r, cc.g, cc.b, brightness)); // Cyan 
	}

	ofTranslate(0, cHeight, 0);
	brightness = ofMap(cmyk[cIndex[1]], 0, 1, 0, 255);
	if (brightness > brightnessThreshhold) {
		ofColor cc = cCMY[cIndex[1]];
		Style_Pixelate(w, cHeight, ofColor(cc.r, cc.g, cc.b, brightness)); // Magenta
	}

	ofTranslate(0, cHeight, 0);
	brightness = ofMap(cmyk[cIndex[2]], 0, 1, 0, 255);
	if (brightness > brightnessThreshhold) {
		ofColor cc = cCMY[cIndex[2]];
		Style_Pixelate(w, cHeight, ofColor(cc.r, cc.g, cc.b, brightness)); // Yellow
	}

	ofPopMatrix();

	cHeight = ofMap(cmyk[3], 0, 1, 0, h);
	Style_Pixelate(w, cHeight, c_black); // Black
}

//--------------------------------------------------------------
// EOF
