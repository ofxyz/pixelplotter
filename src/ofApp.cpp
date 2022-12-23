#include "ofApp.h"

/*
   
   Export image size dimensions not screen dimensions.
   Reload FBO on every image change

   Make some dots
   
   Posterise to a pallette. Set high mid low to start?
   Or dumb down the colours then ability to remap?

   // Add Polkadots

   // RGB needs K
   // Finish zoom function - Draw bigger ... 

   // GUI update on every change.
   // Add save load presets
   
   // Save multiple Settings

*/

//--------------------------------------------------------------
void ofApp::setup() {
	ofLogToConsole();
	ofSetLogLevel(OF_LOG_VERBOSE);
	//ofSetBackgroundAuto(false);
	ofSetCircleResolution(100);
	ofSetWindowTitle("Pixel Plotter");
	ofBackground(c_paper);
	//ofLog() << ofFbo::checkGLSupport();
	zoomFbo.allocate(zoomWindowW, zoomWindowH, GL_RGBA, 8);

	gui.setup();
	c_background = ofColor(114, 144, 154, 255);
	c_paper = ofColor(255, 255, 255, 255);
	c_magentaRed = ofColor(236, 0, 140);
	c_cyanBlue = ofColor(0, 174, 239);
	c_yellowGreen = ofColor(255, 242, 0);
	c_black = ofColor(0, 0, 0);

	currentPlotStyleIndex = ofRandom(0, v_PlotStyles.size() - 1);
	ofDirectory imgDirectory(ofToDataPath("src_img", true));
	imgFiles = imgDirectory.getFiles();
	for (int i = 0; i < imgFiles.size(); i++)
	{
		imgFileNames.push_back(imgFiles[i].getFileName());
	}
	currentImgFileIndex = ofRandom(0, imgFileNames.size()-1);
	loadImage(imgFiles[currentImgFileIndex].getAbsolutePath());
	
}

//--------------------------------------------------------------
void ofApp::exit() {
	//clean
}

//--------------------------------------------------------------
void ofApp::update() {
	//if (ofGetFrameNum() % 500 == 0) updateFbo();
	if (!pauseRender) {
		updateFbo();
	}
	
	if (showZoom) {
		zoomFbo.begin();
		float fX = max((float)0, min((mouseX * zoomMultiplier) - halfZoomWindowW, fbo.getWidth() - zoomWindowW));
		float fY = max((float)0, min((mouseY * zoomMultiplier) - halfZoomWindowH, fbo.getHeight() - zoomWindowH));
		fbo.getTexture().drawSubsection(0, 0, zoomWindowW, zoomWindowH, fX, fY);
		zoomFbo.end();
	}
}

void ofApp::updateFbo() {
	fbo.begin();
	ofBackground(c_paper);

	if (saveVector) {
		ofBeginSaveScreenAsPDF(img_name + "_" + ofGetTimestampString() + ".pdf", false);
	}

	setBlendmode();

	int imgW = img.getWidth();
	int imgH = img.getHeight();
	float tileW = (float)imgW / (float)tilesX;
	float tileH = (float)imgH / (float)tilesY;
	float halfTileW = tileW / 2.0;
	float halfTileH = tileH / 2.0;

	int ycount = 0;
	int xcount = 0;
	for (float y = 0; y < imgH - halfTileH; y += tileH) {
		for (float x = 0; x < imgW - halfTileW; x += tileW) {
			float fx = x + halfTileW;
			float fy = y + halfTileH;
			int cx = floor(fx);
			int cy = floor(fy);
			ofColor c = img.getPixels().getColor(cx, cy);

			if (normalise) {
				c.normalize();
			}

			ofPushMatrix();
			ofTranslate(fx * zoomMultiplier, fy * zoomMultiplier, 0);
			callStyle(v_PlotStyles[currentPlotStyleIndex], ofVec2f((tileW + addonx) * zoomMultiplier, (tileH + addony) * zoomMultiplier), ofVec2f(fx * zoomMultiplier, fy * zoomMultiplier), ofVec2f(xcount, ycount), c);
			ofPopMatrix();
			xcount++;
		}
		ycount++;
	}

	ofDisableBlendMode();

	if (saveVector) {
		ofEndSaveScreenAsPDF();
		saveVector = false;
	}

	fbo.end();
}

