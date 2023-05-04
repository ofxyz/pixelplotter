#include "ofApp.h"

/*
   - Pixelate filter: Add curve widget https://github.com/ocornut/imgui/issues/786
   - Pixelate filter: Create spacing adjustment curve
   - Fix Zoom and make zoom go faster with holding shift
   - Implement alpha on all images and resources
   - Make sure UI is always rendering fast
   - Add video controls pause, play, next frame, previous frame
   - Add draw order, left to right top to bottom, center out, center in
   - Add duplicate button for filters
   - Support for multiple sources (Add source select per filter?)
   - Add rotation -> Map -> X, Y location
   - Reorder colour layer (based on N x, y)
   - Optional adjust colour hue  (based on N x, y, etc)
   - RGB needs B/W check
   - CMYK needs white control option (Nim: Black and white. CMYK RGB)
   - Add modulators (EG X Y tiles between min-max, time)
   - I want to set a value to the colour pallette of four colours
     To control how much each colour is used. Main Mid Accent? Weight.
   - Push X / Y / C to vector for sorting and drawing order.
   - Souce effects on GPU with shaders. Output vector. 
   - Posterise Source (shader?) 

   ## Generators (They should be able to be placed in the filter section or source section)
   - Add grid generator (with noise curve option)

   ## Filters
   - Add mesh filter
   - Use gradients
   - Add hatch filter
   - Add asym hex pixel to Pixelate filter.
*/

//--------------------------------------------------------------

ofx2d x2d;

void ofApp::setup() {
	bLoadSettingsNextFrame = false;
	ofLogToConsole();
	ofSetLogLevel(OF_LOG_ERROR);
	//ofSetLogLevel(OF_LOG_VERBOSE);
	ofSetVerticalSync(false);
	ofLog() << ofFbo::checkGLSupport();
	ofSetWindowTitle("Canvas");
	ofEnableAlphaBlending();

	//ofHideCursor();
	ofAddListener(ofEvents().mouseScrolled, this, &ofApp::mouseScrolled);

	//ofSetBackgroundAuto(false);

	soundManager.setup(this);
	sourceController.setup(this);
	plotCanvas.setup(this, &sourceController.frameBuffer.getFrame());

	userOffset.x = 0;
	userOffset.y = 0;

	gui_loadPresets();

	plotCanvas.dF->addRandomFilter();

}

//--------------------------------------------------------------
void ofApp::exit() {
	ofRemoveListener(ofEvents().mouseScrolled, this, &ofApp::mouseScrolled);
	OF_EXIT_APP(0);
}

//--------------------------------------------------------------
void ofApp::update() {
	std::stringstream strm;
	strm << "Canvas (FPS: " << ofGetFrameRate() << ")";
	ofSetWindowTitle(strm.str());

	soundManager.update();
	//gui_update();
	if (bLoadSettingsNextFrame)
	{
		loadSettings(presetFiles[currentPresetIndex].getAbsolutePath());
		bLoadSettingsNextFrame = false;
	}
	if (sourceController.loadImageNextFrame)
	{
		sourceController.loadSourceIndex();
		sourceController.loadImageNextFrame = false;
	}
	if (!pauseRender) {
		if (plotCanvas.resizeRequest) {
			sourceController.isFresh = true;
		}
		plotCanvas.update();
		sourceController.update();

		if (sourceController.isResized) {
			sourceController.isResized = false;
			plotCanvas.setSourceDimension(sourceController.frameBuffer.getWidth(), sourceController.frameBuffer.getHeight());
		}

		if (sourceController.isFresh) {
			plotCanvas.update(&sourceController.frameBuffer.getFrame());
			sourceController.isFresh = false;
		} else if (sourceController.frameBuffer.isFresh() || plotCanvas.isFresh()) {
			plotCanvas.update(&sourceController.frameBuffer.getFrame());
		}
	}
}

