#include "imageFilter_Controller.h"
#include "imageFilter_Mirror.h"
#include "imageFilter_Duplicate.h"
#include "imageFilter_Tint.h"
#include "imageFilter_Blur.h"
#include "imageFilter_DrawPixel.h"

bool ImageFilterController::isFresh()
{
	return bFresh;
}

void ImageFilterController::setFresh(bool fresh)
{
	bFresh = fresh;
}

void ImageFilterController::update()
{
	for (const auto& filter : v_ImageFilters) {
		if (filter->isFresh()) {
			bFresh = true;
			filter->setFresh(false);
		}
	}
}

void ImageFilterController::addFilter(ofJson filterSettings)
{
	string filterName = filterSettings.value("name", "not_found");
	if (filterName == "Mirror") {
		v_ImageFilters.push_back(new If_mirror);
		v_ImageFilters[v_ImageFilters.size() - 1]->loadSettings(filterSettings);
		bFresh = true;
	}
	else if (filterName == "Duplicate") {
		v_ImageFilters.push_back(new If_duplicate);
		v_ImageFilters[v_ImageFilters.size() - 1]->loadSettings(filterSettings);
		bFresh = true;
	}
	else if (filterName == "Tint") {
		v_ImageFilters.push_back(new If_tint);
		v_ImageFilters[v_ImageFilters.size() - 1]->loadSettings(filterSettings);
		bFresh = true;
	}
	else if (filterName == "Blur") {
		v_ImageFilters.push_back(new If_blur);
		v_ImageFilters[v_ImageFilters.size() - 1]->loadSettings(filterSettings);
		bFresh = true;
	}
	else if (filterName == "DrawPixel") {
		v_ImageFilters.push_back(new If_drawPixel);
		v_ImageFilters[v_ImageFilters.size() - 1]->loadSettings(filterSettings);
		bFresh = true;
	}
}

void ImageFilterController::addFilter(int index)
{
	if (v_ImageFilterNames[index] == "Mirror") {
		v_ImageFilters.push_back(new If_mirror);
		bFresh = true;
	}
	else if (v_ImageFilterNames[index] == "Duplicate") {
		v_ImageFilters.push_back(new If_duplicate);
		bFresh = true;
	}
	else if (v_ImageFilterNames[index] == "Tint") {
		v_ImageFilters.push_back(new If_tint);
		bFresh = true;
	}
	else if (v_ImageFilterNames[index] == "Blur") {
		v_ImageFilters.push_back(new If_blur);
		bFresh = true;
	}
	else if (v_ImageFilterNames[index] == "DrawPixel") {
		v_ImageFilters.push_back(new If_drawPixel);
		bFresh = true;
	}
}

void ImageFilterController::addFilter(ImageFilter* filter)
{
	v_ImageFilters.push_back(filter);
	bFresh = true;
}

void ImageFilterController::addRandomFilter()
{
	addFilter(ofRandom(1, v_ImageFilterNames.size() - 1));
}

void ImageFilterController::cleanFilters()
{
	for (int i = 0; i < v_ImageFilters.size(); i++) {
		if (!v_ImageFilters[i]->active) {
			delete v_ImageFilters[i];
			v_ImageFilters[i] = nullptr;
			bFresh = true;
		}
	}
	v_ImageFilters.erase(std::remove(v_ImageFilters.begin(), v_ImageFilters.end(), nullptr), v_ImageFilters.end());
}

void ImageFilterController::clearFilters()
{
	for (int i = 0; i < v_ImageFilters.size(); i++) {
		delete v_ImageFilters[i];
		v_ImageFilters[i] = nullptr;
	}
	v_ImageFilters.erase(std::remove(v_ImageFilters.begin(), v_ImageFilters.end(), nullptr), v_ImageFilters.end());
	bFresh = true;
}
