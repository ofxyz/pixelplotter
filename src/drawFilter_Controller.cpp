#pragma once
#include "drawFilter_Controller.h"

void DrawFilterController::update() {
	for (const auto& filter : v_DrawFilters) {
		if (filter->isFresh()) {
			filter->setFresh(false);
			setFresh(true);
		}
	}
}

template <typename t> void DrawFilterController::move(std::vector<t>& v, size_t oldIndex, size_t newIndex)
{
	if (oldIndex > newIndex)
		std::rotate(v.rend() - oldIndex - 1, v.rend() - oldIndex, v.rend() - newIndex);
	else
		std::rotate(v.begin() + oldIndex, v.begin() + oldIndex + 1, v.begin() + newIndex + 1);
}

void DrawFilterController::reorder() {
	for (int i = 0; i < v_DrawFilters.size(); i++) {
		if (v_DrawFilters[i]->moveUp) {
			v_DrawFilters[i]->moveUp = false;
			setFresh(true);
			if (i > 0) {
				move(v_DrawFilters, i, i - 1);
			}
		}
		else if (v_DrawFilters[i]->moveDown) {
			v_DrawFilters[i]->moveDown = false;
			setFresh(true);
			if (i < v_DrawFilters.size() - 1) {
				move(v_DrawFilters, i, i + 1);
			}
		}
	}
}

void DrawFilterController::addFilter(Filtertype filterType) {

		if (filterType == Filtertype::PIXELATE)
		{
			v_DrawFilters.push_back(std::make_shared<Df_pixelate>());
		}
		else if (filterType == Filtertype::RINGS)
		{
			v_DrawFilters.push_back(std::make_shared<Df_rings>());
		}
		else if (filterType == Filtertype::NOISE)
		{
			v_DrawFilters.push_back(std::make_shared<Df_noise>());
		}
		else if (filterType == Filtertype::MESH)
		{
			v_DrawFilters.push_back(std::make_shared<Df_mesh>());
		}
		else if (filterType == Filtertype::LUMES)
		{
			v_DrawFilters.push_back(std::make_shared<Df_lumes>());
		}
		setFresh(true);
}

void DrawFilterController::addFilter(ofJson filterSettings) {
	try {
		Filtertype filterType = mapFilters.at(filterSettings.value("name", "not_found"));

		if (filterType == Filtertype::PIXELATE)
		{
			v_DrawFilters.push_back(std::make_shared<Df_pixelate>(filterSettings));
		}
		else if (filterType == Filtertype::RINGS)
		{
			v_DrawFilters.push_back(std::make_shared<Df_rings>(filterSettings));
		}
		else if (filterType == Filtertype::NOISE)
		{
			v_DrawFilters.push_back(std::make_shared<Df_noise>(filterSettings));
		}
		else if (filterType == Filtertype::MESH)
		{
			v_DrawFilters.push_back(std::make_shared<Df_mesh>(filterSettings));
		}
		else if (filterType == Filtertype::LUMES)
		{
			v_DrawFilters.push_back(std::make_shared<Df_lumes>(filterSettings));
		} 
		setFresh(true);
	}
	catch (...) {
		return;
	}
}

void DrawFilterController::addRandomFilter() {
	addFilter(Filtertype(ofRandom(0, mapFilters.size())));
}

void DrawFilterController::cleanFilters() {
	for (int i = 0; i < v_DrawFilters.size(); i++) {
		if (!v_DrawFilters[i]->active) {
			v_DrawFilters[i] = nullptr;
			setFresh(true);
		}
	}
	v_DrawFilters.erase(std::remove(v_DrawFilters.begin(), v_DrawFilters.end(), nullptr), v_DrawFilters.end());
}

void DrawFilterController::clearFilters() {
	v_DrawFilters.clear();
	setFresh(true);
}
