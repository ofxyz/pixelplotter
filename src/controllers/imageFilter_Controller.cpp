#include "imageFilter_Controller.h"
#include "imageFilter.h"
#include "imageFilter_Mirror.h"
#include "imageFilter_Duplicate.h"
#include "imageFilter_Tint.h"
#include "imageFilter_Blur.h"
#include "imageFilter_DrawPixel.h"

ImageFilterController::ImageFilterController()
	: Controller()
{
	// Manual Mapping ...
	mapObjectTypes["Mirror"] = createInstance<If_mirror>;
	mapObjectTypes["Duplicate"] = createInstance<If_duplicate>;
	mapObjectTypes["Tint"] = createInstance<If_tint>;
	mapObjectTypes["Blur"] = createInstance<If_blur>;
	mapObjectTypes["DrawPixel"] = createInstance<If_drawPixel>;

	for (auto p : mapObjectTypes) {
		v_objectNames.push_back(p.first);
	}

	generateMenuNames("ImageFilter");
}
