#pragma once
#include "ofx2d.h"

#include "ofxImGui.h"
#include "imgui_internal.h"
#include "ImHelpers.h"
#include "ImGui_Widget_Tooltip.h"
#include "ImGui_Widget_InputTextString.h"

#include "drawFilter_Controller.h"
#include "drawFilter_Pixelate.h"
#include "drawFilter_Pixelate2.h"
#include "drawFilter_Rings.h"
#include "drawFilter_Noise.h"
#include "drawFilter_Mesh.h"
#include "drawFilter_Lumes.h"

DrawFilterController::DrawFilterController()
{
	pixelplotter = (ofApp*)ofGetAppPtr();

	v_ObjectNames = {
		"Lumes",
		"Mesh",
		"Noise",
		"Pixelate",
		"Pixelate2",
		"Rings"
	};

	generateMenuNames();

}

template <typename t> void DrawFilterController::move(std::vector<t>& v, size_t oldIndex, size_t newIndex)
{
	if (oldIndex > newIndex)
		std::rotate(v.rend() - oldIndex - 1, v.rend() - oldIndex, v.rend() - newIndex);
	else
		std::rotate(v.begin() + oldIndex, v.begin() + oldIndex + 1, v.begin() + newIndex + 1);
}

void DrawFilterController::update() {

	if (cleanDrawFilters) {
		cleanFilters();
	}
	if (reorderDrawFilters) {
		reorder();
	}

	for (const auto& o : v_Objects) {
		if (o->isFresh()) {
			o->setFresh(false);
			setFresh(true);
		}
	}
}

void DrawFilterController::draw(ofImage* img, int width, int height)
{
	for (const auto& filter : v_Objects) {
		// TODO: update one filter per frame to keep things speeedy?
		// Each filter draws to it's own fbo and are drawn here?
		// filter->update(img); filter->update(settings)
		filter->draw(img, width, height);
	}
	setFresh(false);
}

void DrawFilterController::renderImGuiSettings()
{
	string sDrawFilterCount = "DrawFilters (" + ofToString(v_Objects.size()) + ")###DrawFiltersHolder";
	ImGui::SetNextItemOpen(true, ImGuiCond_Once);
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
		reorderDrawFilters = false;
		for (int i = 0; i < v_Objects.size(); i++) {
			ImGui::PushID(i);
			if (v_Objects[i]->active) {
				ImGui::Indent();
				v_Objects[i]->renderImGuiSettings();
				ImGui::Unindent();
			}
			else {
				cleanDrawFilters = true;
			}
			if (v_Objects[i]->moveUp || v_Objects[i]->moveDown) {
				reorderDrawFilters = true;
			}
			ImGui::PopID();
		}

		ImGui::PopStyleColor(10);

		ImGui::Indent(); // ADD FILTERS
		if (ofxImGui::VectorCombo("##Draw Filter Selector", &currentDrawFilterIndex, v_MenuValues))
		{
			add(v_MenuValues[currentDrawFilterIndex]);
			currentDrawFilterIndex = 0;
		}
		ImGui::Unindent();

	}
}

void DrawFilterController::loadSettings(ofJson& settings)
{
	setFresh(true);
}

ofJson DrawFilterController::getSettings()
{
	ofJson settings;

	return settings;
}

void DrawFilterController::reorder() {
	for (int i = 0; i < v_Objects.size(); i++) {
		if (v_Objects[i]->moveUp) {
			v_Objects[i]->moveUp = false;
			setFresh(true);
			if (i > 0) {
				move(v_Objects, i, i - 1);
			}
		}
		else if (v_Objects[i]->moveDown) {
			v_Objects[i]->moveDown = false;
			setFresh(true);
			if (i < v_Objects.size() - 1) {
				move(v_Objects, i, i + 1);
			}
		}
	}
}

void DrawFilterController::add(std::string name, ofJson filterSettings /*= nullptr*/ ) {
	if (name == "Pixelate")
	{
		v_Objects.push_back(std::make_shared<Df_pixelate>(filterSettings));
	}
	else if (name == "Pixelate2")
	{
		v_Objects.push_back(std::make_shared<Df_pixelate2>(filterSettings));
	}
	else if (name == "Rings")
	{
		v_Objects.push_back(std::make_shared<Df_rings>(filterSettings));
	}
	else if (name == "Noise")
	{
		v_Objects.push_back(std::make_shared<Df_noise>(filterSettings));
	}
	else if (name == "Mesh")
	{
		v_Objects.push_back(std::make_shared<Df_mesh>(filterSettings));
	}
	else if (name == "Lumes")
	{
		v_Objects.push_back(std::make_shared<Df_lumes>(filterSettings));
	}
	setFresh(true);
}

void DrawFilterController::add(ofJson filterSettings) {
	try {
		std::string name = filterSettings.value("name", "not_found");
		add(name, filterSettings);
		setFresh(true);
	}
	catch (...) {
		return;
	}
}

void DrawFilterController::addRandom() {
	add(v_ObjectNames[ofRandom(0, v_ObjectNames.size())]);
}

void DrawFilterController::cleanFilters() {
	for (int i = 0; i < v_Objects.size(); i++) {
		if (!v_Objects[i]->active) {
			v_Objects[i] = nullptr;
			setFresh(true);
		}
	}
	v_Objects.erase(std::remove(v_Objects.begin(), v_Objects.end(), nullptr), v_Objects.end());
}

void DrawFilterController::clearFilters() {
	v_Objects.clear();
	setFresh(true);
}

bool DrawFilterController::isFresh() {
	return _bFresh;
}

void DrawFilterController::setFresh(bool fresh) {
	_bFresh = fresh;
}

void DrawFilterController::generateMenuNames()
{
	v_MenuValues.clear();
	v_MenuValues.push_back("Add Draw Filter ...");
	for (std::string s : v_ObjectNames) {
		v_MenuValues.push_back(s);
	}
}
