#pragma once
#include "imageFilter.h"

class ofApp;

class ImageFilterController {
public:
	ofApp* pixelplotter{ nullptr };
	ImageFilterController();

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

	void clean();
	void clear();

	bool isFresh();
	void setFresh(bool fresh);

private:
	bool bFresh = false;
};
