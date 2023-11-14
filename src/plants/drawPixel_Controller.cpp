#include "DrawPixel_Controller.h"
#include "drawPixel.h"
#include "drawPixel_Normal.h"
#include "drawPixel_SeparateCMYK_Bars01.h"
#include "drawPixel_SeparateRGB_Bars01.h"
#include "drawPixel_SeparateCMYK_Rect01.h"
#include "drawPixel_SeparateCMYK_Rect02.h"
#include "drawPixel_SeparateCMYK_Rect03.h"
#include "drawPixel_SeparateCMYK_Rect04.h"

DrawPixelController::DrawPixelController()
{
	v_DrawPixels.push_back(new Dp_normal());
	v_DrawPixels.push_back(new Dp_separateCMYK_Bars01());
	v_DrawPixels.push_back(new Dp_separateRGB_Bars01());
	v_DrawPixels.push_back(new Dp_separateCMYK_Rect01());
	v_DrawPixels.push_back(new Dp_separateCMYK_Rect02());
	v_DrawPixels.push_back(new Dp_separateCMYK_Rect03());
	v_DrawPixels.push_back(new Dp_separateCMYK_Rect04());

	for (int i = 0; i < v_DrawPixels.size(); i++) {
		v_DrawPixelsNames.push_back(v_DrawPixels[i]->name);
	};
}
