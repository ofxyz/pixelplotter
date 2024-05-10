#pragma once

#include "ofGui.h"
#include "ImGui_General.h"
#include "IconsFontAwesome5.h"

#include "ofApp.h"
#include "ofx2d.h"

#define FONT_FILE_PATH "fonts/InputSans-Regular.ttf"

OfGui::OfGui()
{
	pixelplotter = (ofApp*)ofGetAppPtr();
	_bGuiVisible = true;
	_bMenuBarVisible = true;

	_bShowProjectTree = false;
	_bShowTextureTree = true;
	_bShowCanvas = false; // TODO: FinisRename to Zoomed Viewer
	_bShowPlotCanvas = true;
	_bShowInfoPanel = true;
	_bShowToolPalette = false;
	_bShowImGuiStyleEditor = false;
	_bShowImGuiMetricsWindow = false;
	_bShowPropertiesWindow = true;

	_bShowFps = true;

	_bRenderingPaused = false;

	_bSavePresets = false;
	_bLoadSourceFile = true;

	_currentPresetIndex = 0;
	memset(_presetSaveName, 0, sizeof(char)*128);

}

void OfGui::setup()
{
	pixelplotter = (ofApp*)ofGetAppPtr();
	gui.setup(nullptr, true, ImGuiConfigFlags_DockingEnable | ImGuiConfigFlags_ViewportsEnable, true, true);

	ImGui::GetIO().ConfigWindowsMoveFromTitleBarOnly = true;

	// Font Setup
	float baseFontSize = 13.0f; // 13.0f is the size of the default font. Change to the font size you use.
	float iconFontSize = baseFontSize * 2.0f / 3.0f; // FontAwesome fonts need to have their sizes reduced by 2.0f/3.0f in order to align correctly
	
	gui.addFont(FONT_FILE_PATH, baseFontSize, nullptr, nullptr, true);

	ImFontConfig icons_config;
	icons_config.MergeMode = true;
	icons_config.PixelSnapH = true;
	icons_config.GlyphMinAdvanceX = iconFontSize;

	static const ImWchar icons_ranges[] = { ICON_MIN_FA, ICON_MAX_16_FA, 0 };

	gui.addFont(FONT_ICON_FILE_NAME_FAS, iconFontSize, &icons_config, icons_ranges);
	// use FONT_ICON_FILE_NAME_FAR if you want regular instead of solid

	//ImGui::StyleColorsDark();
	ImGui::StyleColorsLight();
	ImGuiStyle* style = &ImGui::GetStyle();

	// Sizes
	style->WindowPadding = ImVec2(4, 4);
	style->FramePadding = ImVec2(4, 4);
	style->CellPadding = ImVec2(4, 4);
	style->ItemSpacing = ImVec2(4, 4);
	style->ItemInnerSpacing = ImVec2(2, 2);
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
	style->Colors[ImGuiCol_PopupBg] = style->Colors[ImGuiCol_WindowBg];
	//style->Colors[ImGuiCol_Text] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
	
	loadPresetDir();
}

void OfGui::update()
{
	if (_bLoadSettingsNextFrame)
	{
		loadPresets(getCurrentPresets());
		_bLoadSettingsNextFrame = false;
	}
}