//--------------------------------------------------------------
void ofApp::draw() {

	plotCanvas.draw(offset.x + userOffset.x, offset.y + userOffset.y, plotCanvas.canvasWidth * zoomLevel, plotCanvas.canvasHeight * zoomLevel);
	
	if (sourceController.showSource) {
		sourceController.frameBuffer.getFrame().draw(offset.x + userOffset.x, offset.y + userOffset.y, plotCanvas.canvasWidth * zoomLevel, plotCanvas.canvasHeight * zoomLevel);
	}

	//gui_draw();
}

void ofApp::resetImageOffset() {
	// Context swiytching multiple windows
	offset.x = ((ofGetWidth() - (plotCanvas.canvasWidth * zoomLevel)) * 0.5) + (gui_width-12); // Need to fix this with context...
	offset.y = (ofGetHeight() - (plotCanvas.canvasHeight * zoomLevel)) * 0.5;
}

void ofApp::resetZoom() {
	zoomLevel = 1;
	resetImageOffset();
	userOffset.x = 0;
	userOffset.y = 0;
}

void ofApp::saveSettings(string& filepath) {
	ofxXmlSettings settings;

	settings.addTag("source");
	settings.pushTag("source");
	ofxXmlSettings sourceSettings = sourceController.getSettings();
	string sSourceSettings;
	sourceSettings.copyXmlToString(sSourceSettings);
	settings.addValue("string_settings", sSourceSettings);
	settings.popTag();

	settings.addTag("imageFilters");
	settings.pushTag("imageFilters");
	for (int i = 0; i < sourceController.iF.v_ImageFilters.size(); i++) {
		ofxXmlSettings imgFilterSettings = sourceController.iF.v_ImageFilters[i]->getSettings();
		string imageFilterSettings;
		imgFilterSettings.copyXmlToString(imageFilterSettings);
		settings.addValue("string_settings", imageFilterSettings);
	}
	settings.popTag();

	settings.addTag("drawFilters");
	settings.pushTag("drawFilters");
	for (int i = 0; i < plotCanvas.dF->v_DrawFilters.size(); i++) {
		ofxXmlSettings filterSettings = plotCanvas.dF->v_DrawFilters[i]->getSettings();
		string drawFilterSettings;
		filterSettings.copyXmlToString(drawFilterSettings);
		settings.addValue("string_settings", drawFilterSettings);
	}
	settings.popTag();

	settings.addTag("plotCanvas");
	settings.pushTag("plotCanvas");
	ofxXmlSettings canvasSettings = plotCanvas.getSettings();
	string sCanvasSettings;
	canvasSettings.copyXmlToString(sCanvasSettings);
	settings.addValue("string_settings", sCanvasSettings);
	settings.popTag();

	settings.saveFile(filepath);
}

void ofApp::loadSettings(string& filepath) {
	ofxXmlSettings settings;
	settings.loadFile(filepath);

	plotCanvas.dF->clearFilters();
	sourceController.iF.clearFilters();

	if (settings.tagExists("source")) {
		settings.pushTag("source");
		ofxXmlSettings sourceSettings;
		string sSourceSettings = settings.getValue("string_settings", "");
		sourceSettings.loadFromBuffer(sSourceSettings);
		sourceController.loadSettings(sourceSettings);
		settings.popTag();
		plotCanvas.fresh = true;
	}

	if (settings.tagExists("imageFilters")) {
		settings.pushTag("imageFilters");
		int count = settings.getNumTags("string_settings");
		for (int i = 0; i < count; i++) {
			ofxXmlSettings filterSettings;
			string stringSettings = settings.getValue("string_settings", "", i);
			filterSettings.loadFromBuffer(stringSettings);
			sourceController.iF.addFilter(filterSettings);
		}
		settings.popTag();
		sourceController.isFresh = true;
	}

	if (settings.tagExists("drawFilters")) {
		settings.pushTag("drawFilters");
		int count = settings.getNumTags("string_settings");
		for (int i = 0; i < count; i++) {
			ofxXmlSettings filterSettings;
			string stringSettings = settings.getValue("string_settings", "", i);
			filterSettings.loadFromBuffer(stringSettings);
			plotCanvas.dF->addFilter(filterSettings);
		}
		plotCanvas.fresh = true;
		settings.popTag();
	}

	if (settings.tagExists("plotCanvas")) {
		settings.pushTag("plotCanvas");
		ofxXmlSettings canvasSettings;
		string sCanvasSettings = settings.getValue("string_settings", "");
		canvasSettings.loadFromBuffer(sCanvasSettings);
		plotCanvas.loadSettings(canvasSettings);
		settings.popTag();
		plotCanvas.fresh = true;
	}
}

