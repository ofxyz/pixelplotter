#include "ofApp.h"

void ofApp::gui_showMain() {
	gui.begin();
	{
		if (show_main_window)
		{
			ImGui::SetNextWindowSize(ofVec2f(gui_width, ofGetHeight()));
			ImGui::SetNextWindowPos(ofVec2f(ofGetWidth() - gui_width, 0));
			ImGui::Begin("Pixel Plotter", &show_main_window, ImGuiWindowFlags_NoDecoration);

			if (showZoom) {
				if (ImGui::Button("Hide Zoom"))
				{
					showZoom = false;
				}
			}
			else {
				if (ImGui::Button("Show Zoom"))
				{
					showZoom = true;
				}
			}

			ImGui::SameLine();
			if (ImGui::Button("Export Vector")) { saveVector = true; }

			if (ImGui::CollapsingHeader("Source"))
			{
				if (!sourceNames.empty())
				{
					if (ofxImGui::VectorCombo("##Source Image", &currentSourceIndex, sourceNames))
					{
						gui_loadSourceIndex();
					}
				}

				ImGui::SameLine();
				if (showImage) {
					if (ImGui::Button("Hide Source"))
					{
						showImage = false;
					}
				}
				else {
					if (ImGui::Button("Show Source"))
					{
						showImage = true;
					}
				}
			}

			if (ImGui::CollapsingHeader("Style Options"))
			{
				/*
				if (ImGui::Button("Quick Save"))
				{
					string savePath = "presets\/quicksave.xml";
					saveSettings(savePath);
				}
				ImGui::SameLine();
				if (ImGui::Button("Quick Load"))
				{
					string savePath = "presets\/quicksave.xml";
					loadSettings(savePath);
				}
				*/

				if (ofxImGui::VectorCombo("##Presets", &currentPresetIndex, presetFileNames))
				{
					loadSettings(presetFiles[currentPresetIndex].getAbsolutePath());
				}

				if (presetFileNames.size() > 0) {
					ImGui::SameLine();
					if (ImGui::Button("Delete Preset"))
					{
						presetFiles[currentPresetIndex].remove();
						gui_loadPresets();
					}
				}

				if (bSavePreset) {
					ImGui::InputText("##presetname", presetSaveName, IM_ARRAYSIZE(presetSaveName));
					ImGui::SameLine();
				}
				if (ImGui::Button("Save Preset"))
				{
					if (bSavePreset) {
						string savePath = "presets\/" + string(presetSaveName) + ".xml";
						saveSettings(savePath);
						gui_loadPresets();
						currentPresetIndex = getIndex(presetFileNames, string(presetSaveName), 0);
						bSavePreset = false;
					}
					else {
						if (presetFileNames.size() > 0) {
							strcpy(presetSaveName, presetFileNames[currentPresetIndex].c_str());
						}
						bSavePreset = true;
					}
				}

				ImGui::AlignTextToFramePadding();

				if (ofxImGui::VectorCombo("Plot Style", &currentPlotStyleIndex, ss.v_PlotStyles))
				{
					// Done
				}

				ImGui::PushItemWidth(100);

				ImGui::Text("Tiles"); ImGui::SameLine(75);
				ImGui::DragInt("X ##Tiles", &ss.tilesX, 1, 1, 1200);
				ImGui::SameLine();
				ImGui::DragInt("Y ##Tiles", &ss.tilesY, 1, 1, 1200);

				ImGui::Text("Addon"); ImGui::SameLine(75);
				ImGui::DragFloat("X ##Addon", &ss.addonx, 0.1f, -100.0f, 100.0f, "%.3f");
				ImGui::SameLine();
				ImGui::DragFloat("Y ##Addon", &ss.addony, 0.1f, -100.0f, 100.0f, "%.3f");

				ImGui::Text("Offset"); ImGui::SameLine(75);
				ImGui::DragFloat("Random", &ss.randomOffset, 0.1f, 0.0f, 250.0f, "%.3f%%");

				ImGui::Text("Noise"); ImGui::SameLine(75);
				ImGui::DragFloat("X ##Noise", &ss.noisepercentX, 0.1f, 0.0f, 100.0f, "%.2f%%");
				ImGui::SameLine();
				ImGui::DragFloat("Y ##Noise", &ss.noisepercentY, 0.1f, 0.0f, 100.0f, "%.2f%%");

				ImGui::Text("Every N"); ImGui::SameLine(75);
				ImGui::DragInt("X ##Every N", &ss.everynx, 1, 1, 128);
				ImGui::SameLine();
				ImGui::DragInt("Y ##Every N", &ss.everyny, 1, 1, 128);

				ImGui::PopItemWidth();

				// Pixel Type: line, square, rect, round, oval
				// Use Radio or dropdown
				if (ss.roundPixels) {
					if (ImGui::Button("Square Pixels"))
					{
						ss.roundPixels = false;
					}
				}
				else {
					if (ImGui::Button("Round Pixels"))
					{
						ss.roundPixels = true;
					}
				}
				ImGui::SameLine();
				ImGui::Checkbox("Polka", &ss.polka);
			}// End Style

			if (ImGui::CollapsingHeader("Colours"))
			{
				ImGui::ColorEdit4("Magenta / Red", (float*)&c_magentaRed, ImGuiColorEditFlags_NoInputs);
				ImGui::ColorEdit4("Cyan / Blue", (float*)&c_cyanBlue, ImGuiColorEditFlags_NoInputs);
				ImGui::ColorEdit4("Yellow / Green", (float*)&c_yellowGreen, ImGuiColorEditFlags_NoInputs);
				ImGui::ColorEdit4("Black", (float*)&c_black, ImGuiColorEditFlags_NoInputs);
				ImGui::ColorEdit4("Paper / White", (float*)&c_paper, ImGuiColorEditFlags_NoInputs);
				ImGui::ColorEdit4("Background", (float*)&c_background, ImGuiColorEditFlags_NoInputs);

				if (ImGui::Button("Set RGB"))
				{
					ofApp::gui_setRGB_pressed();
				}
				ImGui::SameLine();
				if (ImGui::Button("Set CMYK"))
				{
					ofApp::gui_setCMYK_pressed();
				}
				ImGui::SameLine();
				if (ImGui::Button("Set Avarage"))
				{
					ofApp::gui_setAvarage_pressed();
				}

				ImGui::Checkbox("Normalise Colours", &ss.normalise);

				if (ofxImGui::VectorCombo("Blend Mode", &currentBlendModeIndex, ss.v_BlendModes))
				{
					ss.currentBlendmode = ss.v_BlendModes[currentBlendModeIndex];
				}

			} // End Colours

			if (pauseRender) {
				if (ImGui::Button("Continue"))
				{
					pauseRender = false;
				}
			}
			else {
				if (ImGui::Button("Pause"))
				{
					pauseRender = true;
				}
			}

			ImGui::SameLine();

			if (pauseRender) {
				ImGui::Text("Paused at %.1f FPS", ImGui::GetIO().Framerate);
			}
			else {
				ImGui::Text("Rendering at %.1f FPS", ImGui::GetIO().Framerate);
			}

			ImGui::End();
		}
	}
	gui.end();
}

