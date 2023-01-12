#include "ofApp.h"

/*
   - Add clear screen button or tickbox
   - Make some polka dots
   - Posterise Source
   - RGB needs K check and clean options
   - CMYK needs white control option
   - Add modulators (EG X Y tiles between min-max, time)
   - I want to set a value to the colour pallette of four colours
     To control how much each colour is used. Main Mid Accent? Weight.

   - Nim: Black and white. CMYK RGB 
*/

//--------------------------------------------------------------

void ofApp::setup() {
	ofLogToConsole();
	ofSetLogLevel(OF_LOG_WARNING);
	//ofSetBackgroundAuto(false);
	ofSetCircleResolution(100);
	ofSetWindowTitle("Pixel Plotter");
	ofBackground(c_paper);

	videoDevices = videoGrabber.listDevices();
	for (vector<ofVideoDevice>::iterator it = videoDevices.begin(); it != videoDevices.end(); ++it) {
		videoDeviceNames.push_back(it->deviceName);
	}

	videoGrabber.initGrabber(camWidth, camHeight);

	//ofLog() << ofFbo::checkGLSupport();
	zoomFbo.allocate(zoomWindowW, zoomWindowH, GL_RGBA, 8);

	gui.setup();
	ImGuiStyle* style = &ImGui::GetStyle();
	style->ItemSpacing = ImVec2(5, 5);

	ImGui::StyleColorsClassic();

	c_background = ofColor(50, 50, 50, 255);
	c_paper = ofColor(255, 255, 255, 255);
	c_magentaRed = ofColor(236, 0, 140);
	c_cyanBlue = ofColor(0, 174, 239);
	c_yellowGreen = ofColor(255, 242, 0);
	c_black = ofColor(0, 0, 0);

	ofDirectory vidDirectory(ofToDataPath("src_vid", true));
	videoFiles = vidDirectory.getFiles();
	for (int i = 0; i < videoFiles.size(); i++)
	{
		videoFileNames.push_back(videoFiles[i].getFileName());
	}

	ofDirectory imgDirectory(ofToDataPath("src_img", true));
	imgFiles = imgDirectory.getFiles();
	for (int i = 0; i < imgFiles.size(); i++)
	{
		imgFileNames.push_back(imgFiles[i].getFileName());
	}
	
	buildSourceNames();
	gui_loadPresets();

	currentSourceIndex = ofRandom(0, sourceNames.size() - 1);
	currentPlotStyleIndex = ofRandom(0, v_PlotStyles.size() - 1);
	gui_loadSourceIndex();
}

void ofApp::gui_loadPresets() {
	ofDirectory presetDirectory(ofToDataPath("presets", true));
	presetFileNames.clear();
	presetFiles = presetDirectory.getFiles();
	for (int i = 0; i < presetFiles.size(); i++)
	{
		string base_filename = presetFiles[i].getFileName();
		string pname = base_filename.substr(0, base_filename.find_last_of('.'));
		presetFileNames.push_back(pname);
	}
}

//--------------------------------------------------------------
void ofApp::gui_loadSourceIndex() {
	if (currentSourceIndex > (videoDevices.size() + videoFiles.size()) - 1) {
		loadImage(imgFiles[currentSourceIndex - videoDevices.size() - videoFiles.size()].getAbsolutePath());
	}
	else if (currentSourceIndex > videoDevices.size() - 1) {
		loadVideo(videoFiles[currentSourceIndex - videoDevices.size()].getAbsolutePath());
	}
	else {
		bUseVideoDevice = true;
		for (vector<ofVideoDevice>::iterator it = videoDevices.begin(); it != videoDevices.end(); ++it) {
			if (it->deviceName == sourceNames[currentSourceIndex]) {
				videoGrabber.setDeviceID(it->id);
				break;
			}
		}
	}
}

//--------------------------------------------------------------
void ofApp::exit() {
	//clean
}

//--------------------------------------------------------------
void ofApp::update() {
	//if (ofGetFrameNum() % 500 == 0) updateFbo();
	if (!pauseRender) {
		
		if (bUseVideoDevice) {
			videoGrabber.update();
			if (videoGrabber.isFrameNew()) {
				img.setFromPixels(videoGrabber.getPixels());
				img.mirror(false, true); // Webcam
				prep_img();
			}
		}
		else if (bUseVideo) {
			videoPlayer.update();
			if (videoPlayer.isFrameNew()) {
				img.setFromPixels(videoPlayer.getPixels());
				prep_img();
			}
		}

		updateFbo();
	}
	
	if (showZoom) {
		zoomFbo.begin();
		ofClear(c_paper);
		float fX = max((float)0, min(((mouseX - offset.x) * zoomMultiplier) - halfZoomWindowW, fbo.getWidth() - zoomWindowW));
		float fY = max((float)0, min(((mouseY - offset.y) * zoomMultiplier) - halfZoomWindowH, fbo.getHeight() - zoomWindowH));
		fbo.getTexture().drawSubsection(0, 0, zoomWindowW, zoomWindowH, fX, fY);
		zoomFbo.end();
	}
}

