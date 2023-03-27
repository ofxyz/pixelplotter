#include "ofApp.h"

/*
   - Make zoom go faster with holding shift
   - Implement alpha on all images and resources
   - Make sure canvas settings are saved properly
   - Make sure UI is always rendering fast
   - Pause Render update on variable change
   - Add video controls pause, play, next frame, previous frame
   - Add Canvas Size, zoom canvas, like normal
   - Add draw order, left to right top to bottom, center out, center in
   - Add duplicate button for filters
   - Add move up/down for filters (Reorder filters)
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
	ofLogToConsole();
	ofSetLogLevel(OF_LOG_ERROR);
	ofSetVerticalSync(false);
	ofLog() << ofFbo::checkGLSupport();

	ofSetWindowTitle("Pixel Plotter");
	ofEnableAlphaBlending();

	//ofHideCursor();
	ofAddListener(ofEvents().mouseScrolled, this, &ofApp::mouseScrolled);

	//ofSetBackgroundAuto(false);

	userOffset.x = 0;
	userOffset.y = 0;

	gui.setup();

	ImGui::StyleColorsDark();
	ImGuiStyle* style = &ImGui::GetStyle();
	style->ItemSpacing = ImVec2(5, 5);

	sourceController.setup();

	
	gui_loadPresets();

	canvas.dF.addRandomFilter();

}

//--------------------------------------------------------------
void ofApp::exit() {
	ofRemoveListener(ofEvents().mouseScrolled, this, &ofApp::mouseScrolled);
}

//--------------------------------------------------------------
void ofApp::update() {
	gui_update();

	if (!pauseRender) {
		sourceController.update();
		canvas.update();

		if (sourceController.isFresh) {
			canvas.setup(&sourceController.frameBuffer.getFrame());
			sourceController.isFresh = false;
		} 
		else if (sourceController.frameBuffer.isFresh() || canvas.isFresh()) {
			canvas.update(&sourceController.frameBuffer.getFrame());
		}
	}
}

//--------------------------------------------------------------
void ofApp::draw(){
	canvas.draw(offset.x + userOffset.x, offset.y + userOffset.y, sourceController.frameBuffer.getFrame().getWidth() * zoomLevel, sourceController.frameBuffer.getFrame().getHeight() * zoomLevel);

	if (showImage) {
		sourceController.frameBuffer.getFrame().draw(offset.x + userOffset.x, offset.y + userOffset.y, sourceController.frameBuffer.getFrame().getWidth() * zoomLevel, sourceController.frameBuffer.getFrame().getHeight() * zoomLevel);
	}

	gui_draw();
}

void ofApp::resetImageOffset() {
	offset.x = (ofGetWidth() - gui_width - (sourceController.frameBuffer.getFrame().getWidth() * zoomLevel)) * 0.5;
	offset.y = (ofGetHeight() - (sourceController.frameBuffer.getFrame().getHeight() * zoomLevel)) * 0.5;
}

void ofApp::loadImage(string& filepath) {
	sourceController.loadImage(filepath);
	canvas.setup(&sourceController.frameBuffer.getFrame(), sourceController.src_name);
	resetImageOffset();
	zoomLevel = 1;
	userOffset.x = 0;
	userOffset.y = 0;
}

void ofApp::loadVideo(string& filepath) {
	sourceController.loadVideo(filepath);
	canvas.setup(&sourceController.frameBuffer.getFrame(), sourceController.src_name);
	resetImageOffset();
	zoomLevel = 1;
	userOffset.x = 0;
	userOffset.y = 0;
}

void ofApp::onImageChange(string& filepath) {
	loadImage(filepath);
}

void ofApp::saveSettings(string& filepath) {
	ofxXmlSettings settings;

	settings.addTag("imageFilters");
	settings.pushTag("imageFilters");
	for (int i = 0; i < sourceController.iF.v_ImageFilters.size(); i++) {
		ofxXmlSettings imgFilterSettings = sourceController.iF.v_ImageFilters[i]->getSettings();
		string imageFilterSettings;
		imgFilterSettings.copyXmlToString(imageFilterSettings);
		string filterName = sourceController.iF.v_ImageFilters[i]->name;
		settings.addValue("string_settings", imageFilterSettings);
	}
	settings.popTag();

	settings.addTag("drawFilters");
	settings.pushTag("drawFilters");
	for (int i = 0; i < canvas.dF.v_DrawFilters.size(); i++) {
		ofxXmlSettings filterSettings = canvas.dF.v_DrawFilters[i]->getSettings();
		string drawFilterSettings;
		filterSettings.copyXmlToString(drawFilterSettings);
		string filterName = canvas.dF.v_DrawFilters[i]->name;
		settings.addValue("string_settings", drawFilterSettings);
	}
	settings.popTag();

	settings.setValue("appSettings:CanvasColour:r", canvas.c_canvas.x);
	settings.setValue("appSettings:CanvasColour:g", canvas.c_canvas.y);
	settings.setValue("appSettings:CanvasColour:b", canvas.c_canvas.z);
	settings.setValue("appSettings:CanvasColour:a", canvas.c_canvas.w);

	settings.saveFile(filepath);
}

void ofApp::loadSettings(string& filepath) {
	ofxXmlSettings settings;
	settings.loadFile(filepath);

	canvas.c_canvas.x = settings.getValue("appSettings:CanvasColour:r", 255);
	canvas.c_canvas.y = settings.getValue("appSettings:CanvasColour:g", 255);
	canvas.c_canvas.z = settings.getValue("appSettings:CanvasColour:b", 255);
	canvas.c_canvas.w = settings.getValue("appSettings:CanvasColour:a", 255);

	canvas.dF.clearFilters();
	sourceController.iF.clearFilters();

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
	}

	if (settings.tagExists("drawFilters")) {
		settings.pushTag("drawFilters");
		int count = settings.getNumTags("string_settings");
		for (int i = 0; i < count; i++) {
			ofxXmlSettings filterSettings;
			string stringSettings = settings.getValue("string_settings", "", i);
			filterSettings.loadFromBuffer(stringSettings);
			canvas.dF.addFilter(filterSettings);
		}
		settings.popTag();
	}

	sourceController.loadSourceIndex();
}

//-------------------------------------------------------------
void ofApp::keyPressed(int key) { 
	if (key == '-') {
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
			zoomLevel = (ofGetWidth() - gui_width) / sourceController.frameBuffer.getFrame().getWidth();
		}
		else {
			zoomLevel = ofGetHeight() / sourceController.frameBuffer.getFrame().getHeight();
		}
		resetImageOffset();
	}
	else if (key == 'p') {
		canvas.saveVector = true;
	}
	else if (key == 'x') {
		pauseRender = !pauseRender;
	}
	else if (key == '?') {
		//cout << x << endl;
	}
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
		if (x > img_area_WH || x < 0) return;
		if (y > img_area_WH || y < 0) return;
		userOffset.x += x - lastDraggedPos.x;
		userOffset.y += y - lastDraggedPos.y;
	}

	// Update last drag location
	lastDraggedPos.x = x;
	lastDraggedPos.y = y;
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button) {
	if (x < img_area_WH && x > 0) {
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
	if (mouse.x < img_area_WH) {
		glm::vec3 position = canvas.cam.getPosition();
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
				loadImage(dragInfo.files[i]);
			} 
			else if (std::find(sourceController.vid_ext.begin(), sourceController.vid_ext.end(), x2d.to_lower(dragInfo.files[i].substr(dragInfo.files[i].find_last_of(".") + 1))) != sourceController.vid_ext.end())
			{
				loadVideo(dragInfo.files[i]);
			}
			else {
				ofLog(OF_LOG_ERROR) << "No support for file format " << x2d.to_lower(dragInfo.files[i].substr(dragInfo.files[i].find_last_of(".") + 1));
			}
		}
	}
}
