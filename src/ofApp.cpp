#include "ofApp.h"
#include "ofx2d.h"

void ofApp::setup() {
	/* Always disable rectangular textures by default */
	ofDisableArbTex();

	c_background = ofColor(50, 50, 50, 255);

	//ofSetLogLevel(OF_LOG_ERROR);
	//ofSetLogLevel(OF_LOG_VERBOSE);
	ofSetVerticalSync(vSync);
	//ofLog(OF_LOG_VERBOSE) << ofFbo::checkGLSupport();

	ofSetFrameRate(_targetFps); // TODO: Add to settings
	ofEnableAlphaBlending();
	ofDisableAntiAliasing(); // TODO: Add to settings

	//ofHideCursor();
	ofAddListener(ofEvents().mouseScrolled, this, &ofApp::mouseScrolled);

	//ofSetBackgroundAuto(false);

	ofGui.setup();
	soundManager.setup();
	plotCanvas.setup();

	userOffset.x = 0;
	userOffset.y = 0;

	ofGui.loadPresetDir();
}

//--------------------------------------------------------------
void ofApp::exit() {
	ofRemoveListener(ofEvents().mouseScrolled, this, &ofApp::mouseScrolled);
}

//--------------------------------------------------------------
void ofApp::update() {
	ofGui.update();

	if (!ofGui.renderingPaused()) {
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

	ofGui.draw();
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
	ImVec4 availableSpace = ofGui.availableSpace();
	userOffset.x = (availableSpace.x - (plotCanvas.canvasWidth * zoomLevel)) * 0.5;
	userOffset.y = (availableSpace.y - (plotCanvas.canvasHeight * zoomLevel)) * 0.5;
	userOffset.y += availableSpace.w;
}

//-------------------------------------------------------------
void ofApp::keyPressed(int key) {
	if (ImGui::GetIO().WantCaptureKeyboard) return;

	if (key == 'g' || key == 'G') {
		ofGui.setGuiVisible(!ofGui.guiVisible());
	}
	else if (key == 'p' || key == 'P')
	{
		ofGui.setRenderingPaused(!ofGui.renderingPaused());
	}
	bool stsus = ofGui.guiVisible();
	bool x = false;
	// TODO:
	// -------
	// Zoom level and center image needs to take `ofGui._bGuiVisible` into account

	if (key == '-') {
		zoomLevel -= 0.1;
		//resetImageOffset();
	}
	else if (key == '+') {
		zoomLevel += 0.1;
		//resetImageOffset();
	}
	else if (key == '1') {
		zoomLevel = 1;
		centerImage();
	}
	else if (key == '_') {
		// center image
		centerImage();
	}
	else if (key == '=') {
		// fit to screen
		ImVec4 availableSpace =  ofGui.availableSpace();
		if (plotCanvas.sourceController.isLandscape) {
			zoomLevel = availableSpace.x / plotCanvas.canvasWidth;
		}
		else {
			zoomLevel = availableSpace.y / plotCanvas.canvasHeight;
		}
		centerImage();
	}
	else if (key == 'p') {
		plotCanvas.saveVector = true;
	}
	else if (key == 'x') {
		ofGui.setRenderingPaused(!ofGui.renderingPaused());
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
	if (ImGui::GetIO().WantCaptureMouse) return;

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
	if (ImGui::GetIO().WantCaptureMouse) return;

	if (x < ofGetWindowWidth() && x > 0) {
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
	if (ImGui::GetIO().WantCaptureMouse) return;

	glm::vec3 position = plotCanvas.cam.getPosition();
	if (zoomLevel > 1) {
		zoomLevel += (mouse.scrollY * 0.1);
	}
	else {
		zoomLevel += (mouse.scrollY * 0.01);
	}
	if (zoomLevel < 0.02) zoomLevel = 0.02;
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
