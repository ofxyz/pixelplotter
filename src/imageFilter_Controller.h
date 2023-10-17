#pragma once
#include "imageFilter.h"

class ImageFilterController {
public:
	bool bFresh = false;
	bool isFresh();
	void setFresh(bool fresh);

	void update();

	std::vector<ImageFilter*> v_ImageFilters;

	std::vector<std::string> v_ImageFilterNames{
		"Add Image Filter ...",
		"Mirror",
		"Duplicate",
		"Tint",
		"Blur",
		"DrawPixel"
	};

	void addFilter(ImageFilter* filter);

	void addFilter(int index);

	void addFilter(ofJson filterSettings);

	void addRandomFilter();

	void cleanFilters();

	void clearFilters();
};
