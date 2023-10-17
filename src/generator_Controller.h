#pragma once
#include "ofJson.h"
class Generator;
class ofApp;

class GeneratorController {
public:
	template <typename t> void move(std::vector<t>& v, size_t oldIndex, size_t newIndex);
	ofApp* pixelplotter;
	GeneratorController();
	GeneratorController(ofApp* app);;

	std::vector<Generator*> v_Generators;

	std::vector<std::string> v_GeneratorNames;

	void reorder();

	void addGenerator(Generator* generator);

	void addGenerator(int index);

	void addGenerator(std::string name);

	void addGenerator(ofJson filterSettings);

	void addRandomGenerator();

	void cleanFilters();

	void clearFilters();

};