void ofApp::updateFbo() {
	fbo.begin();
	// This should be optional
	ofClear(c_paper);

	if (saveVector) {
		ofBeginSaveScreenAsPDF( "export//" + img_name + "_" + v_PlotStyles[currentPlotStyleIndex] + "_" + to_string(++exportCount) + ".pdf", false);
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

	fbo.draw(glm::vec2(offset.x, offset.y), img.getWidth(), img.getHeight());

	if (showZoom) {
		float zX = max(offset.x, min(mouseX - halfZoomWindowW, (img.getWidth()  + offset.x) - zoomWindowW));
		float zY = max(offset.y, min(mouseY - halfZoomWindowH, (img.getHeight() + offset.y) - zoomWindowH));

		ofPushStyle();
		ofFill();
		ofSetColor(ofColor(255,255,255,255));
		ofDrawRectangle(zX, zY, zoomWindowW, zoomWindowH);
		ofSetColor(c_background);
		ofNoFill();
		ofSetLineWidth(1);
		ofDrawRectangle(zX-1, zY-1, zoomWindowW+2, zoomWindowH+2);
		ofPopStyle();

		zoomFbo.draw(glm::vec2(zX, zY), zoomWindowW, zoomWindowH);
	}

	if (showImage) {
		img.draw(offset.x, offset.y);
	}

	showUI();
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

float ofApp::percentage(float percent, float total) {
	return (percent / 100) * total;
}


void ofApp::buildSourceNames() {
	sourceNames.clear();
	sourceNames.insert(sourceNames.end(), videoDeviceNames.begin(), videoDeviceNames.end());
	sourceNames.insert(sourceNames.end(), videoFileNames.begin(),   videoFileNames.end());
	sourceNames.insert(sourceNames.end(), imgFileNames.begin(),     imgFileNames.end());
}

void ofApp::loadImage(string& filepath) {

	original.load(filepath);
	img.load(filepath);

	std::string base_filename = filepath.substr(filepath.find_last_of("/\\") + 1);
	img_name = base_filename.substr(0, base_filename.find_last_of('.'));

	prep_img();
	bUseVideo = false;
	bUseVideoDevice = false;

}

void ofApp::loadVideo(string& filepath) {

	bUseVideo = true;
	bUseVideoDevice = false;

	videoPlayer.load(filepath);
	videoPlayer.play();

	std::string base_filename = filepath.substr(filepath.find_last_of("/\\") + 1);
	img_name = base_filename.substr(0, base_filename.find_last_of('.'));

}

void ofApp::prep_img() {
	// Resize image fit screen
	(img.getWidth() > img.getHeight()) ? isLandscape = true : isLandscape = false;
	if (isLandscape) {
		ratio = img.getHeight() / img.getWidth();
		img.resize(ofGetWidth() - gui_width, (ofGetWidth() - gui_width) * ratio);
	}
	else {
		ratio = img.getWidth() / img.getHeight();
		img.resize(ofGetHeight() * ratio, ofGetHeight());
	}

	fbo.allocate(img.getWidth() * zoomMultiplier, img.getHeight() * zoomMultiplier, GL_RGBA, 8);

	offset.x = ((ofGetWidth() - gui_width) - img.getWidth()) * 0.5;
	offset.y = (ofGetHeight() - img.getHeight()) * 0.5;
}

void ofApp::onImageChange(string& filepath) {
	loadImage(filepath);
}

int ofApp::getIndex(vector<std::string> v, std::string s, int notFound) {
	auto it = find(v.begin(), v.end(), s);

	if (it != v.end())
	{
		return it - v.begin();
	}
	else {
		return notFound;
	}
}

void ofApp::saveSettings(string& filepath) {
	ofxXmlSettings settings;
	settings.setValue("pixel_plotter:plotStyle", v_PlotStyles[currentPlotStyleIndex]);
	settings.setValue("pixel_plotter:blendmode", v_BlendModes[currentBlendModeIndex]);
	settings.setValue("pixel_plotter:tilesX", tilesX);
	settings.setValue("pixel_plotter:tilesY", tilesY);
	settings.setValue("pixel_plotter:addonx", addonx);
	settings.setValue("pixel_plotter:addony", addony);
	settings.setValue("pixel_plotter:everynx", everynx);
	settings.setValue("pixel_plotter:everyny", everyny);
	settings.setValue("pixel_plotter:randomOffset", randomOffset);
	settings.setValue("pixel_plotter:noisepercentX", noisepercentX);
	settings.setValue("pixel_plotter:noisepercentY", noisepercentY);
	settings.setValue("pixel_plotter:roundPixels", roundPixels);
	settings.saveFile(filepath);
}

void ofApp::loadSettings(string& filepath) {
	ofxXmlSettings settings;
	settings.loadFile(filepath);

	// Below needs proper getter ...b 
	currentPlotStyleIndex = getIndex(v_PlotStyles, settings.getValue("pixel_plotter:plotStyle", "0"), 0);
	currentBlendModeIndex = getIndex(v_BlendModes, settings.getValue("pixel_plotter:blendmode", "0"), 0);
	tilesX = settings.getValue("pixel_plotter:tilesX", 64);
	tilesY = settings.getValue("pixel_plotter:tilesY", 64);
	addonx = settings.getValue("pixel_plotter:addonx", 0);
	addony = settings.getValue("pixel_plotter:addony", 0);

	everynx = settings.getValue("pixel_plotter:everynx", 4);
	everyny = settings.getValue("pixel_plotter:everyny", 4);
	randomOffset = settings.getValue("pixel_plotter:randomOffset", 0);
	noisepercentX = settings.getValue("pixel_plotter:noisepercentX", 0);
	noisepercentY = settings.getValue("pixel_plotter:noisepercentY", 0);
	roundPixels = settings.getValue("pixel_plotter:roundPixels", false);
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
