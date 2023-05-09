#include "ofApp.h"

void ofApp::gui_update() {
	if (cleanImageFilters) {
		sourceController.iF.cleanFilters();
		cleanImageFilters = false;
	}
}

void ofApp::gui_draw() {
	gui.begin();

	// Make windows transparent, to demonstrate drawing behind them.
	//ImGui::PushStyleColor(ImGuiCol_WindowBg, IM_COL32(200, 200, 200, 128)); // This styles the docked windows

	ImGuiDockNodeFlags dockingFlags = ImGuiDockNodeFlags_PassthruCentralNode; // Make the docking space transparent
	// Fixes imgui to expected behaviour, having a transparent central node in passthru mode.
	// Alternative: Otherwise add in ImGui::DockSpace() [±line 14505] : if (flags & ImGuiDockNodeFlags_PassthruCentralNode) window_flags |= ImGuiWindowFlags_NoBackground;
	//ImGui::PushStyleColor(ImGuiCol_ChildBg, IM_COL32(0, 0, 0, 0));

	dockingFlags |= ImGuiDockNodeFlags_NoDockingInCentralNode; // Uncomment to always keep an empty "central node" (a visible oF space)
	//dockingFlags |= ImGuiDockNodeFlags_NoTabBar; // Uncomment to disable creating tabs in the main view

	// Define the ofWindow as a docking space
	ImGuiID dockNodeID = ImGui::DockSpaceOverViewport(NULL, dockingFlags); // Also draws the docked windows
	//ImGui::PopStyleColor(2);

	ImGuiDockNode* dockNode = ImGui::DockBuilderGetNode(dockNodeID);
	if (dockNode) {
		ImGuiDockNode* centralNode = ImGui::DockBuilderGetCentralNode(dockNodeID);
		// Verifies if the central node is empty (visible empty space for oF)
		if (centralNode && centralNode->IsEmpty()) {
			ImRect availableSpace = centralNode->Rect();
			//ImGui::GetForegroundDrawList()->AddRect(availableSpace.GetTL() + ImVec2(8, 8), availableSpace.GetBR() - ImVec2(8, 8), IM_COL32(255, 50, 50, 255));
			//ImVec2 viewCenter = availableSpace.GetCenter();
		}
	}

	gui_drawMenuBar();
	{
		if (bShowGui)
		{
			gui_drawCanvasWindow();

			ImGui::SetNextWindowSize(ofVec2f(gui_width, 500), ImGuiCond_Once);
			ImGui::SetNextWindowPos(ofVec2f(ofGetWidth() - gui_width, 0), ImGuiCond_Once);
			ImGui::Begin("Properties", &bShowGui);

			// Save and load presets ... 
			if (ofxImGui::VectorCombo("##Presets", &currentPresetIndex, presetFileNames))
			{
				bLoadSettingsNextFrame = true;
				plotCanvas.resizeRequest = true;
				plotCanvas.setFresh(true);
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
			ImGui::Spacing();

			string sSourceFilterCount = "Plot Source (" + ofToString(sourceController.iF.v_ImageFilters.size()+1) + ")###Source";
			if (ImGui::CollapsingHeader(sSourceFilterCount.c_str()))
			{
				sourceController.renderImGuiSettings();

				// Start ImageFilters
				//-----------------------------------------------------------------------------------------------------
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

				// End ImageFilters
				//-----------------------------------------------------------------------------------------------------
			}

			//======================================================================================================

			ImGui::Spacing();
			ImGui::Spacing();
			ImGui::Spacing();
			ImGui::Spacing();
			ImGui::Spacing();

			//======================================================================================================

			/*
			if (ImGui::CollapsingHeader("Plot Canvas"))
			{
				ImGui::PushID("plotcanvas");
				plotCanvas.renderImGuiSettings();
				ImGui::PopID();
			}
			*/
			ImGui::Spacing();
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

void ofApp::gui_setup()
{
	gui.setup(nullptr, true, ImGuiConfigFlags_DockingEnable | ImGuiConfigFlags_ViewportsEnable, true, true);

	ImGui::StyleColorsDark();
	ImGuiStyle* style = &ImGui::GetStyle();
	style->ItemSpacing = ImVec2(5, 5);
}

void ofApp::gui_drawMenuBar() {
	if (bShowMenuBar) {
		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(5, 10));
		ImGui::BeginMainMenuBar();
		ImGui::PopStyleVar();

		if (ImGui::BeginMenu("PixelPlotter", &bShowMenuBar)) {
			ImGui::Checkbox("Show Properties", &bShowGui);
			ImGui::SameLine(); HelpMarker("Shows properties window...");

			// Submenu
			ImGui::Separator();
			if (ImGui::BeginMenu("More...")) {
				ImGui::MenuItem("Something");
				ImGui::MenuItem("Something else");
				ImGui::MenuItem("Something different");
				ImGui::EndMenu();
			}

			// Exit
			ImGui::Separator();
			if (ImGui::MenuItem("Quit")) {
				ofExit();
			}

			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Settings")) {

			// Full Screen
			static bool fullScreen = false;
			if (ImGui::Checkbox("Full screen", &fullScreen)) {
				ofSetFullscreen(fullScreen);
			}

			// Vertical Sync
			static bool vSync = false;
			if (ImGui::Checkbox("Vertical Sync", &vSync)) {
				ofSetVerticalSync(vSync);
			}

			ImGui::Separator();

			// Resolution changer
			static int resolution[2];
			resolution[0] = ofGetWidth();
			resolution[1] = ofGetHeight();
			std::string resString = ofToString(resolution[0]).append(" x ").append(ofToString(resolution[1]));
			if (ImGui::BeginCombo("Resolution", resString.c_str())) {
				if (ImGui::Selectable("800 x 600")) {
					ofSetWindowShape(800, 600);
				}
				if (ImGui::Selectable("1024 x 768")) {
					ofSetWindowShape(1024, 768);
				}
				if (ImGui::Selectable("1366 x 768")) {
					ofSetWindowShape(1366, 768);
				}
				if (ImGui::InputInt2("Custom", resolution)) {
					ofSetWindowShape(resolution[0], resolution[1]);
				}
				ImGui::EndCombo();
			}

			ImGui::Separator();

			soundManager.renderImGuiSettings();

			ImGui::EndMenu();
		}

		ImGui::EndMainMenuBar();

	} //End if bShowMenuBar
}

void ofApp::gui_drawCanvasWindow() {
	ImGui::SetNextWindowSize(ofVec2f(gui_width, 500), ImGuiCond_Once);
	ImGui::SetNextWindowPos(ofVec2f(ofGetWidth() - gui_width, 0), ImGuiCond_Once);
	ImGui::Begin("Plot Canvas");
	
	ImGui::PushID("plotcanvas");
	plotCanvas.renderImGuiSettings();
	ImGui::PopID();

	ImGui::End();
}