//--------------------------------------------------------------
void ofApp::gui_loadPresets() {
	ofDirectory presetDirectory(ofToDataPath("presets", true));
	presetFileNames.clear();
	presetFiles = presetDirectory.getFiles();
	for (int i = 0; i < presetFiles.size(); i++)
	{
		string base_filename = presetFiles[i].getFileName();
		string pname = base_filename.substr(0, base_filename.find_last_of('.'));
		presetFileNames.push_back(pname);
	}
}

//--------------------------------------------------------------
void ofApp::gui_loadSourceIndex() {
	if (currentSourceIndex > (videoDevices.size() + videoFiles.size()) - 1) {
		loadImage(imgFiles[currentSourceIndex - videoDevices.size() - videoFiles.size()].getAbsolutePath());
	}
	else if (currentSourceIndex > videoDevices.size() - 1) {
		loadVideo(videoFiles[currentSourceIndex - videoDevices.size()].getAbsolutePath());
	}
	else {
		bUseVideoDevice = true;
		for (vector<ofVideoDevice>::iterator it = videoDevices.begin(); it != videoDevices.end(); ++it) {
			if (it->deviceName == sourceNames[currentSourceIndex]) {
				videoGrabber.setDeviceID(it->id);
				break;
			}
		}
	}
}

//--------------------------------------------------------------
void ofApp::gui_buildSourceNames() {
	sourceNames.clear();
	sourceNames.insert(sourceNames.end(), videoDeviceNames.begin(), videoDeviceNames.end());
	sourceNames.insert(sourceNames.end(), videoFileNames.begin(), videoFileNames.end());
	sourceNames.insert(sourceNames.end(), imgFileNames.begin(), imgFileNames.end());
}

//--------------------------------------------------------------
void ofApp::gui_setRGB_pressed() {
	c_magentaRed = ofColor(255, 0, 0);
	c_cyanBlue = ofColor(0, 0, 255);
	c_yellowGreen = ofColor(0, 255, 0);
	c_black = ofColor(0, 0, 0);
}

//--------------------------------------------------------------
void ofApp::gui_setCMYK_pressed() {
	c_magentaRed = ofColor(236, 0, 140);
	c_cyanBlue = ofColor(0, 174, 239);
	c_yellowGreen = ofColor(255, 242, 0);
	c_black = ofColor(0, 0, 0);
}

//--------------------------------------------------------------
void ofApp::gui_setAvarage_pressed() {
	swatches = ofxPosterize::getClusterColors(img, 4);
	if (swatches.size() > 3) {
		c_magentaRed = swatches[0];
		c_cyanBlue = swatches[1];
		c_yellowGreen = swatches[2];
		c_black = swatches[3];
	}
	else if (swatches.size() > 2) {
		c_magentaRed = swatches[0];
		c_cyanBlue = swatches[1];
		c_yellowGreen = swatches[2];
	}
	else if (swatches.size() > 1) {
		c_magentaRed = swatches[0];
		c_cyanBlue = swatches[1];
	}
	else if (swatches.size() > 0) {
		c_magentaRed = swatches[0];
	}
}

//--------------------------------------------------------------
void ofApp::gui_setBlendmode() {
	if (ss.currentBlendmode == "OF_BLENDMODE_ALPHA") {
		ofEnableBlendMode(OF_BLENDMODE_ALPHA);
	}
	else if (ss.currentBlendmode == "OF_BLENDMODE_ADD") {
		ofEnableBlendMode(OF_BLENDMODE_ADD);
	}
	else if (ss.currentBlendmode == "OF_BLENDMODE_SUBTRACT") {
		ofEnableBlendMode(OF_BLENDMODE_SUBTRACT);
	}
	else if (ss.currentBlendmode == "OF_BLENDMODE_MULTIPLY") {
		ofEnableBlendMode(OF_BLENDMODE_MULTIPLY);
	}
	else if (ss.currentBlendmode == "OF_BLENDMODE_SCREEN") {
		ofEnableBlendMode(OF_BLENDMODE_SCREEN);
	}
	else {
		ofEnableBlendMode(OF_BLENDMODE_DISABLED);
	}
}