#pragma once

#include "ofGui.h"
#include "ofxImGui.h"
#include "imgui_internal.h"
#include "ImHelpers.h"
#include "imgui_stdlib.h"
#include "IconsFontAwesome5.h"
#include "ImGui_Widget_Bezier.h"
#include "ImGui_Widget_Tooltip.h"

#include "ofApp.h"
#include "ofx2d.h"

OfGui::OfGui()
{
	pixelplotter = (ofApp*)ofGetAppPtr();
	bShowGui = true;
	bShowMenuBar = true;
	bShowPlotCanvas = true;
	bShowInfoPanel = true;
	bShowToolPalette = true;
	bShowImGuiMetricsDebugger = false;
	bShowCanvas = true;

	bRenderingPaused = false;
	bShowFps = true;
	bSavePresets = false;
	bTryLoadSource = true;

	currentPresetIndex = 0;
	memset(presetSaveName, 0, sizeof(char)*128);

}

void OfGui::setup()
{
	pixelplotter = (ofApp*)ofGetAppPtr();
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

	ImGuiIO IO = ImGui::GetIO();
	IO.ConfigWindowsMoveFromTitleBarOnly = true;

	loadPresetDir();
}

void OfGui::update()
{
	if (bLoadSettingsNextFrame)
	{
		loadPresets(getCurrentPresets());
		bLoadSettingsNextFrame = false;
	}
}

void OfGui::draw()
{

	if (bShowGui)
	{
		gui.begin();
		drawMainDock();

		if (bShowMenuBar) drawMenuBar();
		if (bShowCanvas) drawCanvas();
		if (bShowToolPalette) drawToolPalette();
		if (bShowInfoPanel) drawInfoPanel();
		if (bShowCanvas) drawCanvasWindow();

		if (bShowImGuiMetricsDebugger) {
			ImGui::ShowMetricsWindow();
			ImGui::ShowStyleEditor();
		}

		gui.end();
	}
}

void OfGui::drawMainDock()
{
	// Make main docking space transparent
	ImGuiDockNodeFlags dockingFlags = ImGuiDockNodeFlags_PassthruCentralNode;

	// Alternative: Otherwise add in ImGui::DockSpace() [±line 14505] : if (flags & ImGuiDockNodeFlags_PassthruCentralNode) window_flags |= ImGuiWindowFlags_NoBackground;
	//ImGui::PushStyleColor(ImGuiCol_ChildBg, IM_COL32(0, 0, 0, 0));

	dockingFlags |= ImGuiDockNodeFlags_NoDockingInCentralNode; // Uncomment to always keep an empty "central node" (a visible oF space)
	//dockingFlags |= ImGuiDockNodeFlags_NoTabBar; // Uncomment to disable creating tabs in the main view

	// Define the ofWindow as a docking space
	MainDockNodeID = ImGui::DockSpaceOverViewport(NULL, dockingFlags); // Also draws the docked windows

}

