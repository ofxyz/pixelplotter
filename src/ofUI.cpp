#include "ofApp.h"

void ofApp::showUI() {
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

				if (ofxImGui::VectorCombo("Plot Style", &currentPlotStyleIndex, v_PlotStyles))
				{
					// Done
				}

				ImGui::PushItemWidth(100);

				ImGui::Text("Tiles"); ImGui::SameLine(75);
				ImGui::DragInt("X ##Tiles", &tilesX, 1, 1, 1200);
				ImGui::SameLine();
				ImGui::DragInt("Y ##Tiles", &tilesY, 1, 1, 1200);

				ImGui::Text("Addon"); ImGui::SameLine(75);
				ImGui::SliderFloat("X ##Addon", &addonx, -100.0f, 100.0f, "%.3f");
				ImGui::SameLine();
				ImGui::SliderFloat("Y ##Addon", &addony, -100.0f, 100.0f, "%.3f");

				ImGui::Text("Offset"); ImGui::SameLine(75);
				ImGui::SliderFloat("Random", &randomOffset, 0.0f, 250.0f, "%.3f%%");

				ImGui::Text("Noise"); ImGui::SameLine(75);
				ImGui::SliderFloat("X ##Noise", &noisepercentX, 0.0f, 100.0f, "%.2f%%");
				ImGui::SameLine();
				ImGui::SliderFloat("Y ##Noise", &noisepercentY, 0.0f, 100.0f, "%.2f%%");

				ImGui::Text("Every N"); ImGui::SameLine(75);
				ImGui::DragInt("X ##Every N", &everynx, 1, 1, 128);
				ImGui::SameLine();
				ImGui::DragInt("Y ##Every N", &everyny, 1, 1, 128);

				ImGui::PopItemWidth();

				// Pixel Type: line, square, rect, round, oval
				// Use Radio or dropdown
				if (roundPixels) {
					if (ImGui::Button("Square Pixels"))
					{
						roundPixels = false;
					}
				}
				else {
					if (ImGui::Button("Round Pixels"))
					{
						roundPixels = true;
					}
				}
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

				ImGui::Checkbox("Normalise Colours", &normalise);

				if (ofxImGui::VectorCombo("Blend Mode", &currentBlendModeIndex, v_BlendModes))
				{
					currentBlendmode = v_BlendModes[currentBlendModeIndex];
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