#pragma once

#include "ofApp.h"
#include "ofx2d.h"
#include "ppGui.h"
#include "ImGui_General.h"
#include "IconsFontAwesome5.h"

#define FONT_FILE_PATH "fonts/InputSans-Regular.ttf"

ppGui::ppGui()
{
	pixelplotter = (ofApp*)ofGetAppPtr();
	_bShowGui = true;
	_bShowMainMenuBar = true;

	_bShowProjectTree = false;
	_bShowTextureBrowser = true;
	_bShowZoomViewer = false;
	_bShowCanvasWindow = true;
	_bShowPropertiesWindow = true;
	_bShowPresetPanel = true;
	_bShowToolPalette = false;
	_bShowStyleEditor = false;
	_bShowMetricsWindow = false;

	_bShowFps = true;

	_bRenderingPaused = false;

	_bSavePresets = false;
	_bLoadSourceFile = true;

	_currentPresetIndex = 0;
	memset(_presetSaveName, 0, sizeof(char)*128);

}

void ppGui::setup()
{
	pixelplotter = (ofApp*)ofGetAppPtr();
	ofxGui.setup(nullptr, true, ImGuiConfigFlags_DockingEnable | ImGuiConfigFlags_ViewportsEnable, true, true);

	ImGui::GetIO().ConfigWindowsMoveFromTitleBarOnly = true;

	// Font Setup
	float baseFontSize = 13.0f; // 13.0f is the size of the default font. Change to the font size you use.
	float iconFontSize = baseFontSize * 2.0f / 3.0f; // FontAwesome fonts need to have their sizes reduced by 2.0f/3.0f in order to align correctly
	
	ofxGui.addFont(FONT_FILE_PATH, baseFontSize, nullptr, nullptr, true);

	ImFontConfig icons_config;
	icons_config.MergeMode = true;
	icons_config.PixelSnapH = true;
	icons_config.GlyphMinAdvanceX = iconFontSize;

	static const ImWchar icons_ranges[] = { ICON_MIN_FA, ICON_MAX_16_FA, 0 };

	ofxGui.addFont(FONT_ICON_FILE_NAME_FAS, iconFontSize, &icons_config, icons_ranges);
	// use FONT_ICON_FILE_NAME_FAR if you want regular instead of solid

	//ImGui::StyleColorsDark();
	ImGui::StyleColorsLight();
	ImGuiStyle* style = &ImGui::GetStyle();

	// Sizes
	style->WindowPadding = ImVec2(4, 4);
	style->FramePadding = ImVec2(4, 4);
	style->CellPadding = ImVec2(4, 4);
	style->ItemSpacing = ImVec2(4, 4);
	style->ItemInnerSpacing = ImVec2(4, 4);
	style->TouchExtraPadding = ImVec2(0, 0);
	style->IndentSpacing = 8.0f;
	style->ScrollbarSize = 16.0f;
	style->GrabMinSize = 8.0f;
	//style->ColumnsMinSpacing = 50.0f;

	// Borders
	style->WindowBorderSize = 0.0f;
	style->ChildBorderSize = 0.0f;
	style->PopupBorderSize = 0.0f;
	style->FrameBorderSize = 0.0f;
	style->TabBorderSize = 0.0f;

	// Rounding
	style->WindowRounding = 2.0f;
	style->ChildRounding = 0.0f;
	style->FrameRounding = 2.0f;
	style->PopupRounding = 0.0f;
	style->ScrollbarRounding = 2.0f;
	style->GrabRounding = 2.0f;
	style->TabRounding = 2.0f;

	// Widgets
	style->WindowTitleAlign = ImVec2(0, 0.5);
	style->WindowMenuButtonPosition = ImGuiDir_None;
	style->ColorButtonPosition = ImGuiDir_Left;
	style->ButtonTextAlign = ImVec2(0.5, 0.5);
	style->SelectableTextAlign = ImVec2(0.0, 0.0);
	style->SeparatorTextBorderSize = 8;
	style->SeparatorTextAlign = ImVec2(0.15, 0.5);
	style->SeparatorTextPadding = ImVec2(0, 8);

	// Misc
	style->DisplaySafeAreaPadding = ImVec2(0, 0);
	style->Alpha = 1.0f;

	// Colour fixes, works for all colour style modes
	//style->Colors[ImGuiCol_PopupBg] = ImVec4(0.10f, 0.10f, 0.10f, 1.00f);
	style->Colors[ImGuiCol_PopupBg] = style->Colors[ImGuiCol_MenuBarBg];

	loadPresetDir();
}

