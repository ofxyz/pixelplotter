#pragma once
#include <string>
#include <vector>

class DrawPixel;

class DrawPixelController {
public:
	std::vector<std::string> v_DrawPixelsNames;
	std::vector<DrawPixel*> v_DrawPixels;

	DrawPixelController();
};
