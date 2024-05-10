#pragma once
#include "ppVectorController.h"
#include "drawFilter.h"

class DrawFilterController : public ppVectorController<DrawFilter>
{
	public:
		DrawFilterController();
		void draw(ofImage* img, int width, int height);
};
