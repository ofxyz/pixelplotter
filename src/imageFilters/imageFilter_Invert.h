#pragma once

#include "imageFilter.h"

class If_invert : public ImageFilter {
public:
	If_invert() {
		name = "Invert";
	};

	void apply(ofImage* img) override;
	void renderImGuiSettings() override;
	void loadSettings(ofJson& settings) override;
	ofJson getSettings() override;

};
