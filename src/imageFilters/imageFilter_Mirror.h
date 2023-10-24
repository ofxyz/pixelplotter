#pragma once

#include "imageFilter.h"

class If_mirror : public ImageFilter {
public:
	If_mirror() {
		name = "Mirror";
	};

	void apply(ofImage* img) override;
	void renderImGuiSettings() override;
	void loadSettings(ofJson settings) override;
	ofJson getSettings() override;

private:
	bool hMirror = false;
	bool vMirror = false;
};
