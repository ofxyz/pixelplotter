#pragma once
#include "generator_Plaids.h"

class GeneratorController {
public:
	template <typename t> void move(std::vector<t>& v, size_t oldIndex, size_t newIndex)
	{
		if (oldIndex > newIndex)
			std::rotate(v.rend() - oldIndex - 1, v.rend() - oldIndex, v.rend() - newIndex);
		else
			std::rotate(v.begin() + oldIndex, v.begin() + oldIndex + 1, v.begin() + newIndex + 1);
	}
	ofApp* pixelplotter;
	GeneratorController() {};
	GeneratorController(ofApp* app) {
		pixelplotter = app;
	};

	std::vector<Generator*> v_Generators;

	std::vector<std::string> v_GeneratorNames{
		"Add Generator ...",
		"Plaids"
	};

	void reorder() {
		for (int i = 0; i < v_Generators.size(); i++) {
			if (v_Generators[i]->moveUp) {
				v_Generators[i]->moveUp = false;
				v_Generators[i]->setFresh(true);
				if (i > 0) {
					move(v_Generators, i, i - 1);
				}
			}
			else if (v_Generators[i]->moveDown) {
				v_Generators[i]->moveDown = false;
				v_Generators[i]->setFresh(true);
				if (i < v_Generators.size() - 1) {
					move(v_Generators, i, i + 1);
				}
			}
		}
	}

	void addGenerator(Generator* generator) {
		v_Generators.push_back(generator);
	}

	void addGenerator(int index) {
		if (v_GeneratorNames[index] == "Plaids") {
			v_Generators.push_back(new G_plaids(pixelplotter));
		}
	}

	void addGenerator(string name) {
		if (name == "Plaids") {
			v_Generators.push_back(new G_plaids(pixelplotter));
		}
	}

	void addGenerator(ofxXmlSettings filterSettings) {
		string filterName = filterSettings.getValue("name", "not_found");
		if (filterName == "Plaids") {
			v_Generators.push_back(new G_plaids(pixelplotter));
			v_Generators[v_Generators.size() - 1]->loadSettings(filterSettings);
		}
	}

	void addRandomGenerator() {
		addGenerator(ofRandom(1, v_GeneratorNames.size() - 1));
	}

	void cleanFilters() {
		for (int i = 0; i < v_Generators.size(); i++) {
			if (!v_Generators[i]->active) {
				delete v_Generators[i];
				v_Generators[i] = nullptr;
			}
		}
		v_Generators.erase(std::remove(v_Generators.begin(), v_Generators.end(), nullptr), v_Generators.end());
	}

	void clearFilters() {
		for (int i = 0; i < v_Generators.size(); i++) {
			delete v_Generators[i];
			v_Generators[i] = nullptr;
		}
		v_Generators.clear();
	}

};