//--------------------------------------------------------------
void ofApp::draw(){

	ofSetBackgroundColor(c_background);

	fbo.draw(glm::vec2(0, 0), img.getWidth(), img.getHeight());

	if (showZoom) {
		float zX = max((float)0, min(mouseX - halfZoomWindowW, img.getWidth() - zoomWindowW));
		float zY = max((float)0, min(mouseY - halfZoomWindowH, img.getHeight() - zoomWindowH));
		zoomFbo.draw(glm::vec2(zX, zY), zoomWindowW, zoomWindowH);
	}

	if (showImage) {
		img.draw(0, 0);
	}

	gui.begin();
	{
		if (show_main_window)
		{
			ImGui::SetNextWindowSize(ofVec2f(300, 600));
			ImGui::SetNextWindowPos(ofVec2f(0,0));
			ImGui::Begin("Pixel Plotter", &show_main_window, ImGuiWindowFlags_NoDecoration);

			if (ImGui::CollapsingHeader("Source"))
			{
				if (!imgFileNames.empty())
				{
					if (ofxImGui::VectorCombo("##Source Image", &currentImgFileIndex, imgFileNames))
					{
						loadImage(imgFiles[currentImgFileIndex].getAbsolutePath());
					}
				}

				ImGui::SameLine();
				if (showImage) {
					if (ImGui::Button("Hide Source"))
					{
						showImage = false;
					}
				}
				else {
					if (ImGui::Button("Show Source"))
					{
						showImage = true;
					}
				}
			}

			if (ImGui::CollapsingHeader("Style"))
			{
				if (ofxImGui::VectorCombo("Plot Style", &currentPlotStyleIndex, v_PlotStyles))
				{
					loadImage(imgFiles[currentImgFileIndex].getAbsolutePath());
				}

				ImGui::AlignTextToFramePadding();
				ImGui::PushItemWidth(100);

				ImGui::Text("Tiles"); ImGui::SameLine();
				ImGui::DragInt("X ##Tiles", &tilesX, 1, 1, 1200);
				ImGui::SameLine();
				ImGui::DragInt("Y ##Tiles", &tilesY, 1, 1, 1200);

				ImGui::Text("Addon"); ImGui::SameLine();
				ImGui::SliderFloat("X ##Addon", &addonx, -100.0f, 100.0f, "%.3f");
				ImGui::SameLine();
				ImGui::SliderFloat("Y ##Addon", &addony, -100.0f, 100.0f, "%.3f");

				ImGui::SliderFloat("Random Offset", &randomOffset, -100.0f, 100.0f, "%.3f");

				ImGui::Text("Noise"); ImGui::SameLine();
				ImGui::SliderFloat("X ##Noise", &noisepercentX, 0.0f, 100.0f, "%.2f%%");
				ImGui::SameLine();
				ImGui::SliderFloat("Y ##Noise", &noisepercentY, 0.0f, 100.0f, "%.2f%%");
				
				ImGui::Text("Every N"); ImGui::SameLine();
				ImGui::DragInt("X ##Every N", &everynx, 1, 1, 128);
				ImGui::SameLine();
				ImGui::DragInt("Y ##Every N", &everyny, 1, 1, 128);

				ImGui::PopItemWidth();

				// Pixel Type: line, square, rect, round, oval
				// Use Radio or dropdown
				if (roundPixels) {
					if (ImGui::Button("Square Pixels"))
					{
						roundPixels = false;
					}
				}
				else {
					if (ImGui::Button("Round Pixels"))
					{
						roundPixels = true;
					}
				}
			}

			if (ImGui::CollapsingHeader("Colours"))
			{
				ImGui::ColorEdit4("Magenta / Red", (float*)&c_magentaRed, ImGuiColorEditFlags_NoInputs);
				ImGui::ColorEdit4("Cyan / Blue", (float*)&c_cyanBlue, ImGuiColorEditFlags_NoInputs);
				ImGui::ColorEdit4("Yellow / Green", (float*)&c_yellowGreen, ImGuiColorEditFlags_NoInputs);
				ImGui::ColorEdit4("Black", (float*)&c_black, ImGuiColorEditFlags_NoInputs);
				ImGui::ColorEdit4("Paper / White", (float*)&c_paper, ImGuiColorEditFlags_NoInputs);
				ImGui::ColorEdit4("Background", (float*)&c_background, ImGuiColorEditFlags_NoInputs);

				if (ImGui::Button("Set RGB"))
				{
					ofApp::gui_setRGB_pressed();
				}
				ImGui::SameLine();
				if (ImGui::Button("Set CMYK"))
				{
					ofApp::gui_setCMYK_pressed();
				}
				ImGui::SameLine();
				if (ImGui::Button("Set Avarage"))
				{
					ofApp::gui_setAvarage_pressed();
				}

				ImGui::Checkbox("Normalise Colours", &normalise);

				if (ofxImGui::VectorCombo("Blend Mode", &currentBlendModeIndex, v_BlendModes))
				{
					currentBlendmode = v_BlendModes[currentBlendModeIndex];
				}

			} // End Colours

			if (ImGui::Button("Export Vector")) { saveVector = true; }
			ImGui::SameLine();

			if (pauseRender) {
				if (ImGui::Button("Continue"))
				{
					pauseRender = false;
				}
			}
			else {
				if (ImGui::Button("Pause"))
				{
					pauseRender = true;
				}
			}

			ImGui::SameLine();

			if (showZoom) {
				if (ImGui::Button("Hide Zoom"))
				{
					showZoom = false;
				}
			}
			else {
				if (ImGui::Button("Show Zoom"))
				{
					showZoom = true;
				}
			}

			if (pauseRender) {
				ImGui::Text("Paused at %.1f FPS", ImGui::GetIO().Framerate);
			}
			else {
				ImGui::Text("Rendering at %.1f FPS", ImGui::GetIO().Framerate);
			}
			
			ImGui::End();
		}
	}
	gui.end();


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
	else if (stylename == "CMYK Seperation 12") {
		Style_CMYK_Seperation_12(w, h, c, xycount);
	}
}

