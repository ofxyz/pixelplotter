#pragma once

#include "imageFilter.h"

class If_rotate : public ImageFilter {
public:
	If_rotate() {
		name = "Rotate";
	};

	void apply(ofImage* img) override;
	void renderImGuiSettings() override;
	void loadSettings(ofJson& settings) override;
	ofJson getSettings() override;

private:
	int rCount = 0;
	bool bRotateCanvas = true;
	bool hMirror = false;
	bool vMirror = false;
};