void OfGui::drawMenuBar()
{
	ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(5, 8));
	ImGui::BeginMainMenuBar();
	ImGui::PopStyleVar();

	if (ImGui::BeginMenu("PixelPlotter", &bShowMenuBar)) {
		ImGui::Checkbox("Hide GUI [g]", &bShowGui);
		ImGui::SameLine(); ImGui::HelpMarker("Hide the Graphical User Interface press [g] to show");

		if (bShowGui) {
			// Sub Menu 
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
			if (bRenderingPaused) {
				bRenderingPaused = false;
				renderPaused = "Pause Rendering";
			}
			else {
				bRenderingPaused = true;
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
			pixelplotter->plotCanvas.sourceController.showSource = true;
		}
		if (ImGui::MenuItem("Plot Canvas")) {
			pixelplotter->plotCanvas.sourceController.showSource = false;
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
		if (ImGui::Checkbox("Vertical Sync", &pixelplotter->vSync)) {
			ofSetVerticalSync(pixelplotter->vSync);
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

		if (ImGui::Checkbox("Show FPS in Title bar", &bShowFps)) {
			if (!bShowFps) ofSetWindowTitle(pixelplotter->getWindowTitle());
		};

		pixelplotter->soundManager.renderImGuiSettings();

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

// TODO: This should be a general texture viewer...
void OfGui::drawCanvas()
{
	ImGui::Begin("Canvas", &bShowCanvas);
	ImGui::PushID("showCanvas");

	ImTextureID textureID = (ImTextureID)(uintptr_t)pixelplotter->plotCanvas.canvasFbo.getTexture().getTextureData().textureID;

	float tw = pixelplotter->plotCanvas.canvasFbo.getWidth();
	float th = pixelplotter->plotCanvas.canvasFbo.getHeight();
	auto availableRegion = ImGui::GetContentRegionAvail();
	float scale = min(availableRegion.x / tw, availableRegion.y / th);

	ImGui::Image(textureID, glm::vec2(tw * scale, th * scale));
	ImGui::PopID();
	ImGui::End();
}

void OfGui::drawToolPalette()
{
	ImGui::Begin("Tool palette", &bShowToolPalette);
	ImGui::PushID("toolPalette");

	// Set bigger font, needs to be loaded first...
	if (ImGui::Button(ICON_FA_PEN "Pencil"))
	{
		cout << "Pencil Button Pressed";
	}

	drawBezierWidget();

	ImGui::PopID();
	ImGui::End();
}

void OfGui::drawInfoPanel()
{
	ImGui::Begin("Presets", &bShowInfoPanel);

	// Save and load presets ... 
	if (ofxImGui::VectorCombo("##Presets", &currentPresetIndex, presetFileNames))
	{
		bLoadSettingsNextFrame = true;
		//TODO: This should be a function in ofApp?
		pixelplotter->plotCanvas.resizeRequest = true;
		pixelplotter->plotCanvas.setFresh(true);
	}

	if (presetFileNames.size() > 0) {
		ImGui::SameLine();
		if (ImGui::Button("Delete Preset"))
		{
			presetFiles[currentPresetIndex].remove();
			loadPresetDir();
		}
	}

	if (bSavePresets) {
		ImGui::InputText("##presetname", presetSaveName, IM_ARRAYSIZE(presetSaveName));
		ImGui::SameLine();
	}

	if (ImGui::Button("Save Preset"))
	{
		if (bSavePresets) {
			savePresets();
			loadPresetDir();
			currentPresetIndex = ofx2d::getIndex(presetFileNames, string(presetSaveName), 0);
			bSavePresets = false;
		}
		else {
			if (presetFileNames.size() > 0) {
				strcpy_s(presetSaveName, presetFileNames[currentPresetIndex].c_str());
			}
			bSavePresets = true;
		}
	}

	ImGui::SameLine();
	ImGui::Checkbox("Load Source File", &bTryLoadSource);

	ImGui::End();
}

void OfGui::drawCanvasWindow()
{
	ImGui::Begin("Plot Canvas", &bShowPlotCanvas);
	ImGui::PushID("plotCanvas");
	pixelplotter->plotCanvas.renderImGuiSettings();
	ImGui::PopID();

	ImGui::End();
}

void OfGui::drawBezierWidget()
{
	static float v[5] = { 0.390f, 0.575f, 0.565f, 1.000f };
	ImGui::Bezier("easeOutSine", v);       // draw
	float y = ImGui::BezierValue(0.5f, v); // x delta in [0..1] range
}

void OfGui::savePresets()
{
	string savePath = "presets/" + string(presetSaveName) + ".json";

	ofJson settings;

	settings["source"].push_back(pixelplotter->plotCanvas.sourceController.getSettings());

	for (int i = 0; i < pixelplotter->plotCanvas.sourceController.iF.v_ImageFilters.size(); i++) {
		settings["imageFilters"].push_back(pixelplotter->plotCanvas.sourceController.iF.v_ImageFilters[i]->getSettings());
	}

	settings["plotCanvas"] = pixelplotter->plotCanvas.getSettings();

	ofSavePrettyJson(savePath, settings);
}

void OfGui::loadPresets(ofJson settings)
{
	pixelplotter->plotCanvas.dF.clear();
	pixelplotter->plotCanvas.sourceController.iF.clear();

	if (bTryLoadSource) {
		ofJson sources = settings.value("source", ofJson::array());
		if (!sources.empty())
		{
			for (auto& cSettings : sources) {
				pixelplotter->plotCanvas.sourceController.loadSettings(cSettings);
			}
		}
	}

	ofJson iFilters = settings.value("imageFilters", ofJson::array());
	if (!iFilters.empty()) {
		for (auto& fSettings : iFilters) {
			pixelplotter->plotCanvas.sourceController.iF.addFilter(fSettings);
		}
	}

	pixelplotter->plotCanvas.loadSettings(settings.value("plotCanvas", ofJson::array()));

}

void OfGui::loadPresetDir()
{
	presetFileNames.clear();
	presetFiles = ofDirectory(ofToDataPath("presets", true)).getFiles();
	for (int i = 0; i < presetFiles.size(); i++)
	{
		string base_filename = presetFiles[i].getFileName();
		string pname = base_filename.substr(0, base_filename.find_last_of('.'));
		presetFileNames.push_back(pname);
	}
}

std::string OfGui::getPresetAbsolutePath(int presetIndex)
{
	return presetFiles[presetIndex].getAbsolutePath();
}

ofJson OfGui::getPresets(int presetIndex)
{
	ofJson settings;
	ofFile file(presetFiles[presetIndex].getAbsolutePath());
	if (file.exists()) {
		file >> settings;
	}
	return settings;
}

ofJson OfGui::getCurrentPresets()
{
	return getPresets(currentPresetIndex);
}

ImVec4 OfGui::availableSpace()
{
	// Width, Height, Top, Left
	ImVec4 rVal(0,0,0,0);

	ImGuiDockNode* centralNode = ImGui::DockBuilderGetCentralNode(MainDockNodeID);
	if (centralNode && centralNode->IsEmpty()) {
		ImRect availableSpace = centralNode->Rect();
		ImVec2 offset = availableSpace.GetTL();
		offset.x -= ofGetWindowPositionX();
		offset.y -= ofGetWindowPositionY();

		rVal.x = availableSpace.GetWidth();
		rVal.y = availableSpace.GetHeight();
		rVal.z = offset.x;
		rVal.w = offset.y;
	}

	return rVal;
}