void ppGui::update()
{
	if (dragWindow) {
		if (ImGui::IsMouseReleased(0)) {
			dragWindow = false;
		}
	}

	if (_bLoadSettingsNextFrame)
	{
		loadPresets(getCurrentPresets());
		_bLoadSettingsNextFrame = false;
	}
}

void ppGui::draw()
{
	ofxGui.begin();

	if (_bShowGui) {
		showMainDock();
		if (_bShowMainMenuBar)      showMainMenuBar();
		if (_bShowToolPalette)      showToolPalette();
		if (_bShowPresetPanel)      showPresetPanel();
		if (_bShowProjectTree)      showProjectTree();
		if (_bShowTextureBrowser)   showTextureBrowser();
		if (_bShowZoomViewer)       showZoomViewer();
		if (_bShowCanvasWindow)     showCanvasWindow();
		if (_bShowPropertiesWindow) showPropertiesWindow();

		if (_bShowStyleEditor)      ImGui::ShowStyleEditor();
		if (_bShowMetricsWindow)    ImGui::ShowMetricsWindow();
	}

	ofxGui.end();
}

void ppGui::showMainDock()
{
	// Main docking space transparent
	ImGuiDockNodeFlags dockingFlags = ImGuiDockNodeFlags_PassthruCentralNode;

	// Always keep an empty "central node" (a visible oF space)
	dockingFlags |= ImGuiDockNodeFlags_NoDockingInCentralNode;

	// Define the ofWindow as a docking space
	MainDockNodeID = ImGui::DockSpaceOverViewport(NULL, dockingFlags); // Also draws the docked windows
}

void ppGui::showMainMenuBar()
{
	// TODO: Make it drag the window

	ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(5, 8));
	ImGui::BeginMainMenuBar();
	ImGui::PopStyleVar();

	if (ImGui::BeginMenu("PixelPlotter", &_bShowMainMenuBar)) {
		ImGui::Checkbox("Hide GUI [g]", &_bShowGui);
		ImGui::SameLine(); ImGui::HelpMarker("Hide the Graphical User Interface press [g] to show");

		// Sub Menu 
		//ImGui::Separator();
		if (ImGui::BeginMenu("Windows...")) {
			//ImGui::MenuItem("Something");
			ImGui::Checkbox("Project", &_bShowProjectTree);
			ImGui::Checkbox("Properties", &_bShowPropertiesWindow);
			ImGui::Checkbox("Plot Canvas", &_bShowCanvasWindow);
			ImGui::Checkbox("Info Panel", &_bShowPresetPanel);
			ImGui::Checkbox("Tool Palette", &_bShowToolPalette);
			ImGui::Checkbox("Style Editor", &_bShowStyleEditor);
			ImGui::Checkbox("Metrics Window", &_bShowMetricsWindow);
			ImGui::Checkbox("Texture Window", &_bShowTextureBrowser);
			ImGui::EndMenu();
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
		if (ImGui::Checkbox("Vertical Sync", &pixelplotter->bVsync)) {
			ofSetVerticalSync(pixelplotter->bVsync);
		}

		ImGui::Separator();

		// Window dimensions
		static int winDim[2];
		winDim[0] = ofGetWidth();
		winDim[1] = ofGetHeight();
		std::string resString = ofToString(winDim[0]).append(" x ").append(ofToString(winDim[1]));
		if (ImGui::BeginCombo("Window Dimensions", resString.c_str())) {
			if (ImGui::Selectable("800 x 600")) {
				ofSetWindowShape(800, 600);
			}
			if (ImGui::Selectable("1024 x 768")) {
				ofSetWindowShape(1024, 768);
			}
			if (ImGui::Selectable("1366 x 768")) {
				ofSetWindowShape(1366, 768);
			}
			if (ImGui::InputInt2("Custom", winDim)) {
				ofSetWindowShape(winDim[0], winDim[1]);
			}
			ImGui::EndCombo();
		}

		ImGui::Separator();

		ImGui::Checkbox("Show FPS in Title bar", &_bShowFps);

		pixelplotter->soundManager.renderImGuiSettings();

		ImGui::EndMenu();

	}

	static float fps = 0.0f;
	if (_bShowFps || renderingPaused()) {
		if(ofGetFrameNum()%10 == 0) fps = round(ofGetFrameRate());
		fpsStringStream.str(std::string());
		fpsStringStream.clear();
		fpsStringStream << "(FPS: " << fps << ")";
		if (renderingPaused()) fpsStringStream << " Rendering Paused ...";

		if (ImGui::BeginMenu(fpsStringStream.str().c_str())) {
			static std::string sRenderStatus = "Pause Rendering [p]";
			if (ImGui::MenuItem(sRenderStatus.c_str())) {
				if (_bRenderingPaused) {
					_bRenderingPaused = false;
					sRenderStatus = "Pause Rendering [p]";
				}
				else {
					_bRenderingPaused = true;
					sRenderStatus = "Continue Rendering [p]";
				}
			}
			ImGui::EndMenu();
		}
	}

	if (ImGui::InvisibleButton("Drag the window", ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y))) {
		dragWindow = false; // On release
	}
	if (ImGui::IsItemClicked()) { // On Click
		dragWindow = true;
	}

	ImGui::EndMainMenuBar();
}

