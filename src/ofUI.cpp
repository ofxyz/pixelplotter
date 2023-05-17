#include "ofApp.h"

void ofApp::gui_update() {
	if (cleanImageFilters) {
		plotCanvas.sourceController.iF.cleanFilters();
		cleanImageFilters = false;
	}
}

void ofApp::gui_draw() {
	gui.begin();

	gui_drawMenuBar();
	gui_drawMainDock();	

	if (bShowGui)
	{
		gui_drawInfoPanel();
		gui_drawCanvasWindow();
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

void ofApp::gui_drawMainDock() {
	// Make main docking space transparent
	ImGuiDockNodeFlags dockingFlags = ImGuiDockNodeFlags_PassthruCentralNode;

	// Alternative: Otherwise add in ImGui::DockSpace() [±line 14505] : if (flags & ImGuiDockNodeFlags_PassthruCentralNode) window_flags |= ImGuiWindowFlags_NoBackground;
	//ImGui::PushStyleColor(ImGuiCol_ChildBg, IM_COL32(0, 0, 0, 0));

	dockingFlags |= ImGuiDockNodeFlags_NoDockingInCentralNode; // Uncomment to always keep an empty "central node" (a visible oF space)
	//dockingFlags |= ImGuiDockNodeFlags_NoTabBar; // Uncomment to disable creating tabs in the main view

	// Define the ofWindow as a docking space
	ImGuiID dockNodeID = ImGui::DockSpaceOverViewport(NULL, dockingFlags); // Also draws the docked windows

	ImGuiDockNode* dockNode = ImGui::DockBuilderGetNode(dockNodeID);
	if (dockNode) {
		ImGuiDockNode* centralNode = ImGui::DockBuilderGetCentralNode(dockNodeID);
		// Verifies if the central node is empty (visible empty space for oF)
		if (centralNode && centralNode->IsEmpty()) {
			ImRect availableSpace = centralNode->Rect();
			workSpaceWidthHeight.x = availableSpace.GetWidth();
			workSpaceWidthHeight.y = availableSpace.GetHeight();
			offset = availableSpace.GetTL();
			offset.x -= ofGetWindowPositionX();
			offset.y -= ofGetWindowPositionY();
			workSpaceCentre = availableSpace.GetCenter();
			//ImGui::GetForegroundDrawList()->AddRect(availableSpace.GetTL() + ImVec2(1, 1), availableSpace.GetBR() - ImVec2(1, 1), IM_COL32(255, 50, 50, 255));
		}
	}
}

void ofApp::gui_drawMenuBar() {
	if (!bShowMenuBar) return;
	ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(5, 8));
	ImGui::BeginMainMenuBar();
	ImGui::PopStyleVar();

	if (ImGui::BeginMenu("PixelPlotter", &bShowMenuBar)) {
		ImGui::Checkbox("Show GUI", &bShowGui);
		//ImGui::SameLine(); HelpMarker("Shows properties window...");

		if (bShowGui) {
			// Submenu 
			//ImGui::Separator();
			if (ImGui::BeginMenu("Windows...")) {
				//ImGui::MenuItem("Something");
				ImGui::Checkbox("Plot Canvas", &bShowPlotCanvas);
				ImGui::Checkbox("Info Panel", &bShowInfoPanel);
				ImGui::EndMenu();
			}
		}

		static std::string renderPaused = "Pause Rendering";
		if (ImGui::MenuItem(renderPaused.c_str())) {
			if (pauseRender) {
				pauseRender = false;
				renderPaused = "Pause Rendering";
			}
			else {
				pauseRender = true;
				renderPaused = "Continue Rendering";
			}
		}

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

		if (ImGui::Checkbox("Show FPS in Titlebar", &bShowFps)) {
			if(!bShowFps) ofSetWindowTitle(windowTitle);
		};

		soundManager.renderImGuiSettings();

		ImGui::EndMenu();
		
	}

	static std::string canvasSelected;
	if (plotCanvas.sourceController.showSource) {
		canvasSelected = "Source Canvas";
	}
	else {
		canvasSelected = "Plot Canvas";
	}

	if (ImGui::BeginMenu(canvasSelected.c_str())) {
		if (ImGui::MenuItem("Source Canvas")) {
			plotCanvas.sourceController.showSource = true;
		}
		if (ImGui::MenuItem("Plot Canvas")) {
			plotCanvas.sourceController.showSource = false;
		}
		ImGui::EndMenu();
	}

	ImGui::EndMainMenuBar();
}

void ofApp::gui_drawCanvasWindow() {
	if (!bShowPlotCanvas) return;

	//ImGui::SetNextWindowSize(ofVec2f(gui_width, 800), ImGuiCond_Once);
	//ImGui::SetNextWindowPos(ofVec2f( (ofGetWidth() - gui_width) + 45, 200+ 100), ImGuiCond_Once);
	ImGui::Begin("Plot Canvas", &bShowPlotCanvas);
	ImGui::PushID("plotcanvas");
	plotCanvas.renderImGuiSettings();
	ImGui::PopID();

	ImGui::End();
}

void ofApp::gui_drawInfoPanel() {
	if (!bShowInfoPanel) return;

	//ImGui::SetNextWindowSize(ofVec2f(gui_width, 200), ImGuiCond_Once);
	//ImGui::SetNextWindowPos(ofVec2f((ofGetWidth() - gui_width) + 45, 100), ImGuiCond_Once);
	ImGui::Begin("Presets", &bShowInfoPanel);

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

	ImGui::End();
}