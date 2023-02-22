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

			/* Save and load presets ... */

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
					currentPresetIndex = x2d.getIndex(presetFileNames, string(presetSaveName), 0);
					bSavePreset = false;
				}
				else {
					if (presetFileNames.size() > 0) {
						strcpy(presetSaveName, presetFileNames[currentPresetIndex].c_str());
					}
					bSavePreset = true;
				}
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

			if (ImGui::CollapsingHeader("Application Settings"))
			{
				ImGui::ColorEdit4("Canvas Colour", (float*)&c_paper, ImGuiColorEditFlags_NoInputs);

			} // End Application Settings

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