void ppGui::showZoomViewer()
{
	
	// TODO: Add scale and move lock/unlock using mouse click?
	// Rename to "Zoomed View" that follows the mouse on main canvas

	ImGui::Begin("Canvas", &_bShowZoomViewer);

	ImTextureID textureID = (ImTextureID)(uintptr_t)pixelplotter->plotCanvas.canvasFbo.getTexture().getTextureData().textureID;

	float my_tex_w = pixelplotter->plotCanvas.canvasFbo.getWidth();
	float my_tex_h = pixelplotter->plotCanvas.canvasFbo.getHeight();
	ImVec2 availableRegion = ImGui::GetContentRegionAvail();

	// float fitScale = min(availableRegion.x / my_tex_w, availableRegion.y / my_tex_h);
	// ImGui::Image(textureID, glm::vec2(my_tex_w * fitScale, my_tex_h * fitScale));

	static float scale = 1;

	ImGuiIO io = ImGui::GetIO();
	ImVec2 pos = ImGui::GetCursorScreenPos();

	// Where is the mouse in the region
	float mouseRelX = CLAMP(io.MousePos.x - pos.x, 0, availableRegion.x);
	float mouseRelY = CLAMP(io.MousePos.y - pos.y, 0, availableRegion.y);

	float mouseNormX = ofx2d::remap(mouseRelX, 0, availableRegion.x, 0, my_tex_w - (availableRegion.x * scale));
	float mouseNormY = ofx2d::remap(mouseRelY, 0, availableRegion.y, 0, my_tex_h - (availableRegion.y * scale));

	ImVec2 uv0 = ImVec2(mouseNormX / my_tex_w, mouseNormY / my_tex_h);
	ImVec2 uv1 = ImVec2((mouseNormX + (availableRegion.x * scale)) / (my_tex_w), (mouseNormY + (availableRegion.y * scale)) / (my_tex_h));

	ImGui::Image(textureID, availableRegion, uv0, uv1);
	if (ImGui::IsItemHovered()) scale = std::abs(scale + (ImGui::GetIO().MouseWheel)*0.01);

	ImGui::End();
}

void ppGui::showToolPalette()
{
	ImGui::Begin("Tool palette", &_bShowToolPalette);

	// Set bigger font, needs to be loaded first...
	if (ImGui::Button(ICON_FA_PEN "Pencil"))
	{
		cout << "Pencil Button Pressed";
	}

	ImGui::End();
}

void ppGui::showPresetPanel()
{
	ImGui::Begin("Presets", &_bShowPresetPanel);

	// Save and load presets ... 
	if (ofxImGui::VectorCombo("##Presets", &_currentPresetIndex, presetFileNames))
	{
		_bLoadSettingsNextFrame = true;
	}

	if (presetFileNames.size() > 0) {
		ImGui::SameLine();
		if (ImGui::Button("Delete Preset"))
		{
			presetFiles[_currentPresetIndex].remove();
			loadPresetDir();
		}
	}

	if (_bSavePresets) {
		ImGui::InputText("##presetname", _presetSaveName, IM_ARRAYSIZE(_presetSaveName));
		ImGui::SameLine();
	}

	if (ImGui::Button("Save Preset"))
	{
		if (_bSavePresets) {
			savePresets();
			loadPresetDir();
			_currentPresetIndex = ofx2d::getIndex(presetFileNames, string(_presetSaveName), 0);
			_bSavePresets = false;
		}
		else {
			if (presetFileNames.size() > 0) {
				strcpy_s(_presetSaveName, presetFileNames[_currentPresetIndex].c_str());
			}
			_bSavePresets = true;
		}
	}

	ImGui::SameLine();
	ImGui::Checkbox("Load Source File", &_bLoadSourceFile);

	ImGui::End();
}

