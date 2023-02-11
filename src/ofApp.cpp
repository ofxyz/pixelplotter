#include "ofApp.h"

/*
   - Finish Style Interfaces
   - Add clear screen button or tickbox
   - RGB needs K check and clean options
   - CMYK needs white control option (Nim: Black and white. CMYK RGB)
   - Add modulators (EG X Y tiles between min-max, time)
   - I want to set a value to the colour pallette of four colours
     To control how much each colour is used. Main Mid Accent? Weight.
   - Push X / Y / C to vector for sorting and drawing order.
   - Souce effects on GPU with shaders. Output vector. 
   - Posterise Source (shader)

   ## Filters
   - Add hatch filter
   - Add asym hex pixel to Pixelate filter.
*/

//--------------------------------------------------------------

void ofApp::setup() {
	ofLogToConsole();
	ofSetLogLevel(OF_LOG_FATAL_ERROR);
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
	
	gui_buildSourceNames();
	//gui_loadPresets();
	
	currentDrawFilterIndex = ofRandom(0, v_DrawFilterNames.size() - 1);
	addDrawFilter();

	currentSourceIndex = ofRandom(0, sourceNames.size() - 1);
	gui_loadSourceIndex();
	
}

//--------------------------------------------------------------
void ofApp::exit() {
	//clean
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
				img.mirror(false, true); // Webcam
				prep_img();
			}
		}
		else if (bUseVideo) {
			videoPlayer.update();
			img.setFromPixels(videoPlayer.getPixels());
			prep_img();
		}
		ofSetBackgroundColor(c_background);
		updateFbo();
	}

	if (showZoom) {
		zoomFbo.begin();
		ofClear(c_paper);
		float fX = max((float)0, min(((mouseX - offset.x) * zoomMultiplier) - halfZoomWindowW, canvasFbo.getWidth() - zoomWindowW));
		float fY = max((float)0, min(((mouseY - offset.y) * zoomMultiplier) - halfZoomWindowH, canvasFbo.getHeight() - zoomWindowH));
		canvasFbo.getTexture().drawSubsection(0, 0, zoomWindowW, zoomWindowH, fX, fY);
		zoomFbo.end();
	}
}


void ofApp::updateFbo() {
	canvasFbo.begin();
	ofClear(c_paper);

	if (saveVector) {
		ofBeginSaveScreenAsPDF( "export//" + img_name + "_" + ss.v_PlotStyles[currentPlotStyleIndex] + "_" + to_string(++exportCount) + ".pdf", false);
	}

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

	gui_showMain();
}

void ofApp::addDrawFilter() {
	if (v_DrawFilterNames[currentDrawFilterIndex] == "Pixelate") {
		v_DrawFilters.push_back(new Df_pixelate);
	}
	if (v_DrawFilterNames[currentDrawFilterIndex] == "Rings") {
		v_DrawFilters.push_back(new Df_rings);
	}
}

float ofApp::percentage(float percent, float total) {
	return (percent / 100) * total;
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
	/*
	ofxXmlSettings settings;
	settings.setValue("pixel_plotter:plotStyle", ss.v_PlotStyles[currentPlotStyleIndex]);
	settings.setValue("pixel_plotter:blendmode", ss.v_BlendModes[currentBlendModeIndex]);
	s1ettings.setValue("pixel_plotter:tilesX", ss.tilesX);
	settings.setValue("pixel_plotter:tilesY", ss.tilesY);
	settings.setValue("pixel_plotter:addonx", ss.addonx);
	settings.setValue("pixel_plotter:addony", ss.addony);
	settings.setValue("pixel_plotter:everynx", ss.everynx);
	settings.setValue("pixel_plotter:everyny", ss.everyny);
	settings.setValue("pixel_plotter:randomOffset", ss.randomOffset);
	settings.setValue("pixel_plotter:noisepercentX", ss.noisepercentX);
	settings.setValue("pixel_plotter:noisepercentY", ss.noisepercentY);
	settings.setValue("pixel_plotter:roundPixels", ss.roundPixels);
	settings.saveFile(filepath);
	*/
}

void ofApp::loadSettings(string& filepath) {
	/*
	ofxXmlSettings settings;
	settings.loadFile(filepath);

	currentPlotStyleIndex = getIndex(ss.v_PlotStyles, settings.getValue("pixel_plotter:plotStyle", "0"), 0);
	currentBlendModeIndex = getIndex(ss.v_BlendModes, settings.getValue("pixel_plotter:blendmode", "0"), 0);
	ss.tilesX = settings.getValue("pixel_plotter:tilesX", 64);
	ss.tilesY = settings.getValue("pixel_plotter:tilesY", 64);
	ss.addonx = settings.getValue("pixel_plotter:addonx", 0);
	ss.addony = settings.getValue("pixel_plotter:addony", 0);

	ss.everynx = settings.getValue("pixel_plotter:everynx", 4);
	ss.everyny = settings.getValue("pixel_plotter:everyny", 4);
	ss.randomOffset = settings.getValue("pixel_plotter:randomOffset", 0);
	ss.noisepercentX = settings.getValue("pixel_plotter:noisepercentX", 0);
	ss.noisepercentY = settings.getValue("pixel_plotter:noisepercentY", 0);
	ss.roundPixels = settings.getValue("pixel_plotter:roundPixels", false);
	*/
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
