#pragma once
#include "drawFilter.h"
#include "drawFilter_Pixelate.h"
#include "drawFilter_Rings.h"
#include "drawFilter_Noise.h"
#include "drawFilter_Mesh.h"
#include "drawFilter_Lumes.h"

class DrawFilterController {
public:
	ofApp* pixelplotter{ nullptr }; 
	DrawFilterController();

	// Sorted alphabetically
	enum Filtertype {
		LUMES = 1,
		MESH,
		NOISE,
		PIXELATE,
		RINGS
	};

	const std::map <std::string, DrawFilterController::Filtertype> mapFilters{
		{ "Lumes"    , Filtertype::LUMES },
		{ "Mesh"     , Filtertype::MESH },
		{ "Noise"    , Filtertype::NOISE },
		{ "Pixelate" , Filtertype::PIXELATE },
		{ "Rings"    , Filtertype::RINGS }
	};

	std::vector<std::string> v_DrawFilterNames;

	void update();

	template <typename t> void move(std::vector<t>& v, size_t oldIndex, size_t newIndex);

	std::vector<std::shared_ptr<DrawFilter>> v_DrawFilters;

	void addFilter(Filtertype filterType);
	void addFilter(ofJson filterSettings);
	void addRandomFilter();

	void reorder();
	void cleanFilters();
	void clearFilters();

	bool isFresh();
	void setFresh(bool fresh);

private:
	bool _bFresh = false;

};
