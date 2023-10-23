#include "ofApp.h"
#include "ofx2d.h"

void ofApp::setup() {

	bLoadSettingsNextFrame = false;

	//ofSetLogLevel(OF_LOG_ERROR);
	//ofSetLogLevel(OF_LOG_VERBOSE);
	ofSetVerticalSync(vSync);
	ofLog(OF_LOG_VERBOSE) << ofFbo::checkGLSupport();
	ofEnableAlphaBlending();

	/* Always disable rectangular textures by default */
	ofDisableArbTex();

	//ofHideCursor();
	ofAddListener(ofEvents().mouseScrolled, this, &ofApp::mouseScrolled);

	//ofSetBackgroundAuto(false);

	gui_setup();

	soundManager.setup(this);

	plotCanvas.setup(this);

	userOffset.x = 0;
	userOffset.y = 0;

	gui_loadPresets();

	plotCanvas.dF.addRandomFilter();
	resetImageOffset();
}

//--------------------------------------------------------------
void ofApp::exit() {
	ofRemoveListener(ofEvents().mouseScrolled, this, &ofApp::mouseScrolled);
}

//--------------------------------------------------------------
void ofApp::update() {
	gui_update();

	if (bLoadSettingsNextFrame)
	{
		loadSettings(presetFiles[currentPresetIndex].getAbsolutePath());
		bLoadSettingsNextFrame = false;
	}
	if (plotCanvas.sourceController.loadImageNextFrame)
	{
		plotCanvas.sourceController.loadSourceIndex();
		plotCanvas.sourceController.loadImageNextFrame = false;
	}

	if (!pauseRender) {
		soundManager.update();
		plotCanvas.update();
	}
}

//--------------------------------------------------------------
void ofApp::draw() {

	if (plotCanvas.sourceController.showSource) {
		plotCanvas.sourceController.frameBuffer.getFrame().draw(offset.x + userOffset.x, offset.y + userOffset.y, plotCanvas.canvasWidth * zoomLevel, plotCanvas.canvasHeight * zoomLevel);
	}
	else {
		plotCanvas.draw(offset.x + userOffset.x, offset.y + userOffset.y, plotCanvas.canvasWidth * zoomLevel, plotCanvas.canvasHeight * zoomLevel);
	}

	gui_draw();
}

void ofApp::resetImageOffset() {
	userOffset.x = 0;
	userOffset.y = 0;
}

void ofApp::resetZoom() {
	zoomLevel = 1;
	resetImageOffset();
}

void ofApp::centerImage() {
	ImRect availableSpace = ImGui::DockBuilderGetCentralNode(MainDockNodeID)->Rect();
	userOffset.x = (availableSpace.GetWidth() - (plotCanvas.canvasWidth * zoomLevel)) * 0.5;
	userOffset.y = (availableSpace.GetHeight() - (plotCanvas.canvasHeight * zoomLevel)) * 0.5;
}

void ofApp::saveSettings(string& filepath) {
	ofJson settings;

	settings["source"].push_back(plotCanvas.sourceController.getSettings());

	for (int i = 0; i < plotCanvas.sourceController.iF.v_ImageFilters.size(); i++) {
		settings["imageFilters"].push_back(plotCanvas.sourceController.iF.v_ImageFilters[i]->getSettings());
	}

	for (int i = 0; i < plotCanvas.dF.v_DrawFilters.size(); i++) {
		settings["drawFilters"].push_back(plotCanvas.dF.v_DrawFilters[i]->getSettings());
	}

	settings["plotCanvas"] = plotCanvas.getSettings();

	ofSavePrettyJson(filepath, settings);

}

void ofApp::loadSettings(string& filepath) {
	ofJson settings;
	ofFile file(filepath);
	if (file.exists()) {
		file >> settings;
	}

	plotCanvas.dF.clearFilters();
	plotCanvas.sourceController.iF.clearFilters();
	
	if (bTryLoadSource) {
		ofJson sources= settings.value("source", ofJson::array());
		if (!sources.empty())
		{
			for (auto& cSettings : sources) {
				plotCanvas.sourceController.loadSettings(cSettings);
			}
		}
	}

	ofJson iFilters = settings.value("imageFilters", ofJson::array());
	if (!iFilters.empty()) {
		for (auto& fSettings : iFilters) {
				plotCanvas.sourceController.iF.addFilter(fSettings);
		}
	}

	ofJson dDilters = settings.value("drawFilters", ofJson::array());
	if (!dDilters.empty()) {
		for (auto& fSettings : dDilters) {
			plotCanvas.dF.addFilter(fSettings);
		}
	}

	plotCanvas.loadSettings(settings.value("plotCanvas", ofJson::array()));

}

//-------------------------------------------------------------
void ofApp::keyPressed(int key) {

	if (key == 'g' || key == 'G') {
		bShowMenuBar = !bShowMenuBar;
		//bShowGui = !bShowGui;
	}
	else if (key == '-') {
		zoomLevel -= 0.1;
		//resetImageOffset();
	}
	else if (key == '+') {
		zoomLevel += 0.1;
		//resetImageOffset();
	}
	else if (key == '_') {
		// center image
		centerImage();
	}
	else if (key == '=') {
		// fit to screen
		userOffset.x = 0;
		userOffset.y = 0;
		ImRect availableSpace =  ImGui::DockBuilderGetCentralNode(MainDockNodeID)->Rect();
		if (plotCanvas.sourceController.isLandscape) {
			zoomLevel = availableSpace.GetWidth() / plotCanvas.canvasWidth;
		}
		else {
			zoomLevel = availableSpace.GetHeight() / plotCanvas.canvasHeight;
		}
		centerImage();
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
		userOffset.x += x - lastDraggedPos.x;
		userOffset.y += y - lastDraggedPos.y;
	}

	// Update last drag location
	lastDraggedPos.x = x;
	lastDraggedPos.y = y;
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button) {
	ImGuiIO& io = ImGui::GetIO();
	if (x < ofGetWindowWidth() && x > 0 && !io.WantCaptureMouse) {
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
	if (!ImGui::GetIO().WantCaptureMouse) {
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
void ofApp::windowResized(int w, int h) {

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg) {

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo) {
	if (dragInfo.files.size() > 0) {
		for (int i = 0; i < dragInfo.files.size(); i++) {
			if (std::find(plotCanvas.sourceController.img_ext.begin(), plotCanvas.sourceController.img_ext.end(), ofx2d::to_lower(dragInfo.files[i].substr(dragInfo.files[i].find_last_of(".") + 1))) != plotCanvas.sourceController.img_ext.end())
			{
				plotCanvas.sourceController.addImage(ofFile(dragInfo.files[i]));
				resetZoom();
			}
			else if (std::find(plotCanvas.sourceController.vid_ext.begin(), plotCanvas.sourceController.vid_ext.end(), ofx2d::to_lower(dragInfo.files[i].substr(dragInfo.files[i].find_last_of(".") + 1))) != plotCanvas.sourceController.vid_ext.end())
			{
				plotCanvas.sourceController.addVideo(ofFile(dragInfo.files[i]));
				resetZoom();
			}
			else {
				ofLog(OF_LOG_ERROR) << "No support for file format " << ofx2d::to_lower(dragInfo.files[i].substr(dragInfo.files[i].find_last_of(".") + 1));
			}
		}
	}
}
