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
	int factor = 0;
};