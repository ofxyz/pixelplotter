#pragma once

#include "imageFilter.h"

class If_dither : public ImageFilter {
public:
	If_dither() {
		name = "Dither";
		FloydSteinberg = {
			7.0f / 16.0f,
			3.0f / 16.0f,
			5.0f / 16.0f,
			1.0f / 16.0f };
	};

	void apply(ofImage* img) override;
	void renderImGuiSettings() override;
	void loadSettings(ofJson& settings) override;
	ofJson getSettings() override;

private:
	void addError(ofImage * img, float factor, glm::vec2 pos, glm::vec3 err);
	void distributeError(ofImage * img, glm::vec2 pos, glm::vec3 err);
	bool bDither = false;
	int factor = 1;
	glm::vec4 FloydSteinberg;
	int currColourOption = 0;
	std::vector<std::string> colourOptions { "Colour", "Grey", "Bitmap" };
};