void ppGui::showCanvasWindow()
{
	ImGui::Begin("Canvas", &_bShowCanvasWindow);
	pixelplotter->plotCanvas.renderImGuiSettings();
	ImGui::End();
}

void ppGui::showPropertiesWindow()
{
	ImGui::Begin("Properties", &_bShowPropertiesWindow);

	if (_selectedItem) {
		auto props = _selectedItem->getProperties();
		for (auto& p : props) {
			//p.Render();
			std::string sItemName;
			ImGui::PushID("PROPWIN"+p.id);

			switch (p.type)
			{
				case EPT_BOOL:
				{
					if (ImGui::Checkbox(p.name.c_str(), (bool*)p.data)) {
						_selectedItem->setFresh(true);
					}
					break;
				}
				case EPT_INT:
				{
					if (ImGui::DragScalar(p.name.c_str(), ImGuiDataType_S32, (int32_t*)p.data)) {
						_selectedItem->setFresh(true);
					}
					break;
				}
				case EPT_UINT:
				{
					if (ImGui::DragScalar(p.name.c_str(), ImGuiDataType_U32, (uint32_t*)p.data)) {
						_selectedItem->setFresh(true);
					}
					break;
				}
				case EPT_FLOAT:
				{
					if (ImGui::DragScalar(p.name.c_str(), ImGuiDataType_Float, (float*)p.data)) {
						_selectedItem->setFresh(true);
					}
					break;
				}
				case EPT_DOUBLE:
				{
					if (ImGui::DragScalar(p.name.c_str(), ImGuiDataType_Double, (double*)p.data)) {
						_selectedItem->setFresh(true);
					}
					break;
				}
				case EPT_STRING:
				{
					if (ImGui::InputText(p.name.c_str(), (std::string*)p.data)) {
						_selectedItem->setFresh(true);
					}
					break;
				}
				case EPT_IMVEC4:
				{
					if (ImGui::ColorEdit4(p.name.c_str(), (float*)p.data, ImGuiColorEditFlags_NoInputs)) {
						_selectedItem->setFresh(true);
					}
					break;
				}
				default:
				{
					break;
				}
			}
			ImGui::PopID();
		}
	}

	ImGui::End();
}

void ppGui::showTextureBrowser()
{
	ImGui::Begin("Texture Browser", &_bShowTextureBrowser, ImGuiWindowFlags_MenuBar);

	if (ImGui::BeginMenuBar()) {
		if (ImGui::BeginMenu("Add")) {
			if (ImGui::MenuItem("New Texture")) {
				pixelplotter->textureController.add((std::string)"Texture");
			}
			ImGui::EndMenu();
		}
		
		// TODO Viewing Mode ... List or Tiles

		ImGui::EndMenuBar();
	}

	for (auto obj : pixelplotter->textureController.v_Objects) {
		ImGui::PushID("drawTextureBrowser"+obj->getId());

		ImGui::BeginGroup();
		std::string ns = "Texture " + obj->getName() + "###DTB" + std::to_string(obj->getId());
		if (ImGui::TreeNodeEx(ns.c_str()))
		{
			obj->renderImGuiSettings(0);
			ImGui::TreePop();
		}
		ImGui::EndGroup();

		if (ImGui::IsItemClicked())
		{
			obj->setSelected(true);
			_drawTexture = obj;
			_selectedItem = obj;
		}

		ImGui::PopID();
		ImGui::Separator();
	}

	ImGui::End();
}

