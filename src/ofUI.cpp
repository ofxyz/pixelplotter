#include "ofApp.h"

void ofApp::gui_update() {
	if (cleanDrawFilters) canvas.dF.cleanFilters();
	if (cleanImageFilters) sourceController.iF.cleanFilters();
}

void ofApp::gui_draw() {
	gui.begin();
	{
		if (show_main_window)
		{
			ImGui::SetNextWindowSize(ofVec2f(gui_width, ofGetHeight()));
			ImGui::SetNextWindowPos(ofVec2f(ofGetWidth() - gui_width, 0));
			ImGui::Begin("Pixel Plotter", &show_main_window, ImGuiWindowFlags_NoDecoration);

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

			ImGui::Spacing();

			if (ImGui::CollapsingHeader("Source"))
			{
				if (!sourceController.sourceNames.empty())
				{
					if (ofxImGui::VectorCombo("##Source Image", &sourceController.currentSourceIndex, sourceController.sourceNames))
					{
						sourceController.loadSourceIndex();
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

			// Start ImageFilters
			string sImageFilterCount = "Source Filters (" + ofToString(sourceController.iF.v_ImageFilters.size()) + ")###ImageFiltersHolder";
			if (ImGui::CollapsingHeader(sImageFilterCount.c_str()))
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

				cleanImageFilters = false;
				for (int i = 0; i < sourceController.iF.v_ImageFilters.size(); i++) {
					ImGui::PushID(i);
					if (sourceController.iF.v_ImageFilters[i]->active) {
						ImGui::Indent();
						sourceController.iF.v_ImageFilters[i]->renderImGuiSettings();
						ImGui::Unindent();
					}
					else {
						cleanImageFilters = true;
					}
					ImGui::PopID();
				}

				ImGui::PopStyleColor(10);

				if (ofxImGui::VectorCombo("##Image Filter Selector", &currentImageFilterIndex, sourceController.iF.v_ImageFilterNames))
				{
					sourceController.iF.addFilter(currentImageFilterIndex);
					currentImageFilterIndex = 0;
				}
			} // End ImageFilters

			//======================================================================================================

			// Start DrawFilters
			string sDrawFilterCount = "Draw (" + ofToString(canvas.dF.v_DrawFilters.size()) + ")###DrawFiltersHolder";
			if (ImGui::CollapsingHeader(sDrawFilterCount.c_str()))
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

				cleanDrawFilters = false;
				for (int i = 0; i < canvas.dF.v_DrawFilters.size(); i++) {
					ImGui::PushID(i);
					if (canvas.dF.v_DrawFilters[i]->active) {
						ImGui::Indent();
						canvas.dF.v_DrawFilters[i]->renderImGuiSettings();
						ImGui::Unindent();
					}
					else {
						cleanDrawFilters = true;
					}
					ImGui::PopID();
				}

				ImGui::PopStyleColor(10);

				if (ofxImGui::VectorCombo("##Draw Filter Selector", &currentDrawFilterIndex, canvas.dF.v_DrawFilterNames))
				{
					canvas.dF.addFilter(currentDrawFilterIndex);
					currentDrawFilterIndex = 0;
				}
			}// End Draw Filters

			//======================================================================================================

			if (ImGui::CollapsingHeader("Canvas"))
			{
				ImGui::ColorEdit4("Canvas Colour", (float*)&canvas.c_canvas, ImGuiColorEditFlags_NoInputs);

				if (ImGui::Button("Export Vector")) { 
					canvas.saveVector = true; 
					sourceController.frameBuffer.setFresh();
				}

			} // End Canvas Settings


			ImGui::Spacing();

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