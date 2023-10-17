#pragma once
#include <string>
#include <vector>
class DrawPixel;

class DrawPixels {
public:
	std::vector<std::string> v_DrawPixelsNames;
	std::vector<DrawPixel*> v_DrawPixels;

	DrawPixels();
};