void ppGui::showProjectTree()
{
	ImGui::Begin("Project", &_bShowProjectTree, ImGuiWindowFlags_MenuBar);

	if (ImGui::BeginMenuBar()) {
		if (ImGui::BeginMenu("Add")) {
			if (ImGui::MenuItem("New Canvas")) {
				pixelplotter->canvasController.add((std::string)"Canvas");
			}
			ImGui::EndMenu();
		}
		ImGui::EndMenuBar();
	}

	for (auto obj : pixelplotter->canvasController.v_Objects) {
		ImGui::PushID("projectTree_canvas" + obj->getId());

		ImGui::BeginGroup();
		std::string ns = "Canvas " + obj->getName() + "###PTC" + std::to_string(obj->getId());
		if (ImGui::TreeNodeEx(ns.c_str()))
		{
			obj->renderImGuiSettings(0);
			ImGui::TreePop();
		}
		ImGui::EndGroup();

		if (ImGui::IsItemClicked())
		{
			obj->setSelected(true);
			_drawTexture = obj;
			_selectedItem = obj;
		}

		ImGui::PopID();
		ImGui::Separator();
	}

	// TODO: Layers move to Canvas
	/*
	if (ImGui::TreeNode("Layers"))
	{
			ImGui::TreePop();
		}
	*/

	ImGui::End();
}

void ppGui::savePresets()
{
	string savePath = "presets/" + string(_presetSaveName) + ".json";

	ofJson settings;

	settings["source"].push_back(pixelplotter->plotCanvas.sourceController.getSettings());

	for (int i = 0; i < pixelplotter->plotCanvas.sourceController.iF.v_Objects.size(); i++) {
		settings["imageFilters"].push_back(pixelplotter->plotCanvas.sourceController.iF.v_Objects[i]->getSettings());
	}
	
	/* Not done here. Refactor TODO
	for (int i = 0; i < pixelplotter->plotCanvas.sourceController.gC.v_Objects.size(); i++) {
		settings["generators"].push_back(pixelplotter->plotCanvas.sourceController.gC.v_Objects[i]->getSettings());
	}
	*/

	settings["plotCanvas"] = pixelplotter->plotCanvas.getSettings();

	ofSavePrettyJson(savePath, settings);
}

void ppGui::loadPresets(ofJson settings)
{
	pixelplotter->plotCanvas.dF.clear();
	pixelplotter->plotCanvas.sourceController.iF.clear();

	if (_bLoadSourceFile) {
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
			pixelplotter->plotCanvas.sourceController.iF.add(fSettings);
		}
	}

	pixelplotter->plotCanvas.loadSettings(settings.value("plotCanvas", ofJson::array()));

}

bool ppGui::guiVisible()
{
	return _bShowGui;
}

void ppGui::setGuiVisible(bool bVisible)
{
	_bShowGui = bVisible;
}

bool ppGui::menuBarVisible()
{
	return _bShowMainMenuBar;
}

void ppGui::setmenuBarVisible(bool bVisible)
{
	_bShowMainMenuBar = bVisible;
}

bool ppGui::renderingPaused()
{
	return _bRenderingPaused;
}

void ppGui::setRenderingPaused(bool bPaused)
{
	_bRenderingPaused = bPaused;
}

void ppGui::loadPresetDir()
{
	presetFileNames.clear();
	presetFiles = ofDirectory(ofToDataPath("presets", true)).getFiles();
	
	/* Only load JSON files */
	presetFiles.erase(
		std::remove_if(
			presetFiles.begin(),
			presetFiles.end(),
			[](ofFile const& f) {
				return f.getFileName().substr(f.getFileName().find_last_of('.'), -1) != ".json";
			}
		),
		presetFiles.end()
	);

	for (int i = 0; i < presetFiles.size(); i++)
	{
		string base_filename = presetFiles[i].getFileName();
		string pname = base_filename.substr(0, base_filename.find_last_of('.'));
		string ext = base_filename.substr(base_filename.find_last_of('.'),-1);
		presetFileNames.push_back(pname);
	}
}

std::string ppGui::getPresetAbsolutePath(int presetIndex)
{
	return presetFiles[presetIndex].getAbsolutePath().generic_string();
}

ofJson ppGui::getPresets(int presetIndex)
{
	ofJson settings;
	ofFile file(presetFiles[presetIndex].getAbsolutePath());
	if (file.exists()) {
		file >> settings;
	}
	return settings;
}

ofJson ppGui::getCurrentPresets()
{
	return getPresets(_currentPresetIndex);
}

ImVec4 ppGui::availableSpace()
{
	// Return Values: Width, Height, x, y
	ImVec4 rVal(0,0,0,0);

	auto centralNode = ImGui::DockBuilderGetCentralNode(MainDockNodeID);
	rVal.x = centralNode->Size.x;
	rVal.y = centralNode->Size.y;
	rVal.z = centralNode->Pos.x - ofGetWindowPositionX();
	rVal.w = centralNode->Pos.y - ofGetWindowPositionY();

	return rVal;
}
