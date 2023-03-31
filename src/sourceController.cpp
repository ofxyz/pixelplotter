#include "sourceController.h"

void SourceController::renderImGuiSettings() {
	if (!sourceNames.empty())
	{
		if (ofxImGui::VectorCombo("##Source Image", &currentSourceIndex, sourceNames))
		{
			loadSourceIndex();
		}
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
}

void SourceController::loadSettings(ofxXmlSettings settings) {

}

ofxXmlSettings SourceController::getSettings() {
	ofxXmlSettings settings;
	return settings;
}

void SourceController::update() {

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
	else { // Static Image
		for (const auto& filter : iF.v_ImageFilters) {
			if (filter->isFresh()) {
				pix = original.getPixels();
				prepImg();
				continue;
			}
		}
	}

	frameBuffer.update();
}

void SourceController::setup() {
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
	sourceNames.insert(sourceNames.end(), imgFileNames.begin(), imgFileNames.end());
}

void SourceController::loadSourceIndex() {
	if (currentSourceIndex == 0) {
		bUseVideoDevice = true;
		for (vector<ofVideoDevice>::iterator it = videoDevices.begin(); it != videoDevices.end(); ++it) {
			if (it->deviceName == sourceNames[currentSourceIndex]) {
				videoGrabber.close();
				videoGrabber.setDeviceID(it->id);
				videoGrabber.initGrabber(camWidth, camHeight);
				pix = videoGrabber.getPixels();
				prepImg();
				isFresh = true;
				if (bUseVideo) {
					videoPlayer.stop();
					videoPlayer.close();
				}
				return;
			}
		}
	}
	else if (currentSourceIndex > (videoDevices.size() + videoFiles.size()) - 1) {
		loadImage(imgFiles[currentSourceIndex - videoDevices.size() - videoFiles.size()].getAbsolutePath());
	}
	else if (currentSourceIndex > videoDevices.size() - 1) {
		loadVideo(videoFiles[currentSourceIndex - videoDevices.size()].getAbsolutePath());
	}
}

int SourceController::getSourceCount() {
	return (videoDeviceNames.size() + videoFileNames.size() + imgFileNames.size());
};

void SourceController::addImage(ofFile file) {
	imgFiles.push_back(file);
	imgFileNames.push_back(file.getFileName());
	buildSourceNames();
	currentSourceIndex = sourceNames.size()-1;
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
	//imageLoader.loadFromDisk(original, filepath);
	original.loadImage(filepath);
	original.setImageType(OF_IMAGE_COLOR);

	pix = original.getPixels();

	frameBuffer.setup(pix);

	std::string base_filename = filepath.substr(filepath.find_last_of("/\\") + 1);
	src_name = base_filename.substr(0, base_filename.find_last_of('.'));

	(pix.getWidth() > pix.getHeight()) ? isLandscape = true : isLandscape = false;
	(isLandscape) ? imgRatio = pix.getHeight() / pix.getWidth() : imgRatio = pix.getWidth() / pix.getHeight();
	isFresh = true;

	bUseVideo = false;
	bUseVideoDevice = false;
	videoPlayer.stop();
	videoPlayer.close();
	prepImg();
}

void SourceController::loadVideo(string& filepath) {
	bUseVideo = true;
	bUseVideoDevice = false;

	videoPlayer.load(filepath);
	videoPlayer.setLoopState(OF_LOOP_NORMAL);
	videoPlayer.setUseTexture(false);
	videoPlayer.play();

	std::string base_filename = filepath.substr(filepath.find_last_of("/\\") + 1);
	src_name = base_filename.substr(0, base_filename.find_last_of('.'));

	(videoPlayer.getWidth() > videoPlayer.getHeight()) ? isLandscape = true : isLandscape = false;
	(isLandscape) ? imgRatio = videoPlayer.getHeight() / videoPlayer.getWidth() : imgRatio = videoPlayer.getWidth() / videoPlayer.getHeight();
	isFresh = true;

}

void SourceController::prepImg() {

	static ofImage updatedFrame;
	if (updatedFrame.isAllocated() == false)
		updatedFrame.allocate(pix.getWidth(), pix.getHeight(), OF_IMAGE_COLOR);

	for (const auto& filter : iF.v_ImageFilters) {
		updatedFrame.setFromPixels(pix);
		filter->apply(&updatedFrame);
		pix = updatedFrame.getPixels();
	}
	frameBuffer.addFrame(pix);
}
