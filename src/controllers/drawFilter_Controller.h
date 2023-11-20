#pragma once
#include "controller.h"
#include "drawFilter.h"

class DrawFilterController : public Controller<DrawFilter> {
public:
	DrawFilterController();
	void draw(ofImage* img, int width, int height);
};
