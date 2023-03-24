#pragma once
#include "ofMain.h"
#include "ofxXmlSettings.h"
#include "ofxImGui.h"
#include "frameBuffer.h"
#include "imageFilter_Controller.h"

class SourceController {
public:
	void setup();
	void update();

	void loadImage(string& filepath);
	void loadVideo(string& filepath);

	void buildSourceNames();

	FrameBuffer frameBuffer;
	ofVideoPlayer videoPlayer;
	ofVideoGrabber videoGrabber;

	ImageFilterController iF;
	
	ofPixels pix; // Use ofImage and turn textures off // Turn on when needed.

	std::string src_name = "PixelPlotted";
	bool bUseVideo = false;
	bool bUseVideoDevice = false;
	bool isFresh = false;
	bool isLandscape = false;
	bool mirrorV = false;
	bool mirrorH = false;
	int currentVideoFrame = 1;
	int currentSourceIndex = 0;

	int width = 0;
	int height = 0;
	int camWidth = 640;
	int camHeight = 480;
	float imgRatio = 1;

	std::vector<std::string> sourceNames;

	std::vector<std::string> videoDeviceNames;
	std::vector<ofVideoDevice> videoDevices;

	std::vector<std::string> videoFileNames;
	std::vector<ofFile> videoFiles;

	std::vector<std::string> imgFileNames;
	std::vector<ofFile> imgFiles;

	void loadSourceIndex();
	void prepImg();
	 
	ofxXmlSettings getSettings();
	void loadSettings(ofxXmlSettings settings);
	void renderImGuiSettings();

	int getSourceCount();

	std::vector<std::string> img_ext{
				"jpg",
				"jpeg",
				"png",
				"bmp",
				"gif"
	};

	std::vector<std::string> vid_ext{
			"mpeg",
			"mpeg-2",
			"mpeg2",
			"mpg",
			"mp4",
			"mkv",
			"m4v",
			"mov",
			"wmv",
			"avi"
	};

};
