#pragma once
#include "ofMain.h"
#include "ofxXmlSettings.h"
#include "ofxImGui.h"

class ofApp;

class SoundManager {
public:
	ofApp* pixelplotter;
	ofSoundStream soundStream;

	void audioIn(ofSoundBuffer& input);
	void loadSoundDeviceIndex();

	vector <float> left;
	vector <float> right;
	vector <float> volHistory;

	std::vector<std::string> soundDeviceNames;
	std::vector<ofSoundDevice> soundDevices;

	int currentSoundDeviceIndex = 1;
	int bufferSize = 256;
	int bufferCounter = 0;
	int drawCounter = 0;

	float smoothedVol = 0.0;
	float scaledVol = 0.0;

	void setup(ofApp* app);
	void update();

	void renderImGuiSettings();
	void loadSettings(ofxXmlSettings settings);
	ofxXmlSettings getSettings();

};
