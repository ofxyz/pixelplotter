#include "ofApp.h"
#include "vc_SoundDevice.h"
#include "imgui_internal.h"
#include "ImHelpers.h"
#include "imgui_stdlib.h"
#include "IconsFontAwesome5.h"
#include "ImGui_Widget_Tooltip.h"

void soundDeviceController::renderImGuiSettings() {
	if (!soundDeviceNames.empty())
	{
		if (ofxImGui::VectorCombo("Sound Devices", &currentSoundDeviceIndex, soundDeviceNames))
		{
			loadSoundDeviceIndex();
		}
		ImGui::SameLine(); ImGui::HelpMarker("A sound source can be used as a modulator.");
	}
}

void soundDeviceController::loadSettings(ofJson& settings) {

}

ofJson soundDeviceController::getSettings() {
	ofJson settings;
	return settings;
}

void soundDeviceController::loadSoundDeviceIndex() {
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

void soundDeviceController::setup() {
	pixelplotter = (ofApp*)ofGetAppPtr();

	left.assign(bufferSize, 0.0);
	right.assign(bufferSize, 0.0);
	volHistory.assign(400, 0.0);

	// you can also get devices for an specific api
	// auto devices = soundStream.getDevicesByApi(ofSoundDevice::Api::PULSE);
	soundDevices = soundStream.getDeviceList(ofSoundDevice::MS_DS);
	//soundDevices = soundStream.getDeviceList();
	for (vector<ofSoundDevice>::iterator it = soundDevices.begin(); it != soundDevices.end(); ++it) {
		soundDeviceNames.push_back(it->name);
	}

	//loadSoundDeviceIndex();

	//virtual bool setInDevice(const ofSoundDevice & device);
	//virtual bool setOutDevice(const ofSoundDevice & device);
	//virtual bool setApi(ofSoundDevice::Api api);


	// settings.device = devices[0];

	// or get the default device for an specific api:
	// settings.api = ofSoundDevice::Api::PULSE;

	// or by name
	//auto devices = soundStream.getMatchingDevices("default");

}

void soundDeviceController::update() {
	// TODO: Only do this when sound is active

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
void soundDeviceController::audioIn(ofSoundBuffer& input) {

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
	//ofLog(OF_LOG_ERROR) << curVol << endl;
	bufferCounter++;
}
