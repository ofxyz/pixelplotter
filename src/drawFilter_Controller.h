#pragma once
#include "drawFilter_Pixelate.h"
#include "drawFilter_Rings.h"

class DrawFilterController {
public:

	std::vector<DrawFilter*> v_DrawFilters;

	std::vector<std::string> v_DrawFilterNames{
		"Add Draw Filter ...",
		"Pixelate",
		"Rings"
	};

	void addFilter(DrawFilter* filter) {
		v_DrawFilters.push_back(filter);
	}

	void addFilter(int index) {
		if (v_DrawFilterNames[index] == "Pixelate") {
			v_DrawFilters.push_back(new Df_pixelate);
		}
		else if (v_DrawFilterNames[index] == "Rings") {
			v_DrawFilters.push_back(new Df_rings);
		}
	}

	void addFilter(ofxXmlSettings filterSettings) {
		string filterName = filterSettings.getValue("name", "not_found");
		if (filterName == "Pixelate") {
			v_DrawFilters.push_back(new Df_pixelate);
			v_DrawFilters[v_DrawFilters.size() - 1]->loadSettings(filterSettings);
		}
		else if (filterName == "Rings") {
			v_DrawFilters.push_back(new Df_rings);
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