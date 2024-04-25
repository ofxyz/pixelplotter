#include "imageFilter_Controller.h"
#include "imageFilter.h"
#include "imageFilter_Mirror.h"
#include "imageFilter_Duplicate.h"
#include "imageFilter_Tint.h"
#include "imageFilter_Blur.h"
#include "imageFilter_DrawPixel.h"
#include "imageFilter_Rotate.h"
#include "imageFilter_Ornament.h"
#include "imageFilter_8Bit.h"
#include "imageFilter_Invert.h"

ImageFilterController::ImageFilterController()
	: Controller()
{
	// Manual Mapping ...
	mapObjectTypes["Mirror"] = createInstance<If_mirror>;
	mapObjectTypes["Duplicate"] = createInstance<If_duplicate>;
	mapObjectTypes["Tint"] = createInstance<If_tint>;
	mapObjectTypes["Blur"] = createInstance<If_blur>;
	mapObjectTypes["DrawPixel"] = createInstance<If_drawPixel>;
	mapObjectTypes["Rotate"] = createInstance<If_rotate>;
	mapObjectTypes["Ornament"] = createInstance<If_ornament>;
	mapObjectTypes["8Bit"] = createInstance<If_8bit>;
	mapObjectTypes["Invert"] = createInstance<If_invert>;

	for (auto p : mapObjectTypes) {
		v_objectNames.push_back(p.first);
	}

	generateMenuNames("ImageFilters");
}
