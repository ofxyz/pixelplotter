#pragma once
#include "drawFilter_Controller.h"
#include "drawFilter_Pixelate.h"
#include "drawFilter_Pixelate2.h"
#include "drawFilter_Rings.h"
#include "drawFilter_Noise.h"
#include "drawFilter_Mesh.h"
#include "drawFilter_Lumes.h"
#include "ofx2d.h"

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

	v_MenuValues.push_back("Add Plotter ...");
	for (std::string s : v_ObjectNames) {
		v_MenuValues.push_back(s);
	}
}

template <typename t> void DrawFilterController::move(std::vector<t>& v, size_t oldIndex, size_t newIndex)
{
	if (oldIndex > newIndex)
		std::rotate(v.rend() - oldIndex - 1, v.rend() - oldIndex, v.rend() - newIndex);
	else
		std::rotate(v.begin() + oldIndex, v.begin() + oldIndex + 1, v.begin() + newIndex + 1);
}

void DrawFilterController::update() {
	for (const auto& o : v_Objects) {
		if (o->isFresh()) {
			o->setFresh(false);
			setFresh(true);
		}
	}
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