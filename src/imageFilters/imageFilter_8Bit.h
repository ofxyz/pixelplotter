#pragma once

#include "imageFilter.h"

class If_8bit : public ImageFilter {
public:
	If_8bit() {
		name = "8Bit";
	};

	void apply(ofImage* img) override;
	void renderImGuiSettings() override;
	void loadSettings(ofJson& settings) override;
	ofJson getSettings() override;

private:
	void addError(ofImage * img, float factor, glm::vec2 pos, glm::vec3 err);
	void distributeError(ofImage * img, glm::vec2 pos, glm::vec3 err);
	bool bDither = false;
	int factor = 0;
};
