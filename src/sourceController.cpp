#include "sourceController.h"

void SourceController::update() {
	if (bUseVideoDevice) {
		videoGrabber.update();
		if (videoGrabber.isFrameNew()) {
			pix = videoGrabber.getPixels();
			prepSource();
		}
	}
	else if (bUseVideo) {
		videoPlayer.update();
		if (videoPlayer.isFrameNew()) {
			pix = videoPlayer.getPixels();
			prepSource();
		}
	}
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

void SourceController::loadImage(string& filepath) {
	ofLoadImage(pix, filepath);
	pix.setImageType(OF_IMAGE_COLOR);

	std::string base_filename = filepath.substr(filepath.find_last_of("/\\") + 1);
	src_name = base_filename.substr(0, base_filename.find_last_of('.'));

	prepSource();
	bUseVideo = false;
	bUseVideoDevice = false;
	videoPlayer.stop();
	videoPlayer.close();
}

void SourceController::loadVideo(string& filepath) {
	bUseVideo = true;
	bUseVideoDevice = false;

	videoPlayer.load(filepath);
	videoPlayer.setLoopState(OF_LOOP_NORMAL);
	videoPlayer.play();

	std::string base_filename = filepath.substr(filepath.find_last_of("/\\") + 1);
	src_name = base_filename.substr(0, base_filename.find_last_of('.'));
}

void SourceController::prepSource() {
	img.getPixelsRef() = pix;
	img.update();

	// Keep pixelated when drawing ...
	img.getTextureReference().setTextureMinMagFilter(GL_NEAREST, GL_NEAREST);

	(img.getWidth() > img.getHeight()) ? isLandscape = true : isLandscape = false;
	(isLandscape) ? imgRatio = img.getHeight() / img.getWidth() : imgRatio = img.getWidth() / img.getHeight();

	prepImg();

}

void SourceController::prepImg() {
	img.mirror(mirrorV, mirrorH);
	isFresh = true;
}

ofxXmlSettings SourceController::getSettings() {
	ofxXmlSettings settings;
	return settings;
}

void SourceController::loadSettings(ofxXmlSettings settings) {
	return;
}

void SourceController::renderImGuiSettings() {
	if (ImGui::CollapsingHeader("Source")) {
		ImGui::AlignTextToFramePadding();
	}
}