void OfGui::draw()
{
	gui.begin();

	if (_bGuiVisible) {
		drawMainDock();
		if (_bMenuBarVisible) drawMenuBar();
		if (_bShowProjectTree) drawProjectTree();
		if (_bShowTextureTree) drawTextureBrowser();
		if (_bShowCanvas) drawCanvas();
		if (_bShowToolPalette) drawToolPalette();
		if (_bShowInfoPanel) drawInfoPanel();
		if (_bShowPlotCanvas) drawCanvasWindow();
		if (_bShowImGuiStyleEditor) ImGui::ShowStyleEditor();
		if (_bShowImGuiMetricsWindow) ImGui::ShowMetricsWindow();
		if (_bShowPropertiesWindow) drawPropertiesWindow();
	}

	gui.end();
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
	// TODO: Make it drag the window

	ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(5, 8));
	ImGui::BeginMainMenuBar();
	ImGui::PopStyleVar();

	if (ImGui::BeginMenu("PixelPlotter", &_bMenuBarVisible)) {
		ImGui::Checkbox("Hide GUI [g]", &_bGuiVisible);
		ImGui::SameLine(); ImGui::HelpMarker("Hide the Graphical User Interface press [g] to show");

		// Sub Menu 
		//ImGui::Separator();
		if (ImGui::BeginMenu("Windows...")) {
			//ImGui::MenuItem("Something");
			ImGui::Checkbox("Project", &_bShowProjectTree);
			ImGui::Checkbox("Properties", &_bShowPropertiesWindow);
			ImGui::Checkbox("Plot Canvas", &_bShowPlotCanvas);
			ImGui::Checkbox("Info Panel", &_bShowInfoPanel);
			ImGui::Checkbox("Tool Palette", &_bShowToolPalette);
			ImGui::Checkbox("Style Editor", &_bShowImGuiStyleEditor);
			ImGui::Checkbox("Metrics Window", &_bShowImGuiMetricsWindow);
			ImGui::Checkbox("Texture Window", &_bShowTextureTree);
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
		if (ImGui::Checkbox("Vertical Sync", &pixelplotter->vSync)) {
			ofSetVerticalSync(pixelplotter->vSync);
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

	ImGui::EndMainMenuBar();
}

// TODO: This should be a general texture viewer...
void OfGui::drawCanvas()
{
	
	// TODO: Add scale and move lock/unlock using mouse click?
	// Rename to "Zoomed View" that follows the mouse on main canvas

	ImGui::Begin("Canvas", &_bShowCanvas);

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

void OfGui::drawToolPalette()
{
	ImGui::Begin("Tool palette", &_bShowToolPalette);

	// Set bigger font, needs to be loaded first...
	if (ImGui::Button(ICON_FA_PEN "Pencil"))
	{
		cout << "Pencil Button Pressed";
	}

	ImGui::End();
}

void OfGui::drawInfoPanel() // B TODO: Info Panel is Presets? Dude
{
	ImGui::Begin("Presets", &_bShowInfoPanel);

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

void OfGui::drawCanvasWindow()
{
	ImGui::Begin("Canvas", &_bShowPlotCanvas);
	pixelplotter->plotCanvas.renderImGuiSettings();
	ImGui::End();
}

void OfGui::drawPropertiesWindow()
{
	ImGui::Begin("Properties", &_bShowPropertiesWindow);

	if (_selectedItem) {
		auto props = _selectedItem->getProperties();
		for (auto& p : props) {
			//p.Render();
			std::string sItemName;
			ImGui::PushID(p.ID);
			switch (p.type)
			{
			case EPT_UINT:
			{
				uint32_t* data = (uint32_t*)p.pData;
				uint32_t dMin = 1;
				uint32_t dMax = 1200;
				if (ImGui::DragScalar(p.name.c_str(), ImGuiDataType_U32, p.pData, 1, &dMin, &dMax)) {
					//setFresh(true);
				}
				break;
			}
			default:
				break;


			}
			ImGui::PopID();
		}
	}

	ImGui::End();
}

void OfGui::drawTextureBrowser()
{
	ImGui::Begin("Textures", &_bShowTextureTree, ImGuiWindowFlags_MenuBar);

	if (ImGui::BeginMenuBar()) {
		if (ImGui::BeginMenu("Add")) {
			if (ImGui::MenuItem("New Texture")) {
				pixelplotter->textureController.add((std::string)"Texture");
			}
			ImGui::EndMenu();
		}
		ImGui::EndMenuBar();
	}

	for (auto obj : pixelplotter->textureController.v_Objects) {
		if (obj->isOpen())
			ImGui::SetNextItemOpen(true, ImGuiCond_Once);

		if (ImGui::TreeNode(obj->getName().c_str()))
		{
			if (ImGui::SmallButton("View")) {
				_drawTexture = obj;
			}
			ImGui::TreePop();
		}
	}

	ImGui::End();
}

void OfGui::drawProjectTree()
{
	ImGui::Begin("Project", &_bShowProjectTree);

	if (ImGui::TreeNode("Layers"))
	{
		pixelplotter->plotCanvas.renderImGuiSettings();
		/*
		if (ImGui::TreeNode("Basic trees"))
		{
			for (int i = 0; i < 5; i++)
			{
				// Use SetNextItemOpen() so set the default state of a node to be open. We could
				// also use TreeNodeEx() with the ImGuiTreeNodeFlags_DefaultOpen flag to achieve the same thing!
				if (i == 0)
					ImGui::SetNextItemOpen(true, ImGuiCond_Once);

				if (ImGui::TreeNode((void*)(intptr_t)i, "Child %d", i))
				{
					ImGui::Text("blah blah");
					ImGui::SameLine();
					if (ImGui::SmallButton("button")) {}
					ImGui::TreePop();
				}
			}
			ImGui::TreePop();
		}
		*/
		ImGui::TreePop();
	}

	ImGui::End();
}

void OfGui::savePresets()
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

void OfGui::loadPresets(ofJson settings)
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

bool OfGui::guiVisible()
{
	return _bGuiVisible;
}

void OfGui::setGuiVisible(bool bVisible)
{
	_bGuiVisible = bVisible;
}

bool OfGui::menuBarVisible()
{
	return _bMenuBarVisible;
}

void OfGui::setmenuBarVisible(bool bVisible)
{
	_bMenuBarVisible = bVisible;
}

bool OfGui::renderingPaused()
{
	return _bRenderingPaused;
}

void OfGui::setRenderingPaused(bool bPaused)
{
	_bRenderingPaused = bPaused;
}

void OfGui::loadPresetDir()
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
	return getPresets(_currentPresetIndex);
}

ImVec4 OfGui::availableSpace()
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
