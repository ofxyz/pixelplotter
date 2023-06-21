#pragma once

#include "drawPixel_Normal.h"
#include "drawPixel_SeparateCMYK.h"

class DrawPixels {
public:
	std::vector<std::string> v_DrawPixelsNames;
	std::vector<DrawPixel*> v_DrawPixels;

	DrawPixels() {
		v_DrawPixels.push_back(new Dp_normal());
		v_DrawPixels.push_back(new Dp_separateCMYK());

		for (int i = 0; i < v_DrawPixels.size(); i++) {
			v_DrawPixelsNames.push_back(v_DrawPixels[i]->name);
		};

	};
};