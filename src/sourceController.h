#pragma once
#include "ofMain.h"
#include "ofxXmlSettings.h"
#include "ofxImGui.h"
#include "frameBuffer.h"
#include "imageFilter_Controller.h"
#include "generator_Controller.h"

class ofApp;

class SourceController {
public:
	ofApp* pixelplotter;

	void setup(ofApp* app);
	void update();

	void renderImGuiSettings();
	void loadSettings(ofxXmlSettings settings);
	ofxXmlSettings getSettings();

	void addImage(ofFile file);
	void addVideo(ofFile file);
	void loadImage(string& filepath);
	void loadVideo(string& filepath);
	void loadGenerator(string& name);

	void buildSourceNames();

	FrameBuffer frameBuffer;
	ofVideoPlayer videoPlayer;
	ofVideoGrabber videoGrabber;

	ImageFilterController iF;
	GeneratorController gC;

	ofImage original;
	ofPixels pix; // Use ofImage and turn textures off // Turn on when needed?

	std::string src_name = "PixelPlotted";
	bool showSource = false;
	bool bUseVideo = false;
	bool bUseGenerator = false;
	bool bUseVideoDevice = false;
	bool isResized = false;
	bool isLandscape = false;
	bool mirrorV = false;
	bool mirrorH = false;
	bool loadImageNextFrame = false;
	int currentVideoFrame = 1;
	int currentSourceIndex = 0;

	int width = 640;
	int height = 480;
	int camWidth = 640;
	int camHeight = 480;
	float imgRatio = 1;

	ofFbo sourceCanvas;
	
	std::vector<std::string> sourceNames;

	std::vector<std::string> videoDeviceNames;
	std::vector<ofVideoDevice> videoDevices;

	std::vector<std::string> videoFileNames;
	std::vector<ofFile> videoFiles;

	std::vector<std::string> imgFileNames;
	std::vector<ofFile> imgFiles;

	void loadSourceIndex();
	void prepImg();

	int getSourceCount();

	std::vector<std::string> img_ext{
				"jpg",
				"jpeg",
				"png",
				"bmp",
				"gif",
				"webp",
				"tif",
				"tiff",
				"psd"
	};

	std::vector<std::string> vid_ext{
			"mpeg",
			"mpe",
			"m1v",
			"m2v",
			"mpv2",
			"mp2v",
			"vob",
			"evo",
			"mod",
			"mpeg-2",
			"mpeg2",
			"mpg",
			"mp4",
			"mp4v",
			"hdmov",
			"mkv",
			"m4v",
			"mov",
			"wmv",
			"avi",
			"divx",
			"amv",
			"ogm",
			"ogv",
			"wmv",
			"asf"
			"rm",
			"rmvb",
			"dv",
			"mxf",
			"dav"
	};

};
