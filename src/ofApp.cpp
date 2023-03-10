#include "ofApp.h"

/*
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
	ofSetLogLevel(OF_LOG_FATAL_ERROR);
	//ofEnableAlphaBlending();
	
	//ofSetLogLevel(OF_LOG_WARNING);
	//ofSetLogLevel(OF_LOG_VERBOSE);
	//ofSetBackgroundAuto(false);
	ofSetWindowTitle("Pixel Plotter");

	videoDevices = videoGrabber.listDevices();
	for (vector<ofVideoDevice>::iterator it = videoDevices.begin(); it != videoDevices.end(); ++it) {
		videoDeviceNames.push_back(it->deviceName);
	}

	//ofLog() << ofFbo::checkGLSupport();
	zoomFbo.allocate(zoomWindowW, zoomWindowH, GL_RGB, 8);

	gui.setup();
	ImGui::StyleColorsDark();
	ImGuiStyle* style = &ImGui::GetStyle();
	style->ItemSpacing = ImVec2(5, 5);

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

	gui_buildSourceNames();
	addDrawFilter(ofRandom(1, v_DrawFilterNames.size() - 1));

	gui_loadPresets();

	currentSourceIndex = ofRandom(videoDeviceNames.size() + videoFileNames.size(), sourceNames.size() - 1);
	gui_loadSourceIndex();

}

//--------------------------------------------------------------
void ofApp::exit() {
	
}

//--------------------------------------------------------------
void ofApp::update() {

	if (cleanFilters) {
		for (int i = 0; i < v_DrawFilters.size(); i++) {
			if (!v_DrawFilters[i]->active) {
				delete v_DrawFilters[i];
				v_DrawFilters[i] = nullptr;
			}
		}
		v_DrawFilters.erase(std::remove(v_DrawFilters.begin(), v_DrawFilters.end(), nullptr), v_DrawFilters.end());
	}

	if (!pauseRender) {

		if (bUseVideoDevice) {
			videoGrabber.update();
			if (videoGrabber.isFrameNew()) {
				img.setFromPixels(videoGrabber.getPixels());
				img.mirror(false, true);
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
}


void ofApp::updateFbo() {
	canvasFbo.begin();

	if (saveVector) {
		ofBeginSaveScreenAsPDF( "export//" + img_name + "_" + v_DrawFilterNames[currentDrawFilterIndex] + "_" + to_string(++exportCount) + ".pdf", false);
	}

	ofClear(c_paper);

	for (const auto& filter : v_DrawFilters) {
		filter->draw(&img);
	}

	if (saveVector) {
		ofEndSaveScreenAsPDF();
		saveVector = false;
	}

	canvasFbo.end();
}


//--------------------------------------------------------------
void ofApp::draw(){


	canvasFbo.draw(glm::vec2(offset.x, offset.y), img.getWidth(), img.getHeight());

	if (showImage) {
		img.draw(offset.x, offset.y);
	}

	gui_showMain();
}

void ofApp::addDrawFilter(int index) {
	if (v_DrawFilterNames[index] == "Pixelate") {
		v_DrawFilters.push_back(new Df_pixelate);
	}
	if (v_DrawFilterNames[index] == "Rings") {
		v_DrawFilters.push_back(new Df_rings);
	}
}

void ofApp::loadImage(string& filepath) {

	original.load(filepath);
	img.load(filepath);
	img.setImageType(OF_IMAGE_COLOR);

	std::string base_filename = filepath.substr(filepath.find_last_of("/\\") + 1);
	img_name = base_filename.substr(0, base_filename.find_last_of('.'));

	prep_img();
	bUseVideo = false;
	bUseVideoDevice = false;
	videoPlayer.stop();
	videoPlayer.close();

}

void ofApp::loadVideo(string& filepath) {

	bUseVideo = true;
	bUseVideoDevice = false;

	videoPlayer.load(filepath);
	videoPlayer.setLoopState(OF_LOOP_NORMAL);
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

	canvasFbo.allocate(img.getWidth() * zoomMultiplier, img.getHeight() * zoomMultiplier, GL_RGB, 8);

	offset.x = ((ofGetWidth() - gui_width) - img.getWidth()) * 0.5;
	offset.y = (ofGetHeight() - img.getHeight()) * 0.5;
}

void ofApp::onImageChange(string& filepath) {
	loadImage(filepath);
}

void ofApp::saveSettings(string& filepath) {
	ofxXmlSettings settings;

	settings.addTag("drawFilters");
	settings.pushTag("drawFilters");
	for (int i = 0; i < v_DrawFilters.size(); i++) {
		ofxXmlSettings filterSettings = v_DrawFilters[i]->getSettings();
		string drawFilterSettings;
		filterSettings.copyXmlToString(drawFilterSettings);
		string filterName = v_DrawFilters[i]->name;
		settings.addValue("string_settings", drawFilterSettings);
	}
	settings.popTag();

	settings.setValue("appSettings:CanvasColour:r", c_paper.x);
	settings.setValue("appSettings:CanvasColour:g", c_paper.y);
	settings.setValue("appSettings:CanvasColour:b", c_paper.z);
	settings.setValue("appSettings:CanvasColour:a", c_paper.w);

	settings.saveFile(filepath);
}

void ofApp::loadSettings(string& filepath) {
	ofxXmlSettings settings;
	settings.loadFile(filepath);

	c_paper.x = settings.getValue("appSettings:CanvasColour:r", 255);
	c_paper.y = settings.getValue("appSettings:CanvasColour:g", 255);
	c_paper.z = settings.getValue("appSettings:CanvasColour:b", 255);
	c_paper.w = settings.getValue("appSettings:CanvasColour:a", 255);

	// Clear all Filters
	for (int i = 0; i < v_DrawFilters.size(); i++) {
		delete v_DrawFilters[i];
		v_DrawFilters[i] = nullptr;
	}
	v_DrawFilters.erase(std::remove(v_DrawFilters.begin(), v_DrawFilters.end(), nullptr), v_DrawFilters.end());

	// Add as we go through settings file
	if (settings.tagExists("drawFilters")) {
		settings.pushTag("drawFilters");
		int count = settings.getNumTags("string_settings");
		for (int i = 0; i < count; i++) {
			ofxXmlSettings filterSettings;
			string stringSettings = settings.getValue("string_settings", "", i);
			filterSettings.loadFromBuffer(stringSettings);
			string filterName = filterSettings.getValue("name", "not_found");

			if (filterName == "Pixelate") {
				v_DrawFilters.push_back(new Df_pixelate);
				v_DrawFilters[v_DrawFilters.size() - 1]->loadSettings(filterSettings);
			} else if (filterName == "Rings") {
				v_DrawFilters.push_back(new Df_rings);
			}
		}
		settings.popTag();
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
		for (int i = 0; i < dragInfo.files.size(); i++) {
			if (std::find(img_ext.begin(), img_ext.end(), to_lower(dragInfo.files[i].substr(dragInfo.files[i].find_last_of(".") + 1))) != img_ext.end())
			{
				loadImage(dragInfo.files[i]);
			} 
			else if (std::find(vid_ext.begin(), vid_ext.end(), to_lower(dragInfo.files[i].substr(dragInfo.files[i].find_last_of(".") + 1))) != vid_ext.end())
			{
				loadVideo(dragInfo.files[i]);
			}
			else {
				ofLog(OF_LOG_ERROR) << "No support for file format " << to_lower(dragInfo.files[i].substr(dragInfo.files[i].find_last_of(".") + 1));
			}
		}
	}
}
