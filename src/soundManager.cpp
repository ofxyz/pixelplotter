#include "soundManager.h"
#include "ofApp.h";

void SoundManager::renderImGuiSettings() {
	if (!soundDeviceNames.empty())
	{
		if (ofxImGui::VectorCombo("##Sound Devices", &currentSoundDeviceIndex, soundDeviceNames))
		{
			loadSoundDeviceIndex();
		}
	}
}

void SoundManager::loadSettings(ofxXmlSettings settings) {

}

ofxXmlSettings SoundManager::getSettings() {
	ofxXmlSettings settings;
	return settings;
}

void SoundManager::loadSoundDeviceIndex() {
	ofSoundStreamSettings settings;

	if (!soundDevices.empty()) {
		settings.setInDevice(soundDevices[currentSoundDeviceIndex]);
	}

	// Pass the audioIn function from ofApp ...
	settings.setInListener(pixelplotter);

	settings.sampleRate = 44100;
	settings.numOutputChannels = 0;
	settings.numInputChannels = 2;
	settings.bufferSize = bufferSize;
	soundStream.setup(settings);
	soundStream.setOutput(pixelplotter);
}

void SoundManager::setup(ofApp* app) {
	pixelplotter = app;

	left.assign(bufferSize, 0.0);
	right.assign(bufferSize, 0.0);
	volHistory.assign(400, 0.0);

	soundDevices = soundStream.getDeviceList(ofSoundDevice::MS_DS);
	//soundDevices = soundStream.getDeviceList();
	for (vector<ofSoundDevice>::iterator it = soundDevices.begin(); it != soundDevices.end(); ++it) {
		soundDeviceNames.push_back(it->name);
	}

	//loadSoundDeviceIndex();

	//virtual bool setInDevice(const ofSoundDevice & device);
	//virtual bool setOutDevice(const ofSoundDevice & device);
	//virtual bool setApi(ofSoundDevice::Api api);

	// you can also get devices for an specific api
	// auto devices = soundStream.getDevicesByApi(ofSoundDevice::Api::PULSE);
	// settings.device = devices[0];

	// or get the default device for an specific api:
	// settings.api = ofSoundDevice::Api::PULSE;

	// or by name
	//auto devices = soundStream.getMatchingDevices("default");

	

	
}

void SoundManager::update() {
	//lets scale the vol up to a 0-1 range 
	scaledVol = ofMap(smoothedVol, 0.0, 0.17, 0.0, 1.0, true);

	//lets record the volume into an array
	volHistory.push_back(scaledVol);

	//if we are bigger the the size we want to record - lets drop the oldest value
	if (volHistory.size() >= 400) {
		volHistory.erase(volHistory.begin(), volHistory.begin() + 1);
	}
}

//--------------------------------------------------------------
void SoundManager::audioIn(ofSoundBuffer& input) {

	float curVol = 0.0;

	// samples are "interleaved"
	int numCounted = 0;

	//lets go through each sample and calculate the root mean square which is a rough way to calculate volume	
	for (size_t i = 0; i < input.getNumFrames(); i++) {
		left[i] = input[i * 2] * 0.5;
		right[i] = input[i * 2 + 1] * 0.5;

		curVol += left[i] * left[i];
		curVol += right[i] * right[i];
		numCounted += 2;
	}

	//this is how we get the mean of rms :) 
	curVol /= (float)numCounted;

	// this is how we get the root of rms :) 
	curVol = sqrt(curVol);

	smoothedVol *= 0.93;
	smoothedVol += 0.07 * curVol;
	ofLog() << 
	bufferCounter++;
}