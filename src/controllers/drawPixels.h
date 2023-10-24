#pragma once
#include <string>
#include <vector>
class DrawPixel;

//TODO: Should this be called DrawPixel_Controller?
class DrawPixels {
public:
	std::vector<std::string> v_DrawPixelsNames;
	std::vector<DrawPixel*> v_DrawPixels;

	DrawPixels();
};