void ofApp::setBlendmode() {
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
	//w = abs(w);
	//h = abs(h);

	if ( (w > -0.25 && w < 0.25) || (h > -0.25 && h < 0.25)) {
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
		ofDrawRectangle(-(w * 0.5)+ offsetX, -(h * 0.5)+ offsetY, w, h);
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
	Style_Pixelate(w, cHeight, c_magentaRed);

	cHeight = ofMap(c.g, 0, 255, 0, h);
	Style_Pixelate(w, cHeight, c_yellowGreen);

	cHeight = ofMap(c.b, 0, 255, 0, h);
	Style_Pixelate(w, cHeight, c_cyanBlue);

}

void ofApp::Style_RGB_Seperation_2(float w, float h, ofColor c) {

	float cWidth, cHeight;

	cWidth  = ofMap(c.r, 0, 255, 0, w);
	cHeight = ofMap(c.r, 0, 255, 0, h);
	Style_Pixelate(cWidth, cHeight, c_magentaRed);

	cWidth  = ofMap(c.g, 0, 255, 0, w);
	cHeight = ofMap(c.g, 0, 255, 0, h);
	Style_Pixelate(cWidth, cHeight, c_yellowGreen);

	cWidth  = ofMap(c.b, 0, 255, 0, w);
	cHeight = ofMap(c.b, 0, 255, 0, h);
	Style_Pixelate(cWidth, cHeight, c_cyanBlue);
}

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
	ofTranslate(-(w*0.5)+(cWidth*0.5), 0, 0);
	Style_Pixelate(cWidth, h, c_magentaRed);

	ofTranslate(cWidth*0.5, 0, 0);
	cWidth = ofMap(c.g + addon, 0, maxC, 0, w);
	ofTranslate(cWidth * 0.5, 0, 0);
	Style_Pixelate(cWidth, h, c_yellowGreen);

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
void ofApp::Style_CMYK_Seperation_1(float w, float h, ofColor c) {
	float cWidth, cHeight;
	ofVec4f cmyk = getCMYK(c);

	cWidth = ofMap(cmyk[2], 0, 1, 0, w);
	cHeight = ofMap(cmyk[2], 0, 1, 0, h);

	Style_Pixelate(cWidth, cHeight, c_yellowGreen); // Yellow

	cWidth  = ofMap(cmyk[0], 0, 1, 0, w);
	cHeight = ofMap(cmyk[0], 0, 1, 0, h);
	Style_Pixelate(cWidth, cHeight,c_cyanBlue); // Cyan

	cWidth  = ofMap(cmyk[1], 0, 1, 0, w);
	cHeight = ofMap(cmyk[1], 0, 1, 0, h);
	Style_Pixelate(cWidth, cHeight, c_magentaRed); // Magenta

	cWidth  = ofMap(cmyk[3], 0, 1, 0, w);
	cHeight = ofMap(cmyk[3], 0, 1, 0, h);
	Style_Pixelate(cWidth, cHeight, c_black); // Black

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

	cWidth = ofMap(cmyk[3], 0, 1, 0, w/2);

	if (cWidth > w * 0.1) {
		Style_Pixelate(cWidth, h, c_black); // Black
	}
	

	ofPushMatrix(); // offset

	cWidth = ofMap(cmyk[0], 0, 1, 0, w/2);
	ofTranslate(-(w * 0.5) + (cWidth * 0.5), 0, 0);
	Style_Pixelate(cWidth, h, c_cyanBlue); // Cyan
	
	ofTranslate(cWidth * 0.5, 0, 0);

	cWidth = ofMap(cmyk[1], 0, 1, 0, w/2);
	ofTranslate(cWidth * 0.5, 0, 0);
	Style_Pixelate(cWidth, h, c_magentaRed); // Magenta
	
	ofTranslate(cWidth * 0.5, 0, 0);

	cWidth = ofMap(cmyk[2], 0, 1, 0, w/2);
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
	ofTranslate(0, (h * 0.5)-(cHeight*0.5), 0);
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

	cHeight = ofMap(cmyk[3], 0, 1, 2, h-2);
	if (cHeight > h * 0.5) {
		Style_Pixelate(w, cHeight, c_black); // Black
	}

	cHeight = ofMap(cmyk[0], 0, 1, 2, h-2);
	ofPushMatrix();
	ofTranslate(0, ((-h - 2) * 0.5) - (cHeight * 0.5), 0);
	ofColor cc = c_cyanBlue;
	cc.setBrightness(130);
	Style_Pixelate(w, cHeight, cc); // Cyan
	ofPopMatrix();

	cHeight = ofMap(cmyk[1], 0, 1, 2, h-2);
	ofPushMatrix();
	ofTranslate(0, ((-h-2) * 0.5) + (cHeight * 0.5), 0);
	cc = c_magentaRed;
	cc.setBrightness(130);
	Style_Pixelate(w, cHeight, cc); // Magenta
	ofPopMatrix();

	cHeight = ofMap(cmyk[2], 0, 1, 2, h / 2);
	cc = c_yellowGreen;
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

void ofApp::Style_CMYK_Seperation_8(float w, float h, ofColor c) {
	float cHeight;
	ofVec4f cmyk = getCMYK(c);

	float total = cmyk[0] + cmyk[1] + cmyk[2];
	float pc = (100 * cmyk[0]) / total;
	float pm = (100 * cmyk[1]) / total;
	float py = (100 * cmyk[2]) / total;

	cHeight = ofMap(cmyk[3], 0, 1, 0, h);
	Style_Pixelate(w, cHeight, c_black); // c_black

	cHeight = ofMap(cmyk[0], 0, 1, 0, h/3);
	ofPushMatrix();
	ofTranslate(0, (-h * 0.5) + (cHeight * 0.5), 0);
	Style_Pixelate(w, cHeight, c_cyanBlue); // Cyan

	ofTranslate(0, cHeight * 0.5, 0);

	cHeight = ofMap(cmyk[1], 0, 1, 0, h/3);
	ofTranslate(0, cHeight * 0.5, 0);
	Style_Pixelate(w, cHeight, c_magentaRed); // Magenta

	ofTranslate(0, cHeight * 0.5, 0);

	cHeight = ofMap(cmyk[2], 0, 1, 0, h/3);
	ofTranslate(0, cHeight * 0.5, 0);
	Style_Pixelate(w, cHeight, c_yellowGreen); // Yellow

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

void ofApp::Style_CMYK_Seperation_11(float w, float h, ofColor c, ofDefaultVec2 xycount) {

	//Style_Pixelate(w, h, c);
	
	int xcount = xycount[0];
	int ycount = xycount[1];
	int rc = 1 + rand() % 8;
	if (xcount % 4 == 0 && ofRandom(0, 100) > 30) {
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

void ofApp::loadImage(string& filepath) {
	
	original.load(filepath);
	img.load(filepath);

	std::string base_filename = filepath.substr(filepath.find_last_of("/\\") + 1);
	std::string::size_type const p(base_filename.find_last_of('.'));
	std::string file_without_extension = base_filename.substr(0, p);
	img_name = file_without_extension;

	// Resize image fit screen
	(img.getWidth() > img.getHeight()) ? isLandscape = true : isLandscape = false;
	if (isLandscape) {
		ratio = img.getHeight() / img.getWidth();
		img.resize(ofGetWidth(), ofGetWidth() * ratio);	
	}
	else {
		ratio = img.getWidth() / img.getHeight();
		img.resize(ofGetHeight() * ratio, ofGetHeight());
	}

	fbo.allocate(img.getWidth()*zoomMultiplier, img.getHeight() * zoomMultiplier, GL_RGBA, 8);
}

void ofApp::onImageChange(string& filepath) {
	loadImage(filepath);
}

void ofApp::onPresetChange(string& filepath) {
	// loadFromFile(filepath);
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
	c_magentaRed = ofColor(255, 0, 0);
	c_cyanBlue = ofColor(0, 0, 255);
	c_yellowGreen = ofColor(0,255,0);
	c_black = ofColor(0,0,0);
}

//--------------------------------------------------------------
void ofApp::gui_setCMYK_pressed() {
	c_magentaRed = ofColor(236, 0, 140);
	c_cyanBlue = ofColor(0, 174, 239);
	c_yellowGreen = ofColor(255, 242, 0);
	c_black = ofColor(0, 0, 0);
}

//--------------------------------------------------------------
void ofApp::gui_setAvarage_pressed() {
	swatches = ofxPosterize::getClusterColors(img, 4);
	if (swatches.size() > 3) {
		c_magentaRed = swatches[0];
		c_cyanBlue = swatches[1];
		c_yellowGreen = swatches[2];
		c_black = swatches[3];
	} else if(swatches.size() > 2) {
		c_magentaRed = swatches[0];
		c_cyanBlue = swatches[1];
		c_yellowGreen = swatches[2];
	} else if (swatches.size() > 1) {
		c_magentaRed = swatches[0];
		c_cyanBlue = swatches[1];
	} else if (swatches.size() > 0) {
		c_magentaRed = swatches[0];
	}
}

//-------------------------------------------------------------
void ofApp::keyPressed(int key){
	updateFbo();
	if (key == 'p') {
		saveVector = true;
	}
	if (key == 'x') {
		pauseRender = !pauseRender;
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
