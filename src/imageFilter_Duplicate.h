#pragma once

#include "imageFilter.h"

class If_duplicate : public ImageFilter {
public:
	If_duplicate() {
		name = "Duplicate";
	};

	void apply(ofImage* img) override;
	void renderImGuiSettings() override;
	void loadSettings(ofJson settings) override;
	ofJson getSettings() override;

private:
	int hCount = 1;
	int vCount = 1;
	bool bMirror = true;
};