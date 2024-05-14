#include "vc_ImageFilter.h"
#include "imageFilter.h"
#include "imageFilter_Mirror.h"
#include "imageFilter_Duplicate.h"
#include "imageFilter_Tint.h"
#include "imageFilter_Blur.h"
#include "imageFilter_DrawPixel.h"
#include "imageFilter_Rotate.h"
#include "imageFilter_Ornament.h"
#include "imageFilter_Dither.h"
#include "imageFilter_Invert.h"

ImageFilterController::ImageFilterController()
	: ppVectorController()
{
	// Manual Mapping
	mapObjectTypes["Mirror"]    = createInstance<If_mirror>;
	mapObjectTypes["Duplicate"] = createInstance<If_duplicate>;
	mapObjectTypes["Tint"]      = createInstance<If_tint>;
	mapObjectTypes["Blur"]      = createInstance<If_blur>;
	mapObjectTypes["DrawPixel"] = createInstance<If_drawPixel>;
	mapObjectTypes["Rotate"]    = createInstance<If_rotate>;
	mapObjectTypes["Ornament"]  = createInstance<If_ornament>;
	mapObjectTypes["Dither"]    = createInstance<If_dither>;
	mapObjectTypes["Invert"]    = createInstance<If_invert>;

	for (auto p : mapObjectTypes) {
		v_objectNames.push_back(p.first);
	}

	generateMenuNames("ImageFilters");
}
