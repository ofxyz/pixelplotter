#pragma once
#include "drawFilter_Pixelate.h"
#include "drawFilter_Rings.h"
#include "drawFilter_Noise.h"

class DrawFilterController {
public:
	template <typename t> void move(std::vector<t>& v, size_t oldIndex, size_t newIndex)
	{
		if (oldIndex > newIndex)
			std::rotate(v.rend() - oldIndex - 1, v.rend() - oldIndex, v.rend() - newIndex);
		else
			std::rotate(v.begin() + oldIndex, v.begin() + oldIndex + 1, v.begin() + newIndex + 1);
	}

	ofApp* pixelplotter;
	std::vector<DrawFilter*> v_DrawFilters;

	std::vector<std::string> v_DrawFilterNames{
		"Add Plotter ...",
		"Pixelate",
		"Rings",
		"Noise"
	};

	DrawFilterController() {};
	DrawFilterController(ofApp* app) {
		pixelplotter = app;
	};

	void reorder() {
		for (int i = 0; i < v_DrawFilters.size(); i++) {
			if (v_DrawFilters[i]->moveUp) {
				v_DrawFilters[i]->moveUp = false;
				v_DrawFilters[i]->setFresh(true);
				if (i > 0) {
					move(v_DrawFilters, i, i - 1);
				}
			}
			else if (v_DrawFilters[i]->moveDown) {
				v_DrawFilters[i]->moveDown = false;
				v_DrawFilters[i]->setFresh(true);
				if (i < v_DrawFilters.size() - 1) {
					move(v_DrawFilters, i, i + 1);
				}
			}
		}
	}

	void addFilter(DrawFilter* filter) {
		v_DrawFilters.push_back(filter);
	}

	void addFilter(int index) {
		if (v_DrawFilterNames[index] == "Pixelate") {
			v_DrawFilters.push_back(new Df_pixelate(pixelplotter));
		}
		else if (v_DrawFilterNames[index] == "Rings") {
			v_DrawFilters.push_back(new Df_rings(pixelplotter));
		}
		else if (v_DrawFilterNames[index] == "Noise") {
			v_DrawFilters.push_back(new Df_noise(pixelplotter));
		}
	}

	void addFilter(ofxXmlSettings filterSettings) {
		string filterName = filterSettings.getValue("name", "not_found");
		if (filterName == "Pixelate") {
			v_DrawFilters.push_back(new Df_pixelate(pixelplotter));
			v_DrawFilters[v_DrawFilters.size() - 1]->loadSettings(filterSettings);
		}
		else if (filterName == "Rings") {
			v_DrawFilters.push_back(new Df_rings(pixelplotter));
			v_DrawFilters[v_DrawFilters.size() - 1]->loadSettings(filterSettings);
		}
		else if (filterName == "Noise") {
			v_DrawFilters.push_back(new Df_noise(pixelplotter));
			v_DrawFilters[v_DrawFilters.size() - 1]->loadSettings(filterSettings);
		}
	}

	void addRandomFilter() {
		addFilter(ofRandom(1, v_DrawFilterNames.size() - 1));
	}

	void cleanFilters() {
		for (int i = 0; i < v_DrawFilters.size(); i++) {
			if (!v_DrawFilters[i]->active) {
				delete v_DrawFilters[i];
				v_DrawFilters[i] = nullptr;
			}
		}
		v_DrawFilters.erase(std::remove(v_DrawFilters.begin(), v_DrawFilters.end(), nullptr), v_DrawFilters.end());
	}

	void clearFilters() {
		for (int i = 0; i < v_DrawFilters.size(); i++) {
			delete v_DrawFilters[i];
			v_DrawFilters[i] = nullptr;
		}
		v_DrawFilters.clear();
	}

};