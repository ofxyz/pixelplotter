#include "ofApp.h"
#include "ImGui_Widget_Bezier.h"

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
		gui_renderCanvas();
		gui_drawToolPalette();
		gui_drawInfoPanel();
		gui_drawCanvasWindow();
		

		if (bShowImGuiMetricsDebugger) {
			ImGui::ShowMetricsWindow();
			ImGui::ShowStyleEditor();
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
	
	// Font Setup
	float baseFontSize = 13.0f; // 13.0f is the size of the default font. Change to the font size you use.
	float iconFontSize = baseFontSize * 2.0f / 3.0f; // FontAwesome fonts need to have their sizes reduced by 2.0f/3.0f in order to align correctly
	gui.setDefaultFont(baseFontSize);

	ImFontConfig icons_config;
	icons_config.MergeMode = true;
	icons_config.PixelSnapH = true;
	icons_config.GlyphMinAdvanceX = iconFontSize;

	static const ImWchar icons_ranges[] = { ICON_MIN_FA, ICON_MAX_16_FA, 0 };

	gui.addFont(FONT_ICON_FILE_NAME_FAS, iconFontSize, &icons_config, icons_ranges);
	// use FONT_ICON_FILE_NAME_FAR if you want regular instead of solid
	//---------------------------

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
	dockNodeID = ImGui::DockSpaceOverViewport(NULL, dockingFlags); // Also draws the docked windows

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
				ImGui::Checkbox("Tool Palette", &bShowToolPalette);
				ImGui::Checkbox("ImGui Metrics Debugger", &bShowImGuiMetricsDebugger);
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

	if (ImGui::BeginMenu("View")) {
		if (ImGui::MenuItem("Source Canvas")) {
			plotCanvas.sourceController.showSource = true;
		}
		if (ImGui::MenuItem("Plot Canvas")) {
			plotCanvas.sourceController.showSource = false;
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
			if (!bShowFps) ofSetWindowTitle(windowTitle);
		};

		soundManager.renderImGuiSettings();

		ImGui::EndMenu();

	}

	if (bShowFps) {
		fpsStringStream.str(std::string());
		fpsStringStream.clear();
		fpsStringStream << "(FPS: " << ofGetFrameRate() << ")";

		if (ImGui::BeginMenu(fpsStringStream.str().c_str())) {
			ImGui::EndMenu();
		}
	}

	ImGui::EndMainMenuBar();
}

void ofApp::gui_drawToolPalette() {
	// TODO: Move all these to where they're drawn ...
	if (!bShowToolPalette) return;
	 
	ImGui::Begin("Tool Pallette", &bShowToolPalette);
	ImGui::PushID("toolPallette");

	// Set bigger font, needs to be loadded first...
	if (ImGui::Button(ICON_FA_PEN "Pencil"))
	{
		cout << "Pencil Button Pressed";
	}

	gui_renderBezierWidget();

	ImGui::PopID();
	ImGui::End();
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

// This should be a more general texture viewer
void ofApp::gui_renderCanvas() {
	ImGui::Begin("Canvas", &bShowCanvas);
	ImGui::PushID("showcanvas");

	ImTextureID textureID = (ImTextureID)(uintptr_t)plotCanvas.canvasFbo.getTexture().getTextureData().textureID;

	float tw = plotCanvas.canvasFbo.getWidth();
	float th = plotCanvas.canvasFbo.getHeight();
	auto availableRegion = ImGui::GetContentRegionAvail();
	float scale = min(availableRegion.x / tw, availableRegion.y / th);

	ImGui::Image(textureID, glm::vec2(tw * scale, th * scale));
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

	ImGui::SameLine();
	ImGui::Checkbox("Load Source File", &bTryLoadSource);

	ImGui::End();
}

void ofApp::gui_renderBezierWidget() {
	static float v[5] = { 0.390f, 0.575f, 0.565f, 1.000f };
	ImGui::Bezier( "easeOutSine", v );       // draw
	float y = ImGui::BezierValue( 0.5f, v ); // x delta in [0..1] range
}
