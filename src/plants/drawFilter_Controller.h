#pragma once
#include "drawFilter.h"

class DrawFilterController {
public:
	ofApp* pixelplotter{ nullptr }; 
	DrawFilterController();

	std::vector<std::string> v_ObjectNames;
	std::vector<std::shared_ptr<DrawFilter>> v_Objects;
	std::vector<std::string> v_MenuValues;

	template <typename t> void move(std::vector<t>& v, size_t oldIndex, size_t newIndex);

	void update();
	void draw(ofImage* img, int width, int height);
	void add(std::string name, ofJson filterSettings = {});
	void add(ofJson filterSettings);
	void addRandom();

	void reorder();
	void cleanFilters();
	void clearFilters();

	bool isFresh();
	void setFresh(bool fresh);

private:
	bool _bFresh = false;
	void generateMenuNames();

};
