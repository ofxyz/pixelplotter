#include "DrawPixel_Controller.h"
#include "drawPixel.h"
#include "drawPixel_Normal.h"
#include "drawPixel_SeparateCMY_Bars01.h"
#include "drawPixel_SeparateCMYK_Bars01.h"
#include "drawPixel_SeparateRGB_Bars01.h"
#include "drawPixel_SeparateCMYK_Para01.h"
#include "drawPixel_SeparateCMYK_Rect01.h"
#include "drawPixel_SeparateCMYK_Rect02.h"
#include "drawPixel_SeparateCMYK_Rect03.h"
#include "drawPixel_SeparateCMYK_Rect04.h"

DrawPixelController::DrawPixelController()
	: Controller()
{
	// Manual Mapping ...
	
	mapObjectTypes["Normal"] = createInstance<Dp_normal>;
	mapObjectTypes["Separate CMY Bars 01"]  = createInstance<Dp_separateCMY_Bars01>;
	mapObjectTypes["Separate CMYK Bars 01"] = createInstance<Dp_separateCMYK_Bars01>;
	mapObjectTypes["Separate RGB Bars 01"]  = createInstance<Dp_separateRGB_Bars01>;
	mapObjectTypes["Separate CMYK Para 01"] = createInstance<Dp_separateCMYK_Para01>;
	mapObjectTypes["Separate CMYK Rect 01"] = createInstance<Dp_separateCMYK_Rect01>;
	mapObjectTypes["Separate CMYK Rect 02"] = createInstance<Dp_separateCMYK_Rect02>;
	mapObjectTypes["Separate CMYK Rect 03"] = createInstance<Dp_separateCMYK_Rect03>;
	mapObjectTypes["Separate CMYK Rect 04"] = createInstance<Dp_separateCMYK_Rect04>;

	for (auto p : mapObjectTypes) {
		v_objectNames.push_back(p.first);
		add(p.first);
	}

	generateMenuNames("DrawPixels");

}
