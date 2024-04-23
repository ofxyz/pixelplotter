#include "ofApp.h"
#include "ofx2d.h"
#include "source_Controller.h"
#include "ofxImGui.h"
#include "imgui_internal.h"
#include "ImHelpers.h"
#include "imgui_stdlib.h"

void SourceController::renderImGuiSettings()
{
	if (!sourceNames.empty())
	{
		if (ofxImGui::VectorCombo("##Sources", &currentSourceIndex, sourceNames))
		{
			loadSourceIndex();
			loadImageNextFrame = true;
		}

		ImGui::SameLine();
		if (showSource) {
			if (ImGui::Button("Hide Source"))
			{
				showSource = false;
			}
		}
		else {
			if (ImGui::Button("Show Source"))
			{
				showSource = true;
			}
		}

		ImGui::Checkbox("Update Canvas Dimension When Loading Source", &bUpdateCanvasOnSourceLoad);
	}
}

void SourceController::loadSettings(ofJson settings)
{
	std::string sourceString = settings.value("source", std::string());
	currentSourceIndex = ofx2d::getIndex(sourceNames, sourceString, currentSourceIndex);
	bUpdateCanvasOnSourceLoad = settings.value("bUpdateCanvasOnSourceLoad", bUpdateCanvasOnSourceLoad);
	loadSourceIndex();
}

ofJson SourceController::getSettings()
{
	ofJson settings;
	settings["source"] = sourceNames[currentSourceIndex];
	settings["bUpdateCanvasOnSourceLoad"] = bUpdateCanvasOnSourceLoad;
	return settings;
}

void SourceController::update()
{
	if (loadImageNextFrame)
	{
		loadSourceIndex();
		loadImageNextFrame = false;
	}

	if (bUseVideoDevice) {
		videoGrabber.update();
		if (videoGrabber.isFrameNew()) {
			pix = videoGrabber.getPixels();
			prepImg();
		}
	}
	else if (bUseVideo) {
		videoPlayer.update();
		if (videoPlayer.isFrameNew()) {
			pix = videoPlayer.getPixels();
			prepImg();
		}
	}
	else if (bUseGenerator) {
		gC.v_Objects[currentGeneratorIndex]->update();
		if (gC.isFresh()) {
			gC.v_Objects[currentGeneratorIndex]->m_fbo.readToPixels(pix);
			prepImg();
			gC.setFresh(false);
		}
	}
	else { // Static Image
		if (iF.isFresh()) {
			pix = original.getPixels();
			prepImg();
			iF.setFresh(false);
		}
	}

	iF.update();
	frameBuffer.update();
}

void SourceController::setup() {
	pixelplotter = (ofApp*)ofGetAppPtr();

	videoDevices = videoGrabber.listDevices();
	for (vector<ofVideoDevice>::iterator it = videoDevices.begin(); it != videoDevices.end(); ++it) {
		videoDeviceNames.push_back(it->deviceName);
	}

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

	currentSourceIndex = ofRandom(videoDeviceNames.size() + videoFileNames.size(), sourceNames.size() - 1);

	loadSourceIndex();
}

void SourceController::buildSourceNames() {
	sourceNames.clear();
	sourceNames.insert(sourceNames.end(), videoDeviceNames.begin(), videoDeviceNames.end());
	sourceNames.insert(sourceNames.end(), videoFileNames.begin(), videoFileNames.end());
	sourceNames.insert(sourceNames.end(), gC.v_objectNames.begin(), gC.v_objectNames.end());
	sourceNames.insert(sourceNames.end(), imgFileNames.begin(), imgFileNames.end());
}

void SourceController::loadSourceIndex() {
	if (currentSourceIndex < videoDevices.size()) {
		bUseVideoDevice = true;
		for (vector<ofVideoDevice>::iterator it = videoDevices.begin(); it != videoDevices.end(); ++it) {
			if (it->deviceName == sourceNames[currentSourceIndex]) {
				if (bUseVideo) {
					videoPlayer.stop();
					videoPlayer.close();
				}
				videoGrabber.close();
				videoGrabber.setDeviceID(it->id);
				// videoGrabber.setDesiredFrameRate(pixelplotter->_targetFps);
				videoGrabber.setup(camWidth, camHeight);
				if(bUpdateCanvasOnSourceLoad) pixelplotter->plotCanvas.setDimensions(camWidth, camHeight);
				pix = videoGrabber.getPixels();
				prepImg();
				return;
			}
		}
	}
	else if (currentSourceIndex < videoDevices.size() + videoFiles.size()) {
		loadVideo(videoFiles[currentSourceIndex - videoDevices.size()].getAbsolutePath());
	}
	else if (currentSourceIndex < videoDevices.size() + videoFiles.size() + gC.v_objectNames.size()) {
		loadGenerator(gC.v_objectNames[currentSourceIndex - videoDevices.size() - videoFiles.size()]);
	}
	else {
		loadImage(imgFiles[currentSourceIndex - videoDevices.size() - videoFiles.size() - gC.v_objectNames.size()].getAbsolutePath());
	}

	isResized = true;
}

