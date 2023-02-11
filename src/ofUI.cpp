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

			string sFilterCount = "Draw Filters (" + ofToString(v_DrawFilters.size()) + ")###DrawFiltersHolder";
			if (ImGui::CollapsingHeader(sFilterCount.c_str()))
			{
				ImGui::PushStyleColor(ImGuiCol_Header, (ImVec4)ImColor::HSV(0, 0, 0.2));
				ImGui::PushStyleColor(ImGuiCol_HeaderActive, (ImVec4)ImColor::HSV(0, 0, 0.4));
				ImGui::PushStyleColor(ImGuiCol_HeaderHovered, (ImVec4)ImColor::HSV(0, 0, 0.7));

				ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(0, 0, 0.2));
				ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(0, 0, 0.2));
				ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(0, 0, 0.7));

				ImGui::PushStyleColor(ImGuiCol_FrameBg, (ImVec4)ImColor::HSV(0, 0, 0.2));
				ImGui::PushStyleColor(ImGuiCol_FrameBgActive, (ImVec4)ImColor::HSV(0, 0, 0.4));
				ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, (ImVec4)ImColor::HSV(0, 0, 0.5));

				ImGui::PushStyleColor(ImGuiCol_CheckMark, (ImVec4)ImColor::HSV(0, 0, 0.8));

				cleanFilters = false;
				for (int i = 0; i < v_DrawFilters.size(); i++) {
					ImGui::PushID(i);
					if (v_DrawFilters[i]->active) {
						ImGui::Indent();
						v_DrawFilters[i]->renderImGuiSettings();
						ImGui::Unindent();
					}
					else {
						cleanFilters = true;
					}
					ImGui::PopID();
				}

				ImGui::PopStyleColor(10);

				if (ofxImGui::VectorCombo("##Draw Filter Selector", &currentDrawFilterIndex, v_DrawFilterNames))
				{
					addDrawFilter(currentDrawFilterIndex);
					currentDrawFilterIndex = 0;
				}
			}// End Draw Filters

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
			} // End Colours

			if (ImGui::Button("Export Vector")) { saveVector = true; }

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
	ofDisableBlendMode();
}