//-------------------------------------------------------------
void ofApp::keyPressed(int key) { 

	if (key == 'g' || key == 'G') {
		bShowGui = !bShowGui;
	}
	else if (key == '-') {
		zoomLevel -= 0.1;
		resetImageOffset();
	}
	else if (key == '+') {
		zoomLevel += 0.1;
		resetImageOffset();
	}
	else if (key == '_') {
		// center image
		userOffset.x = 0;
		userOffset.y = 0;
	}
	else if (key == '=') {
		// fit to screen
		userOffset.x = 0;
		userOffset.y = 0;
		if (sourceController.isLandscape) {
			zoomLevel = (ofGetWidth() - gui_width) / plotCanvas.canvasWidth;
		}
		else {
			zoomLevel = ofGetHeight() / plotCanvas.canvasHeight;
		}
		resetImageOffset();
	}
	else if (key == 'p') {
		plotCanvas.saveVector = true;
	}
	else if (key == 'x') {
		pauseRender = !pauseRender;
	}
	else if (key == '?') {
		//cout << x << end=l;
	}
}

//--------------------------------------------------------------
void ofApp::audioIn(ofSoundBuffer& input) {
	soundManager.audioIn(input);
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key) {

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button) {
	if (bDragCanvas) {
		// This doesn't work anymore either...
		// Seems to be offset by UI.
		// Maybe create this contect first and UI second?
		//if (x > ofGetWidth() || x < 0) return;
		//if (y > ofGetHeight() || y < 0) return;
		userOffset.x += x - lastDraggedPos.x;
		userOffset.y += y - lastDraggedPos.y;
	}

	// Update last drag location
	lastDraggedPos.x = x;
	lastDraggedPos.y = y;
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button) {
	if (x < (ofGetWindowWidth() - gui_width) && x > 0) {
		bDragCanvas = true;
		lastDraggedPos.x = x;
		lastDraggedPos.y = y;
	}
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button) {
	bDragCanvas = false;
}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::mouseScrolled(ofMouseEventArgs& mouse) {
	if (mouse.x < (ofGetWindowWidth()-gui_width)) {
		glm::vec3 position = plotCanvas.cam.getPosition();
		if (zoomLevel > 1) {
			zoomLevel += (mouse.scrollY * 0.1);
		}
		else {
			zoomLevel += (mouse.scrollY * 0.01);
		}
		if (zoomLevel < 0.02) zoomLevel = 0.02;
	}
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
		for (int i = 0; i < dragInfo.files.size(); i++) {
			if (std::find(sourceController.img_ext.begin(), sourceController.img_ext.end(), x2d.to_lower(dragInfo.files[i].substr(dragInfo.files[i].find_last_of(".") + 1))) != sourceController.img_ext.end())
			{
				sourceController.addImage(ofFile(dragInfo.files[i]));
				resetZoom();
			} 
			else if (std::find(sourceController.vid_ext.begin(), sourceController.vid_ext.end(), x2d.to_lower(dragInfo.files[i].substr(dragInfo.files[i].find_last_of(".") + 1))) != sourceController.vid_ext.end())
			{
				sourceController.addVideo(ofFile(dragInfo.files[i]));
				resetZoom();
			}
			else {
				ofLog(OF_LOG_ERROR) << "No support for file format " << x2d.to_lower(dragInfo.files[i].substr(dragInfo.files[i].find_last_of(".") + 1));
			}
		}
	}
}