int SourceController::getSourceCount() {
	return (int)(videoDeviceNames.size() + videoFileNames.size() + gC.v_objectNames.size() + imgFileNames.size());
};

void SourceController::addImage(ofFile file) {
	imgFiles.push_back(file);
	imgFileNames.push_back(file.getFileName());
	buildSourceNames();
	currentSourceIndex = sourceNames.size() - 1;
	loadSourceIndex();
}

void SourceController::addVideo(ofFile file) {
	videoFiles.push_back(file);
	videoFileNames.push_back(file.getFileName());
	buildSourceNames();
	currentSourceIndex = videoDeviceNames.size() + videoFileNames.size() - 1;
	loadSourceIndex();
}

void SourceController::loadImage(string& filepath) {
	original.load(filepath);
	original.setImageType(OF_IMAGE_COLOR_ALPHA);
	original.getTexture().setTextureMinMagFilter(GL_NEAREST, GL_NEAREST);

	pix = original.getPixels();

	frameBuffer.setup(pix);

	std::string base_filename = filepath.substr(filepath.find_last_of("/\\") + 1);
	src_name = base_filename.substr(0, base_filename.find_last_of('.'));

	(pix.getWidth() > pix.getHeight()) ? isLandscape = true : isLandscape = false;
	(isLandscape) ? imgRatio = pix.getHeight() / pix.getWidth() : imgRatio = pix.getWidth() / pix.getHeight();

	bUseVideo = false;
	bUseVideoDevice = false;
	bUseGenerator = false;
	videoPlayer.stop();
	videoPlayer.close();
	if (bUpdateCanvasOnSourceLoad) pixelplotter->plotCanvas.setDimensions(pix.getWidth(), pix.getHeight());
	prepImg();
}

void SourceController::loadVideo(string& filepath) {
	bUseVideo = true;
	bUseVideoDevice = false;
	bUseGenerator = false;

	videoPlayer.load(filepath);
	//videoPlayer.setUseTexture(false);
	currentVideoFrame = 0;
	videoPlayer.setLoopState(OF_LOOP_NORMAL);
	videoPlayer.play();

	std::string base_filename = filepath.substr(filepath.find_last_of("/\\") + 1);
	src_name = base_filename.substr(0, base_filename.find_last_of('.'));

	(videoPlayer.getWidth() > videoPlayer.getHeight()) ? isLandscape = true : isLandscape = false;
	(isLandscape) ? imgRatio = videoPlayer.getHeight() / videoPlayer.getWidth() : imgRatio = videoPlayer.getWidth() / videoPlayer.getHeight();

	if (bUpdateCanvasOnSourceLoad) pixelplotter->plotCanvas.setDimensions(videoPlayer.getWidth(), videoPlayer.getHeight());
}

void SourceController::loadGenerator(string & name) {
	// This needs it's own function
	bUseVideo = false;
	bUseVideoDevice = false;
	bUseGenerator = true;
	videoPlayer.stop();
	videoPlayer.close();

	src_name = name;

	currentGeneratorIndex = ofx2d::getIndex(gC.v_objectNames, name, -1);

	if (currentGeneratorIndex >= 0) {
		gC.v_Objects[currentGeneratorIndex]->setup(frameBuffer.getWidth(), frameBuffer.getHeight());
	}

	gC.v_Objects[currentGeneratorIndex]->m_fbo.readToPixels(pix);
	prepImg();
	gC.setFresh(false);
}

void SourceController::prepImg() {
	static ofImage updatedFrame;
	if (updatedFrame.isAllocated() == false) {
		updatedFrame.allocate(pix.getWidth(), pix.getHeight(), OF_IMAGE_COLOR_ALPHA);
		updatedFrame.getTexture().setTextureMinMagFilter(GL_NEAREST, GL_NEAREST);
	}

	for (const auto& filter : iF.v_Objects) {
		updatedFrame.setFromPixels(pix);
		filter->apply(&updatedFrame);
		pix = updatedFrame.getPixels();
	}

	iF.setFresh(false);
	gC.setFresh(false);

	frameBuffer.addFrame(pix);
}
