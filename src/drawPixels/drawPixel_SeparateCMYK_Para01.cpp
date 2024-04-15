#include "drawPixel_SeparateCMYK_Para01.h"

ofJson Dp_separateCMYK_Para01::getSettings() {
	ofJson settings;
	settings["name"] = name;
	return settings;
}

void Dp_separateCMYK_Para01::loadSettings(ofJson& settings) {
	// name = settings.value("name", name);
	return;
}

void Dp_separateCMYK_Para01::renderImGuiSettings() {
	// Add draw order (shuffle ofVec4f)
	// Save settings
	renderImGuiColourSettings();
}

void Dp_separateCMYK_Para01::draw(ofColor c, glm::vec2 dim, glm::vec2 pos /*= { 0,0 }*/, glm::vec2 posNorm /*= { 0,0 }*/)
{
	glm::vec4 cmyk = ofx2d::getCMYK(c);

	float whiteVal = min(min(c.r, c.g), c.b);
	if (whiteVal > 0) {
		whiteVal /= 255;
	}

	float totalInk = cmyk[0] + cmyk[1] + cmyk[2] + cmyk[3] + whiteVal;

	// TODO: Make a rich black conversion
	// Or maybe add adjustment curves?
	/*
	if (cmyk[3] > 0.1) {
		cmyk[3] *= 0.5;
		double z = cmyk[3] * 0.5;
		// Don't add new colors just rich existing
		if (cmyk[0] > 0) cmyk[0] += z;
		if (cmyk[1] > 0) cmyk[1] += z;
		if (cmyk[2] > 0) cmyk[2] += z;
	}
	*/

	cmyk[0] = ofMap(cmyk[0], 0, totalInk, 0, 1);
	cmyk[1] = ofMap(cmyk[1], 0, totalInk, 0, 1);
	cmyk[2] = ofMap(cmyk[2], 0, totalInk, 0, 1);
	cmyk[3] = ofMap(cmyk[3], 0, totalInk, 0, 1);

	// Lets out the colors and densities in pairs for sorting
	// But ignore anything which isn't drawn ...
	std::vector<std::pair<float, /*color*/ImVec4>> cmykValues;
	
	if (cmyk[0] > 0) cmykValues.push_back(make_pair(cmyk[0], c_cyanBlue));
	if (cmyk[1] > 0) cmykValues.push_back(make_pair(cmyk[1], c_magentaRed));
	if (cmyk[2] > 0) cmykValues.push_back(make_pair(cmyk[2], c_yellowGreen));
	if (cmyk[3] > 0) cmykValues.push_back(make_pair(cmyk[3], c_black));
	if (whiteVal > 0) cmykValues.push_back(make_pair(whiteVal, c_white));

	if (cmykValues.size() == 0) return;

	sort(cmykValues.begin(), cmykValues.end(), [=](std::pair<float, /*color*/ImVec4>& a, std::pair<float, /*color*/ImVec4>& b)
		{
			return a.first > b.first;
		}
	);

	glm::vec2 start = { -(dim.x * 0.5), 0 };
	float barWidth = dim.x; // Space left

	/* white at end ...
	if (whiteVal > 0) { // Offset for white
		barWidth = whiteVal * width;
		start += barWidth;
	}
	*/

	for (auto const& c : cmykValues)
	{
		barWidth = c.first * dim.x;
		start.x += (barWidth * 0.5);
		drawRectangle(start.x, start.y, barWidth, dim.y, c.second);
		start.x += (barWidth * 0.5);
	}

}
