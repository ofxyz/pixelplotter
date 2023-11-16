#pragma once
#include "ofJson.h"
class Generator;
class ofApp;

class GeneratorController {
public:
	ofApp* pixelplotter;
	GeneratorController();

	std::vector<Generator*> v_Generators;

	std::vector<std::string> v_ObjectNames;
	std::vector<std::string> v_MenuValues;

	template <typename t> void move(std::vector<t>& v, size_t oldIndex, size_t newIndex);

	void generateMenuNames();

	void reorder();

	void addGenerator(Generator* generator);

	void addGenerator(int index);

	void addGenerator(std::string name);

	void addGenerator(ofJson& filterSettings);

	void addRandomGenerator();

	void cleanFilters();

	void clearFilters();

};
