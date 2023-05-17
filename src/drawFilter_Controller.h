#pragma once
#include "drawFilter_Pixelate.h"
#include "drawFilter_Rings.h"
#include "drawFilter_Noise.h"
#include "drawFilter_Mesh.h"

class DrawFilterController {
public:
	void update() {
		for (const auto& filter : v_DrawFilters) {
			if (filter->isFresh()) {
				filter->setFresh(false);
				setFresh(true);
			}
		}
	}

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
		"Noise",
		"Mesh"
	};

	DrawFilterController() {};
	DrawFilterController(ofApp* app) {
		pixelplotter = app;
	};

	void reorder() {
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

	void addFilter(DrawFilter* filter) {
		v_DrawFilters.push_back(filter);
		setFresh(true);
	}

	void addFilter(int index) {
		if (v_DrawFilterNames[index] == "Pixelate") {
			v_DrawFilters.push_back(new Df_pixelate(pixelplotter));
			setFresh(true);
		}
		else if (v_DrawFilterNames[index] == "Rings") {
			v_DrawFilters.push_back(new Df_rings(pixelplotter));
			setFresh(true);
		}
		else if (v_DrawFilterNames[index] == "Noise") {
			v_DrawFilters.push_back(new Df_noise(pixelplotter));
			setFresh(true);
		}
		else if (v_DrawFilterNames[index] == "Mesh") {
			v_DrawFilters.push_back(new Df_mesh(pixelplotter));
			setFresh(true);
		}
	}

	void addFilter(ofxXmlSettings filterSettings) {
		string filterName = filterSettings.getValue("name", "not_found");
		if (filterName == "Pixelate") {
			v_DrawFilters.push_back(new Df_pixelate(pixelplotter));
			v_DrawFilters[v_DrawFilters.size() - 1]->loadSettings(filterSettings);
			setFresh(true);
		}
		else if (filterName == "Rings") {
			v_DrawFilters.push_back(new Df_rings(pixelplotter));
			v_DrawFilters[v_DrawFilters.size() - 1]->loadSettings(filterSettings);
			setFresh(true);
		}
		else if (filterName == "Noise") {
			v_DrawFilters.push_back(new Df_noise(pixelplotter));
			v_DrawFilters[v_DrawFilters.size() - 1]->loadSettings(filterSettings);
			setFresh(true);
		}
		else if (filterName == "Mesh") {
			v_DrawFilters.push_back(new Df_mesh(pixelplotter));
			v_DrawFilters[v_DrawFilters.size() - 1]->loadSettings(filterSettings);
			setFresh(true);
		}
	}

	void addRandomFilter() {
		addFilter(ofRandom(1, v_DrawFilterNames.size() - 2));
	}

	void cleanFilters() {
		for (int i = 0; i < v_DrawFilters.size(); i++) {
			if (!v_DrawFilters[i]->active) {
				delete v_DrawFilters[i];
				v_DrawFilters[i] = nullptr;
				setFresh(true);
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
		setFresh(true);
	}

	bool isFresh() {
		return bFresh;
	}
	void setFresh(bool fresh) {
		bFresh = fresh;
	}

private:
	bool bFresh = false;

};
